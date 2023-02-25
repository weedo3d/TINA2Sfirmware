#include "status_screen_DOGM.h"
#include "ultralcd_impl_DOGM.h"
#include "../module/temperature.h"
#include "../module/planner.h"
#include "../module/printcounter.h"
#include "../sd/cardreader.h"
#include "../wtlib/WTGcodeinfo.h"
#include "../u8g/u8g2.h"

void _draw_centered_temp(const int16_t temp, const uint8_t x, const uint8_t y)
{
  const char *const str = i16tostr3rj(temp);
  u8g.setCursor(x - (str[0] != ' ' ? 0 : str[1] != ' ' ? 1
                                                       : 2) *
                        DOG_CHAR_WIDTH / 2,
                y);
  lcd_print(str);
  lcd_print(LCD_STR_DEGREE " ");
}

#ifndef HEAT_INDICATOR_X
#define HEAT_INDICATOR_X 8
#endif

FORCE_INLINE void _draw_heater_status(const uint8_t x, const int8_t heater, const bool blink)
{

  const bool isBed = heater < 0;

  const bool is_idle = (isBed ? thermalManager.bed_idle.timed_out :
                              // thermalManager.is_heater_idle(heater)
                            thermalManager.hotend_idle[heater].timed_out);

  if (blink || !is_idle)
    _draw_centered_temp(0.5f + (isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater)),
                        x, 7);

  _draw_centered_temp(0.5f + (isBed ? thermalManager.degBed() : thermalManager.degHotend(heater)),
                      x, 28);

  const uint8_t h = isBed ? 7 : HEAT_INDICATOR_X,
                y = isBed ? 18 : 17;
  if (
      isBed ? thermalManager.isHeatingBed() : thermalManager.isHeatingHotend(heater))
  {
    u8g.setColorIndex(0); // white on black
    u8g.drawBox(x + h, y, 2, 2);
    u8g.setColorIndex(1); // black on white
  }
  else
    u8g.drawBox(x + h, y, 2, 2);
}

void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink)
{
  if (blink)
    lcd_print(value);
  else
  {
    if (!TEST(axis_homed, axis))
      while (const char c = *value++)
        lcd_print(c <= '.' ? c : '?');
    else
    {
#if DISABLED(HOME_AFTER_DEACTIVATE) && DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
      if (!TEST(axis_known_position, axis))
        lcd_print(axis == Z_AXIS ? PSTR("      ") : PSTR("    "));
      else
#endif
        lcd_print(value);
    }
  }
}

void lcd_implementation_status_message(const bool blink)
{
  UNUSED(blink);

  // Get the UTF8 character count of the string
  // uint8_t slen = utf8_strlen(lcd_status_message);

  // Just print the string to the LCD
  lcd_print(lcd_status_message);

  // // Fill the rest with spaces if there are missing spaces
  // while (slen < LCD_WIDTH)
  // {
  //   u8g.print(' ');
  //   ++slen;
  // }
}

