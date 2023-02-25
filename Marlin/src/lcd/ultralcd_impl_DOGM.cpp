#include "ultralcd_impl_DOGM.h"

#include "../module/temperature.h"
#include "../module/planner.h"
#include "../libs/numtostr.h"
#include "../sd/cardreader.h"
#include "../u8g/u8g2.h"

int16_t lcd_contrast;                                                 // Initialized by settings.load()
U8G2_UC1701_MINI12864_F_4W_HW_SPI u8g(U8G2_R0, DOGLCD_CS, DOGLCD_A0); // 8 stripes				

uint8_t row_y1, row_y2;

void lcd_kill_screen()
{
    const uint8_t h4 = u8g.getHeight() / 4;
    u8g.firstPage();
    do
    {
        // lcd_setFont(FONT_MENU);
        u8g.setCursor(0, h4 * 1);
        lcd_print(lcd_status_message);
        u8g.setCursor(0, h4 * 2);
        lcd_print(MMSG_HALTED[wtvar_language]);
        u8g.setCursor(0, h4 * 3);
        lcd_print(MMSG_PLEASE_RESET[wtvar_language]);
    } while (u8g.nextPage());
}

void lcd_implementation_hotend_status(const uint8_t row, const uint8_t extruder = active_extruder)
{
    row_y1 = row * row_height + 1;
    row_y2 = row_y1 + row_height - 1;

    u8g.setCursor(LCD_PIXEL_WIDTH - 11 * (DOG_CHAR_WIDTH), row_y2);
    lcd_print('E');
    lcd_print((char)('1' + extruder));
    lcd_print(' ');
    lcd_print(i16tostr3rj(thermalManager.degHotend(extruder)));
    lcd_print('/');

    if (lcd_blink() || !thermalManager.hotend_idle[extruder].timed_out)
        lcd_print(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
}

void lcd_implementation_drawmenu_listfile(const bool isSelected, const uint8_t row, const char *const pstr, const char *filename)
{
    UNUSED(pstr);

    lcd_implementation_mark_as_selected(row, isSelected);

    // if (!PAGE_CONTAINS(row_y1, row_y2)) return;

    // constexpr uint8_t maxlen = LCD_WIDTH - (START_COL)-1;
    // const char *outstr = filename;

    // if (isDir) lcd_print(LCD_STR_FOLDER[0]);

    // char c;
    // uint8_t n = maxlen;
    // while (n && (c = *outstr)) {
    // 	n -= lcd_print_and_count(c);
    // 	++outstr;
    // }
    // while (n) { --n; u8g.print(' '); }
    u8g.print(filename);
}

void lcd_implementation_drawedit(const char *const pstr, const char *const value)
{
    const uint8_t labellen = utf8_strlen(pstr),
                  vallen = utf8_strlen(value);

    uint8_t rows = (labellen > LCD_WIDTH - 2 - vallen) ? 2 : 1;

    constexpr uint8_t lcd_width = LCD_WIDTH - (START_COL),
                      char_width = DOG_CHAR_WIDTH;

    // Center either one or two rows
    const uint8_t segmentHeight = u8g.getHeight() / (rows + 1); // 1 / (rows+1) = 1/2 or 1/3
    uint8_t baseline = segmentHeight + (DOG_CHAR_HEIGHT_EDIT + 1) / 2;

    u8g.setCursor(0, baseline);
    lcd_print(pstr);

    if (value != NULL)
    {
        u8g.print(':');
        if (rows == 2)
        {
            baseline += segmentHeight;
        }

        u8g.setCursor(((lcd_width - 1) - (vallen + 1)) * char_width, baseline); // Right-justified, leaving padded by spaces
        u8g.print(' ');         
        lcd_print(value);                                                // overwrite char if value gets shorter
    }
}

void lcd_custom_bootscreen()
{
    constexpr uint8_t left = (LCD_PIXEL_WIDTH - (CUSTOM_BOOTSCREEN_BMPWIDTH)) / 2,
                      top = (LCD_PIXEL_HEIGHT - (CUSTOM_BOOTSCREEN_BMPHEIGHT)) / 2;
    u8g.firstPage();
    do
    {
        u8g.drawBitmap(
            left, top,
            CEILING(CUSTOM_BOOTSCREEN_BMPWIDTH, 8), CUSTOM_BOOTSCREEN_BMPHEIGHT, custom_start_bmp);
    } while (u8g.nextPage());
    safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT);
}

void lcd_bootscreen()
{
    lcd_custom_bootscreen();
    return;
}

void lcd_print(const char *str)
{
    u8g.print(str);
}

void lcd_print(const char c)
{
    u8g.print(c);
}