void lcd_implementation_status_screen()
{

  const bool blink = lcd_blink();

  // Status Menu Font
  //   lcd_setFont(FONT_STATUSMENU);

  //
  // Fan Animation
  //
  // Draws the whole heading image as a B/W bitmap rather than
  // drawing the elements separately.
  // This was done as an optimization, as it was slower to draw
  // multiple parts compared to a single bitmap.
  //
  // The bitmap:
  // - May be offset in X
  // - Includes all nozzle(s), bed(s), and the fan.
  //
  // TODO:
  //
  // - Only draw the whole header on the first
  //   entry to the status screen. Nozzle, bed, and
  //   fan outline bits don't change.
  //

  u8g.drawBitmap(
      STATUS_SCREEN_X + 8, STATUS_SCREEN_Y,
      (STATUS_SCREENWIDTH + 7) / 8, STATUS_SCREENHEIGHT, status_screen0_bmp);

  // draw wifi icon
  if (wifi_manager.getIPAddress)
  {
    if (wifi_manager.wifi_signal == 4)
      u8g.drawBitmap(STATUS_WIFI_X, STATUS_WIFI_Y, (STATUS_WIFI_WIDTH + 7) / 8, STATUS_WIFI_HEIGHT, status_wifi_4_bmp);
    else if (wifi_manager.wifi_signal == 3)
      u8g.drawBitmap(STATUS_WIFI_X, STATUS_WIFI_Y, (STATUS_WIFI_WIDTH + 7) / 8, STATUS_WIFI_HEIGHT, status_wifi_3_bmp);
    else if (wifi_manager.wifi_signal == 2)
      u8g.drawBitmap(STATUS_WIFI_X, STATUS_WIFI_Y, (STATUS_WIFI_WIDTH + 7) / 8, STATUS_WIFI_HEIGHT, status_wifi_2_bmp);
    else if (wifi_manager.wifi_signal == 1)
      u8g.drawBitmap(STATUS_WIFI_X, STATUS_WIFI_Y, (STATUS_WIFI_WIDTH + 7) / 8, STATUS_WIFI_HEIGHT, status_wifi_1_bmp);
    else
      u8g.drawBitmap(STATUS_WIFI_X, STATUS_WIFI_Y, (STATUS_WIFI_WIDTH + 7) / 8, STATUS_WIFI_HEIGHT, status_wifi_0_bmp);
  }
  else
  {
    u8g.drawBitmap(STATUS_WIFI_X, STATUS_WIFI_Y, (STATUS_WIFI_WIDTH + 7) / 8, STATUS_WIFI_HEIGHT, status_wifi_off_bmp);
  }

  //
  // Temperature Graphics and Info
  //

  // Extruders
  HOTEND_LOOP()
  _draw_heater_status(STATUS_SCREEN_HOTEND_TEXT_X(e), e, blink);

// Heated bed
#if HOTENDS < 4 && HAS_HEATED_BED
  _draw_heater_status(STATUS_SCREEN_BED_TEXT_X, -1, blink);
#endif

  //
  // SD Card Symbol
  //

  // Upper box
  u8g.drawBox(42, 42 - (TALL_FONT_CORRECTION), 8, 7); // 42-48 (or 41-47)
  // Right edge
  u8g.drawBox(50, 44 - (TALL_FONT_CORRECTION), 2, 5); // 44-48 (or 43-47)
  // Bottom hollow box
  u8g.drawFrame(42, 49 - (TALL_FONT_CORRECTION), 10, 4); // 49-52 (or 48-51)
  // Corner pixel
  u8g.drawPixel(50, 43 - (TALL_FONT_CORRECTION)); // 43 (or 42)

#if ENABLED(SDSUPPORT) || ENABLED(LCD_SET_PROGRESS_MANUALLY)
//
// Progress bar frame
//
#define PROGRESS_BAR_X 54
#define PROGRESS_BAR_WIDTH (LCD_PIXEL_WIDTH - PROGRESS_BAR_X) // 74

  // if (PAGE_CONTAINS(49, 52 - 1)) // 49-52 (or 49-51)
  u8g.drawFrame(
      PROGRESS_BAR_X, 49,
      PROGRESS_BAR_WIDTH, 4 - 1);

  if (wt_onlineprinting == SPARK_IDLE)
    progress_bar_percent = card.percentDone();

  if (progress_bar_percent > 1)
  {

    u8g.drawBox(
        PROGRESS_BAR_X + 1, 50,
        (uint16_t)((PROGRESS_BAR_WIDTH - 2) * progress_bar_percent * 0.01), 2 - 1);
  }

  //
  // Elapsed Time
  //

#define SD_DURATION_X (PROGRESS_BAR_X + (PROGRESS_BAR_WIDTH / 2) - len * (DOG_CHAR_WIDTH / 2))

  char buffer[10];
  duration_t elapsed;
  if (wtvar_status_time == 0)
  { // show printjob time
    elapsed = print_job_timer.duration();
    bool has_days = (elapsed.value >= 60 * 60 * 24L);
    uint8_t len = elapsed.toDigital(buffer, has_days);
    u8g.setCursor(SD_DURATION_X, 48);
    lcd_print(buffer);
  }
  else if (wtvar_status_time == 1)
  { // show print used time
    elapsed = print_job_timer.duration();
    bool has_days = (elapsed.value >= 60 * 60 * 24L);
    uint8_t len = elapsed.toDigital(buffer, has_days);
    u8g.setCursor(SD_DURATION_X - 16, 48);
    lcd_print(PSTR(LCD_STR_ARROW_RIGHT " "));
    lcd_print(buffer);
  }
  else
  { // show left time
    elapsed = gcodeinfo.info.i_lefttime;
    bool has_days = (elapsed.value >= 60 * 60 * 24L);
    uint8_t len = elapsed.toDigital(buffer, has_days);
    u8g.setCursor(SD_DURATION_X, 48);
    lcd_print(buffer);
    lcd_print(PSTR(" " LCD_STR_ARROW_RIGHT));
  }

#endif // SDSUPPORT || LCD_SET_PROGRESS_MANUALLY

  //
  // XYZ Coordinates
  //

#define XYZ_BASELINE (30 + INFO_FONT_HEIGHT)

#define X_LABEL_POS 3
#define X_VALUE_POS 9
#define XYZ_SPACING 38

#define XYZ_FRAME_TOP 30
#define XYZ_FRAME_HEIGHT INFO_FONT_HEIGHT + 1

  static char xstring[5], ystring[5], zstring[7];

  strcpy(xstring, ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
  strcpy(ystring, ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])));
  strcpy(zstring, ftostr52sp(LOGICAL_Z_POSITION(current_position[Z_AXIS])));

  u8g.drawBox(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT); // 8: 30-39  7: 30-37

  u8g.setColorIndex(0); // white on black

  u8g.setCursor(0 * XYZ_SPACING + X_LABEL_POS, XYZ_BASELINE);
  lcd_print(PSTR(MSG_X));
  u8g.setCursor(0 * XYZ_SPACING + X_VALUE_POS, XYZ_BASELINE);
  _draw_axis_value(X_AXIS, xstring, blink);

  u8g.setCursor(1 * XYZ_SPACING + X_LABEL_POS, XYZ_BASELINE);
  lcd_print(PSTR(MSG_Y));
  u8g.setCursor(1 * XYZ_SPACING + X_VALUE_POS, XYZ_BASELINE);
  _draw_axis_value(Y_AXIS, ystring, blink);

  u8g.setCursor(2 * XYZ_SPACING + X_LABEL_POS, XYZ_BASELINE);
  lcd_print(PSTR(MSG_Z));
  u8g.setCursor(2 * XYZ_SPACING + X_VALUE_POS, XYZ_BASELINE);
  _draw_axis_value(Z_AXIS, zstring, blink);

  u8g.setColorIndex(1); // black on white

  //
  // Feedrate
  //

  u8g.setCursor(3, 50);
  lcd_print(LCD_STR_FEEDRATE);

  // lcd_setFont(FONT_STATUSMENU);
  u8g.setCursor(12, 50);
  lcd_print(i16tostr3rj(feedrate_percentage));
  //lcd_print("100");
  u8g.print('%');

  //
  // Status line
  //
  u8g.setCursor(0, 62);

  lcd_implementation_status_message(blink);
}