char lcd_print_and_count(const char c)
{
    // if (WITHIN(c, 1, LCD_STR_SPECIAL_MAX))
    // {
    //     // u8g.setFont(FONT_SPECIAL_NAME);
    //     u8g.print(c);
    //     // lcd_setFont(currentfont);
    //     return 1;
    // }
    // else
    // {
    //   return charset_mapper(c);
    u8g.print(c);
    return 1;
    // }
}

void lcd_implementation_init()
{

#if PIN_EXISTS(LCD_RESET)
    OUT_WRITE(LCD_RESET_PIN, LOW); // perform a clean hardware reset
    _delay_ms(5);
    OUT_WRITE(LCD_RESET_PIN, HIGH);
    _delay_ms(5); // delay to allow the display to initalize
#endif

#if PIN_EXISTS(LCD_RESET)
    u8g.begin();
    u8g.enableUTF8Print();
    u8g.setFont(u8g2_font_weedo3d_gb2312);
#endif

#if DISABLED(MINIPANEL) // setContrast not working for Mini Panel
    u8g.setContrast(lcd_contrast);
#endif

}

void wt_single_line_message(const uint8_t row, const char *str)
{
    row_y1 = row * row_height + 1;
    row_y2 = row_y1 + row_height - 1;

    u8g.setCursor(1, row_y2);
    lcd_print(str);
}

void _drawmenu_setting_edit_generic(const bool isSelected, const uint8_t row, const char *pstr, const char *const data, const bool pgm)
{

    lcd_implementation_mark_as_selected(row, isSelected);

    const uint8_t vallen = (pgm ? utf8_strlen(data) : utf8_strlen((char *)data));
    uint8_t n = LCD_WIDTH - (START_COL)-2 - vallen;

    while (char c = pgm_read_byte(pstr))
    {
        n -= lcd_print_and_count(c);
        pstr++;
    }
    u8g.print(':');
    while (n--)
        u8g.print(' ');
    u8g.setCursor(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH)*vallen, row_y2);
    lcd_print((char *)data);
}

void lcd_implementation_drawmenu_var(const bool isSelected, const uint8_t row, const char *pstr, const char *vstr, const char pre_char, const char post_char)
{
    UNUSED(pre_char);

    lcd_implementation_mark_as_selected(row, isSelected);

    uint8_t n = LCD_WIDTH - (START_COL)-2;

    while (char c = pgm_read_byte(pstr))
    {
        n -= lcd_print_and_count(c);
        pstr++;
    }
    lcd_print_and_count(' ');
    while (char c = *vstr)
    {
        n -= lcd_print_and_count(c);
        vstr++;
    }
    while (n--)
        u8g.print(' ');
    u8g.setCursor(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH), row_y2);
    lcd_print(post_char);
    u8g.print(' ');
}

void lcd_implementation_drawmenu_static(const uint8_t row, const char *pstr, const bool center, const bool invert, const char *valstr)
{

    lcd_implementation_mark_as_selected(row, invert);

    int8_t n = LCD_WIDTH - (START_COL);

    if (center && !valstr)
    {
        int8_t pad = (LCD_WIDTH - utf8_strlen(pstr)) / 2;
        while (--pad >= 0)
        {
            u8g.print(' ');
            n--;
        }
    }
    u8g.print(pstr);
    n -=utf8_strlen(pstr);
    if (valstr)
    {
        u8g.print(valstr);
        n -=utf8_strlen(valstr);
    }
    while (n-- > 0)
        u8g.print(' ');
}

// Draw a generic menu item
void lcd_implementation_drawmenu_generic(const bool isSelected, const uint8_t row, const char *pstr, const char *pre_char, const char *post_char)
{
    UNUSED(pre_char);

    lcd_implementation_mark_as_selected(row, isSelected);

    u8g.print(pstr);
    u8g.setCursor(LCD_PIXEL_WIDTH - 10, row_y2);
    lcd_print(post_char);
    u8g.print(' ');
}

void lcd_implementation_mark_as_selected(const uint8_t row, const bool isSelected)
{
    row_y1 = row * row_height + 1;
    row_y2 = row_y1 + row_height - 1;

    if (isSelected)
    {
        u8g.setColorIndex(1);                                    // black on white
                                                                 //u8g.drawBox(0, row_y1 + 2, LCD_PIXEL_WIDTH, row_height - 1);
        u8g.drawBox(0, row_y1 + 1, LCD_PIXEL_WIDTH, row_height);
        u8g.setColorIndex(0);                                    // white on black
    }
    else
    {
        u8g.setColorIndex(1); // unmarked text is black on white
    }

    u8g.setCursor((START_COL) * (DOG_CHAR_WIDTH), row_y2);
}