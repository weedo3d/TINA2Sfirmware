/**
* Marlin 3D Printer Firmware
* Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
*
* Based on Sprinter and grbl.
* Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/


#include "ultralcd.h"
#include "language.h"

#include "../MarlinCore.h"

#include "../module/temperature.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../module/configuration_store.h"
#include "../module/probe.h"
#include "../core/utility.h"
#include "../gcode/parser.h"
#include "../gcode/queue.h"
#include "../gcode/gcode.h"
#include "../wtlib/WTGcodeinfo.h"
#include "../wtlib/WTUtilty.h"
#include "../feature/babystep.h"
#include "../feature/pause.h"
#include "../wifi/WIFI_Manager.h"
#include "status_screen_DOGM.h"

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
#include "../libs/buzzer.h"
#endif

#include "../module/printcounter.h"

#if ENABLED(PRINTCOUNTER)
#include "../libs/duration_t.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
#include "../feature/powerloss.h"
#endif

#define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80u)

char lcd_status_message[MAX_MESSAGE_LENGTH + 1];
uint8_t lcd_status_update_delay = 1, // First update one loop delayed
lcd_status_message_level;    // Higher level blocks lower level

#if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
millis_t previous_lcd_status_ms = 0;
#endif

#if ENABLED(ULTIPANEL) && ENABLED(SCROLL_LONG_FILENAMES)
uint8_t filename_scroll_pos, filename_scroll_max, filename_scroll_hash;
#endif

uint8_t progress_bar_percent;

#include "ultralcd_impl_DOGM.h"

#include "../u8g/u8g2.h"
bool drawing_screen, first_page; // = false

// The main status screen
void lcd_status_screen();

millis_t next_lcd_update_ms;

uint8_t lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; // Set when the LCD needs to draw, decrements after every draw. Set to 2 in LCD routines so the LCD gets at least 1 full redraw (first redraw is partial)
uint16_t max_display_update_time = 0;

#if ENABLED(ULTIPANEL)

#define DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(_type, _name, _strFunc) \
inline void lcd_implementation_drawmenu_setting_edit_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...) { \
UNUSED(pstr2); \
DRAWMENU_SETTING_EDIT_GENERIC(_strFunc(*(data))); \
} \
inline void lcd_implementation_drawmenu_setting_edit_callback_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...) { \
UNUSED(pstr2); \
DRAWMENU_SETTING_EDIT_GENERIC(_strFunc(*(data))); \
} \
inline void lcd_implementation_drawmenu_setting_edit_accessor_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type (*pget)(), void (*pset)(_type), ...) { \
UNUSED(pstr2); UNUSED(pset); \
DRAWMENU_SETTING_EDIT_GENERIC(_strFunc(pget())); \
} \
typedef void _name##_void
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(int16_t, int3, i16tostr3rj);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(uint8_t, int8, ui8tostr3rj);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float3, ftostr3);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float52, ftostr52);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float43, ftostr43sign);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float5, ftostr5rj);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float51, ftostr51sign);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float52sign, ftostr52sign);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float62, ftostr62rj);
DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(uint32_t, long5, ftostr5rj);
#define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data)                    DRAW_BOOL_SETTING(sel, row, pstr, data)
#define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) DRAW_BOOL_SETTING(sel, row, pstr, data)
#define lcd_implementation_drawmenu_setting_edit_accessor_bool(sel, row, pstr, pstr2, pget, pset)     DRAW_BOOL_SETTING(sel, row, pstr, data)


bool no_reentry = false;
// constexpr int8_t menu_bottom = LCD_HEIGHT - (TALL_FONT_CORRECTION);

// Initialized by settings.load()
int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];

#if ENABLED(BABYSTEPPING)
long babysteps_done = 0;
#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
static void lcd_babystep_zoffset();
#else
static void lcd_babystep_z();
#endif
#endif

////////////////////////////////////////////
///////////////// Menu Tree ////////////////
////////////////////////////////////////////

void lcd_main_menu();
void lcd_tune_menu();
extern void lcd_prepare_menu();
extern void lcd_control_menu();

#if ENABLED(USE_WATCHDOG)
#include "watchdog.h"
#endif

uint8_t wtvar_language = 0;
uint16_t wtvar_sdcard_filecount = 0;
uint8_t wtvar_sdcard_pagecount = 0;
uint8_t wtvar_sdcard_pageid = 0;

uint16_t wtvar_sdcard_file_index[SDCARD_MENU_PAGE_ITEMS];
bool wtvar_paused;
uint8_t wtvar_save_stage = DSSP_NOTHING;
uint8_t wtvar_resume_stage = 0;
uint8_t wtvar_wizzard_mode = 0;
uint32_t _spos;
char headgcode[MAX_CMD_SIZE + 16] = {0}, str_1[16], str_2[16], flchar[10] = {0};

// extern uint8_t queue.length;
extern xyze_pos_t resume_position;

extern void wtlcd_goto_welcome1();
extern void wt_saveoff();
extern void wt_load_sdcard();
extern void wt_file_detail_menu();
extern void wtlcd_Load_led_set_menu();

bool encoderRateMultiplierEnabled;
#define ENCODER_RATE_MULTIPLY(F) (encoderRateMultiplierEnabled = F)


////////////////////////////////////////////
///////////// Global Variables /////////////
////////////////////////////////////////////


// Encoder Movement
volatile int8_t encoderDiff; // Updated in lcd_buttons_update, added to encoderPosition every LCD update
uint32_t encoderPosition;
millis_t lastEncoderMovementMillis = 0;

// Button States
bool lcd_clicked, wait_for_unclick;
volatile uint8_t buttons;
millis_t next_button_update_ms;
#if ENABLED(REPRAPWORLD_KEYPAD)
volatile uint8_t buttons_reprapworld_keypad;
#endif
#if ENABLED(LCD_HAS_SLOW_BUTTONS)
volatile uint8_t slow_buttons;
#endif

// Menu System Navigation
screenFunc_t currentScreen = lcd_status_screen;
int8_t encoderTopLine;
typedef struct {
	screenFunc_t menu_function;
	uint32_t encoder_position;
} menuPosition;
menuPosition screen_history[6];
uint8_t screen_history_depth = 0;
bool screen_changed, defer_return_to_status;

// Value Editing
const char *editLabel;
void *editValue;
int32_t minEditValue, maxEditValue;
screenFunc_t callbackFunc;
bool liveEdit;

// Manual Moves
const float manual_feedrate_mm_m[] = MANUAL_FEEDRATE;
millis_t manual_move_start_time = 0;
int8_t manual_move_axis = (int8_t)NO_AXIS;
#if EXTRUDERS > 1
int8_t manual_move_e_index = 0;
#else
#define manual_move_e_index 0
#endif

constexpr bool processing_manual_move = false;

#if PIN_EXISTS(SD_DETECT)
uint8_t lcd_sd_status;		// 0, Î´²åÈë, 1, MOTOR¿ØÖÆ, 2, WIFI¿ØÖÆ, 3, UNKNOWN
#endif

#if ENABLED(PIDTEMP)
float raw_Ki, raw_Kd; // place-holders for Ki and Kd edits
#endif

bool use_click() {
	const bool click = lcd_clicked;
	lcd_clicked = false;
	return click;
}

/**
* General function to go directly to a screen
*/
void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder/*=0*/) {
	if (currentScreen != screen) {

		currentScreen = screen;
		encoderPosition = encoder;
		if (screen == lcd_status_screen) {
			defer_return_to_status = false;
#if ENABLED(AUTO_BED_LEVELING_UBL)
			ubl.lcd_map_control = false;
#endif
			screen_history_depth = 0;
		}
		// lcd_implementation_clear();

		lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
		screen_changed = true;
		drawing_screen = false;
	}
}

/**
* Show "Moving..." till moves are done, then revert to previous display.
*/
//static const char moving[] PROGMEM = MSG_MOVING;
//static const char *sync_message = moving;

//
// Display the synchronize screen until moves are
// finished, and don't return to the caller until
// done. ** This blocks the command queue! **
//
void _lcd_synchronize() {
	if (lcdDrawUpdate) lcd_implementation_drawmenu_static(LCD_HEIGHT >= 4 ? 1 : 0, MMSG_MOVING[wtvar_language]);
	if (no_reentry) return;
	// Make this the current handler till all moves are done
	no_reentry = true;
	const screenFunc_t old_screen = currentScreen;
	lcd_goto_screen(_lcd_synchronize);
	planner.synchronize(); // idle() is called until moves complete
	no_reentry = false;
	lcd_goto_screen(old_screen);
}

// Display the synchronize screen with a custom message
// ** This blocks the command queue! **
void lcd_synchronize(const char * const msg = NULL) {
	//sync_message = msg ? msg : moving;
	_lcd_synchronize();
}

void lcd_return_to_status() { lcd_goto_screen(lcd_status_screen); }

void lcd_save_previous_screen() {
	if (screen_history_depth < COUNT(screen_history)) {
		screen_history[screen_history_depth].menu_function = currentScreen;
		screen_history[screen_history_depth].encoder_position = encoderPosition;
		++screen_history_depth;
	}
}

void lcd_goto_previous_menu() {
	if (screen_history_depth > 0) {
		--screen_history_depth;
		lcd_goto_screen(
			screen_history[screen_history_depth].menu_function,
			screen_history[screen_history_depth].encoder_position
		);
	}
	else
		lcd_return_to_status();
}

void lcd_goto_previous_menu_no_defer() {
	defer_return_to_status = false;
	lcd_goto_previous_menu();
}

/**
* Scrolling for menus and other line-based screens
*
*   encoderLine is the position based on the encoder
*   encoderTopLine is the top menu line to display
*   _lcdLineNr is the index of the LCD line (e.g., 0-3)
*   _menuLineNr is the menu item to draw and process
*   _thisItemNr is the index of each MENU_ITEM or STATIC_ITEM
*   screen_items is the total number of items in the menu (after one call)
*/
int8_t encoderLine, screen_items;
void scroll_screen(const uint8_t limit, const bool is_menu) {
	ENCODER_DIRECTION_MENUS();
	ENCODER_RATE_MULTIPLY(false);
	if (encoderPosition > 0x8000) encoderPosition = 0;
	if (first_page) {
		encoderLine = encoderPosition / (ENCODER_STEPS_PER_MENU_ITEM);
		screen_changed = false;
	}
	if (screen_items > 0 && encoderLine >= screen_items - limit) {
		encoderLine = _MAX(0, screen_items - limit);
		encoderPosition = encoderLine * (ENCODER_STEPS_PER_MENU_ITEM);
	}
	if (is_menu) {
		NOMORE(encoderTopLine, encoderLine);
		if (encoderLine >= encoderTopLine + menu_bottom)
			encoderTopLine = encoderLine - menu_bottom + 1;
	}
	else
		encoderTopLine = encoderLine;
}

#endif // ULTIPANEL

/**
*
* "Info Screen"
*
* This is very display-dependent, so the lcd implementation draws this.
*/

void lcd_status_screen() {

	ENCODER_DIRECTION_NORMAL();
	ENCODER_RATE_MULTIPLY(false);

	if (use_click()) {
		lcd_implementation_init( // to maybe revive the LCD if static electricity killed it.
		);
		lcd_goto_screen(lcd_main_menu);
		return;
	}

	feedrate_percentage = constrain(feedrate_percentage, 10, 999);

	lcd_implementation_status_screen();
}

/**
* Reset the status message
*/
void lcd_reset_status() {
	const char *msg;
	if (print_job_timer.isPaused())
		msg = MMSG_PRINT_PAUSED[wtvar_language];
	else if (card.flag.sdprinting)
		return lcd_setstatus((const char*)card.utf8_filename, true);
	else if (print_job_timer.isRunning())
		msg = MMSG_PRINTING[wtvar_language];
	else
		msg = MWELCOME_MSG[wtvar_language];

	lcd_setstatusPGM(msg, -1);
}

/**
*
* draw the kill screen
*
*/
void kill_screen(const char* lcd_msg) {
	lcd_init();
	lcd_setalertstatusPGM(lcd_msg);
	lcd_kill_screen();
}

/**
*
* Audio feedback for controller clicks
*
*/
void lcd_buzz(const long duration, const uint16_t freq) {
	buzzer.tone(duration, freq);
}

void lcd_quick_feedback(const bool clear_buttons) {

	lcd_refresh();
	if (clear_buttons) buttons = 0;
	next_button_update_ms = millis() + 500;

	// Buzz and wait. The delay is needed for buttons to settle!
	lcd_buzz(LCD_FEEDBACK_FREQUENCY_DURATION_MS, LCD_FEEDBACK_FREQUENCY_HZ);

	for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
}

#if ENABLED(ULTIPANEL)

void lcd_completion_feedback(const bool good/*=true*/) {
	if (good) {
		lcd_buzz(100, 659);
		lcd_buzz(100, 698);
	}
	else lcd_buzz(20, 440);
}

inline void line_to_current_z() {
	planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[Z_AXIS]), active_extruder);
}

/**
*
* "Main" menu
*
*/

void lcd_main_menu() 
{
	START_MENU();

	MENU_BACK_MULTI(MMSG_INFO_SCREEN[wtvar_language]);

	if (wt_onlineprinting == SPARK_IDLE || wt_onlineprinting == SPARK_LOST)
	{	// ±¾µØ´òÓ¡
		if (planner.movesplanned() || IS_SD_PRINTING())
		{
			MENU_ITEM_MULTI(submenu, MMSG_TUNE[wtvar_language], lcd_tune_menu);
		}

		if (card.flag.mounted)
		{
			if (card.isFileOpen())
			{
				if (card.flag.sdprinting)
				{
					MENU_ITEM_MULTI(function, MMSG_PAUSE_PRINT[wtvar_language], lcd_sdcard_pause);
				}
				else
				{
					MENU_ITEM_MULTI(function, MMSG_RESUME_PRINT[wtvar_language], lcd_sdcard_resume);
					MENU_ITEM_MULTI(gcode, MMSG_FILAMENTLOAD[wtvar_language], PSTR("M104 S210\r\nG28 Z\r\nM701 T0"));
					MENU_ITEM_MULTI(gcode, MMSG_FILAMENTOUT[wtvar_language], PSTR("M104 S210\r\nG28 Z\r\nM702 T0"));
				}

				MENU_ITEM_MULTI(submenu, MMSG_LED_CONTROL[wtvar_language], wtlcd_Load_led_set_menu);
				MENU_ITEM_MULTI(function, MMSG_SAVED_OFF[wtvar_language], wt_saveoff);
				MENU_ITEM_MULTI(function, MMSG_STOP_PRINT[wtvar_language], lcd_sdcard_stop);
			}
			else
			{
				MENU_ITEM_MULTI(submenu, MMSG_PREPARE[wtvar_language], lcd_prepare_menu);
				MENU_ITEM_MULTI(submenu, MMSG_CONTROL[wtvar_language], lcd_control_menu);
				MENU_ITEM_MULTI(function, MMSG_CARD_MENU[wtvar_language], wt_load_sdcard);
			}
		}
		else
		{
			MENU_ITEM_MULTI(submenu, MMSG_PREPARE[wtvar_language], lcd_prepare_menu);
			MENU_ITEM_MULTI(submenu, MMSG_CONTROL[wtvar_language], lcd_control_menu);
			MENU_ITEM_MULTI(function, MMSG_NO_CARD[wtvar_language], wt_load_sdcard);
		}

	}
	else if (wt_onlineprinting == SPARK_PRINTING)
	{	// Áª»ú´òÓ¡×´Ì¬ 
		MENU_ITEM_MULTI(function, MMSG_PAUSE_PRINT[wtvar_language], wt_online_pause);
		MENU_ITEM_MULTI(function, MMSG_STOP_PRINT[wtvar_language], wt_online_cancel);

	}
	else
	{	// Áª»úÔÝÍ£×´Ì¬
		MENU_ITEM_MULTI(function, MMSG_RESUME_PRINT[wtvar_language], wt_online_resume);
		MENU_ITEM_MULTI(submenu, MMSG_TUNE[wtvar_language], lcd_tune_menu);
		MENU_ITEM_MULTI(submenu, MMSG_LED_CONTROL[wtvar_language], wtlcd_Load_led_set_menu);
		MENU_ITEM_MULTI(gcode, MMSG_FILAMENTLOAD[wtvar_language], PSTR("M104 T0 S210\r\nM701 T0"));
		MENU_ITEM_MULTI(gcode, MMSG_FILAMENTOUT[wtvar_language], PSTR("M104 T0 S210\r\nM702 T0"));
		MENU_ITEM_MULTI(function, MMSG_STOP_PRINT[wtvar_language], wt_online_cancel);
	}
	

	END_MENU();
}

/**
*
* "Tune" submenu items
*
*/
#if ENABLED(BABYSTEPPING)

void _lcd_babystep(const AxisEnum axis, const char* msg) {
	if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
	ENCODER_DIRECTION_NORMAL();
	if (encoderPosition) {
		const int16_t babystep_increment = (int32_t)encoderPosition * (BABYSTEP_MULTIPLICATOR_Z);
		encoderPosition = 0;
		lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
		babystep.add_steps(axis, babystep_increment);
		babysteps_done += babystep_increment;
	}
	if (lcdDrawUpdate)
		lcd_implementation_drawedit(msg, ftostr43sign(planner.steps_to_mm[axis] * babysteps_done));
}

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)

void lcd_babystep_zoffset()
{
	if (use_click())
	{
		(void)settings.save();
		return lcd_goto_previous_menu_no_defer();
	}

	defer_return_to_status = true;
	ENCODER_DIRECTION_NORMAL();
	if (encoderPosition)
	{
		const int16_t babystep_increment = (int32_t)encoderPosition * (BABYSTEP_MULTIPLICATOR_Z);
		encoderPosition = 0;

		const float new_zoffset = probe.offset.z + planner.steps_to_mm[Z_AXIS] * babystep_increment;
		if (WITHIN(new_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
		{
			babystep.add_steps(Z_AXIS, babystep_increment);
			probe.offset.z = new_zoffset;
			lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
		}
	}
	if (lcdDrawUpdate)
	{
		lcd_implementation_drawedit(MMSG_ZPROBE_TITLE[wtvar_language], ftostr43sign(probe.offset.z));
#if ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY)
		_lcd_zoffset_overlay_gfx(probe.offset.z);
#endif
	}
}

#else // !BABYSTEP_ZPROBE_OFFSET

void _lcd_babystep_z() { _lcd_babystep(Z_AXIS, PSTR(MSG_BABYSTEP_Z)); }
void lcd_babystep_z() { lcd_goto_screen(_lcd_babystep_z); babysteps_done = 0; defer_return_to_status = true; }

#endif // !BABYSTEP_ZPROBE_OFFSET

#endif // BABYSTEPPING


/**
* Watch temperature callbacks
*/
#if HAS_TEMP_HOTEND
#define _WATCH_FUNC(N) thermalManager.start_watching_hotend(N)
void watch_temp_callback_E0() { _WATCH_FUNC(0); }
#endif // HAS_TEMP_HOTEND

void watch_temp_callback_bed() {
#if WATCH_THE_BED
	thermalManager.start_watching_bed();
#endif
}

// Refresh the E factor after changing flow
inline void _lcd_refresh_e_factor_0() { planner.refresh_e_factor(0); }

/**
*
* "Tune" submenu
*
*/
void lcd_tune_menu() {
	START_MENU();

	//
	// ^ Main
	//
	MENU_BACK_MULTI(MMSG_MAIN[wtvar_language]);

	//
	// Speed:
	//
	MENU_ITEM_EDIT_MULTI(int3, MMSG_SPEED[wtvar_language], &feedrate_percentage, 10, 200);


	//
	// Nozzle:
	// Nozzle [1-4]:
	//
	MENU_MULTIPLIER_ITEM_EDIT_CALLBACK_MULTI(int3, MMSG_NOZZLE[wtvar_language], &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);

	//
	// Bed:
	//
	#if HAS_HEATED_BED
		MENU_MULTIPLIER_ITEM_EDIT_CALLBACK_MULTI(int3, MMSG_BED[wtvar_language], &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
	#endif
	// Flow:
	// Flow [1-5]:
	//
#if EXTRUDERS == 1
	MENU_ITEM_EDIT_CALLBACK_MULTI(int3, MMSG_FLOW[wtvar_language], &planner.flow_percentage[0], 10, 200, _lcd_refresh_e_factor_0);
#endif // EXTRUDERS > 1

	//
	// Babystep X:
	// Babystep Y:
	// Babystep Z:
	//
#if ENABLED(BABYSTEPPING)
#if ENABLED(BABYSTEP_XY)
	MENU_ITEM(submenu, MSG_BABYSTEP_X, lcd_babystep_x);
	MENU_ITEM(submenu, MSG_BABYSTEP_Y, lcd_babystep_y);
#endif
#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
	MENU_ITEM_MULTI(submenu, MMSG_ZPROBE_ZOFFSET[wtvar_language], lcd_babystep_zoffset);
#else
	MENU_ITEM(submenu, MSG_BABYSTEP_Z, lcd_babystep_z);
#endif
#endif

	END_MENU();
}
#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(PID_AUTOTUNE_MENU) || ENABLED(ADVANCED_PAUSE_FEATURE)

/**
* If the queue is full, the command will fail, so we have to loop
* with idle() to make sure the command has been enqueued.
*/
void lcd_enqueue_command(char * const cmd) {
	no_reentry = true;
	queue.enqueue_one_now(cmd);
	no_reentry = false;
}

void lcd_enqueue_commands_P(const char * const cmd) {
	no_reentry = true;
	queue.enqueue_now_P(cmd);
	no_reentry = false;
}

#endif

float move_menu_scale;

/**
* If the most recent manual move hasn't been fed to the planner yet,
* and the planner can accept one, send immediately
*/
inline void manage_manual_move() {

	if (processing_manual_move) return;

	if (manual_move_axis != (int8_t)NO_AXIS && ELAPSED(millis(), manual_move_start_time) && !planner.is_full()) {

	planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[manual_move_axis]), manual_move_axis == E_AXIS ? manual_move_e_index : active_extruder);
	manual_move_axis = (int8_t)NO_AXIS;

	}
}

/**
* Set a flag that lcd_update() should start a move
* to "current_position" after a short delay.
*/
void manual_move_to_current(AxisEnum axis) {
	manual_move_start_time = millis() + (move_menu_scale < 0.99f ? 0UL : 250UL); // delay for bigger moves
	manual_move_axis = (int8_t)axis;
}

/**
*
* "Prepare" > "Move Axis" submenu
*
*/

void _lcd_move_xyz(const char* name, AxisEnum axis) {
	if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
	ENCODER_DIRECTION_NORMAL();
	if (encoderPosition && !processing_manual_move) {

		// Start with no limits to movement
		float min = current_position[axis] - 1000,
			max = current_position[axis] + 1000;

		// Limit to software endstops, if enabled
#if ENABLED(MIN_SOFTWARE_ENDSTOPS) || ENABLED(MAX_SOFTWARE_ENDSTOPS)
		if (soft_endstops_enabled) switch (axis) {
		case X_AXIS:
#if ENABLED(MIN_SOFTWARE_ENDSTOP_X)
			min = soft_endstop.min.x;
#endif
#if ENABLED(MAX_SOFTWARE_ENDSTOP_X)
			max = soft_endstop.max.x;
#endif
			break;
		case Y_AXIS:
#if ENABLED(MIN_SOFTWARE_ENDSTOP_Y)
			min = soft_endstop.min.y;
#endif
#if ENABLED(MAX_SOFTWARE_ENDSTOP_Y)
			max = soft_endstop.max.y;
#endif
			break;
		case Z_AXIS:
#if ENABLED(MIN_SOFTWARE_ENDSTOP_Z)
			min = soft_endstop.min.z;
#endif
#if ENABLED(MAX_SOFTWARE_ENDSTOP_Z)
			max = soft_endstop.max.z;
#endif
		default: break;
		}
#endif

		// Get the new position
		const float diff = float((int32_t)encoderPosition) * move_menu_scale;

		current_position[axis] += diff;
		if ((int32_t)encoderPosition < 0)
			NOLESS(current_position[axis], min);
		else
			NOMORE(current_position[axis], max);

		manual_move_to_current(axis);
		lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
	}
	encoderPosition = 0;
	if (lcdDrawUpdate) {
		const float pos = NATIVE_TO_LOGICAL(processing_manual_move ? destination[axis] : current_position[axis], axis);
		lcd_implementation_drawedit(name, move_menu_scale >= 0.1f ? ftostr41sign(pos) : ftostr43sign(pos));
	}
}
void lcd_move_x() { _lcd_move_xyz(MMSG_MOVE_X[wtvar_language], X_AXIS); }
void lcd_move_y() { _lcd_move_xyz(MMSG_MOVE_Y[wtvar_language], Y_AXIS); }
void lcd_move_z() { _lcd_move_xyz(MMSG_MOVE_Z[wtvar_language], Z_AXIS); }
void _lcd_move_e(
#if E_MANUAL > 1
	const int8_t eindex = -1
#endif
) {
	if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
	ENCODER_DIRECTION_NORMAL();
	if (encoderPosition) {
		if (!processing_manual_move) {
			const float diff = float((int32_t)encoderPosition) * move_menu_scale;
			current_position[E_AXIS] += diff;
			manual_move_to_current(E_AXIS
#if E_MANUAL > 1
				, eindex
#endif
			);
			lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
		}
		encoderPosition = 0;
	}
	if (lcdDrawUpdate) {
		PGM_P pos_label;
#if E_MANUAL == 1
		pos_label = MMSG_MOVE_E[wtvar_language];
#else
		switch (eindex) {
		default: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E1); break;
		case 1: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E2); break;
#if E_MANUAL > 2
		case 2: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E3); break;
#if E_MANUAL > 3
		case 3: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E4); break;
#if E_MANUAL > 4
		case 4: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E5); break;
#endif // E_MANUAL > 4
#endif // E_MANUAL > 3
#endif // E_MANUAL > 2
		}
#endif // E_MANUAL > 1
		lcd_implementation_drawedit(pos_label, ftostr41sign(current_position[E_AXIS]
		));
	}
}

void lcd_move_e() { _lcd_move_e(); }
#if E_MANUAL > 1
void lcd_move_e0() { _lcd_move_e(0); }
void lcd_move_e1() { _lcd_move_e(1); }
#if E_MANUAL > 2
void lcd_move_e2() { _lcd_move_e(2); }
#if E_MANUAL > 3
void lcd_move_e3() { _lcd_move_e(3); }
#if E_MANUAL > 4
void lcd_move_e4() { _lcd_move_e(4); }
#endif // E_MANUAL > 4
#endif // E_MANUAL > 3
#endif // E_MANUAL > 2
#endif // E_MANUAL > 1

/**
*
* "Prepare" > "Move Xmm" > "Move XYZ" submenu
*
*/

screenFunc_t _manual_move_func_ptr;

void _goto_manual_move(const float scale) {
	defer_return_to_status = true;
	move_menu_scale = scale;
	lcd_goto_screen(_manual_move_func_ptr);
}
void lcd_move_menu_10mm() { _goto_manual_move(10); }
void lcd_move_menu_1mm() { _goto_manual_move(1); }
void lcd_move_menu_01mm() { _goto_manual_move(0.1f); }

void _lcd_move_distance_menu(const AxisEnum axis, const screenFunc_t func) {
	_manual_move_func_ptr = func;
	START_MENU();
	if (LCD_HEIGHT >= 4) {
		switch (axis) {
		case X_AXIS:
			STATIC_ITEM_MULTI(MMSG_MOVE_X[wtvar_language], true, true); break;
		case Y_AXIS:
			STATIC_ITEM_MULTI(MMSG_MOVE_Y[wtvar_language], true, true); break;
		case Z_AXIS:
			STATIC_ITEM_MULTI(MMSG_MOVE_Z[wtvar_language], true, true); break;
		default:
			STATIC_ITEM_MULTI(MMSG_MOVE_E[wtvar_language], true, true); break;
		}
	}
	MENU_BACK_MULTI(MMSG_MOVE_AXIS[wtvar_language]);
	MENU_ITEM_MULTI(submenu, MMSG_MOVE_10MM[wtvar_language], lcd_move_menu_10mm);
	MENU_ITEM_MULTI(submenu, MMSG_MOVE_1MM[wtvar_language], lcd_move_menu_1mm);
	MENU_ITEM_MULTI(submenu, MMSG_MOVE_01MM[wtvar_language], lcd_move_menu_01mm);
	END_MENU();
}
void lcd_move_get_x_amount() { _lcd_move_distance_menu(X_AXIS, lcd_move_x); }
void lcd_move_get_y_amount() { _lcd_move_distance_menu(Y_AXIS, lcd_move_y); }
void lcd_move_get_z_amount() { _lcd_move_distance_menu(Z_AXIS, lcd_move_z); }
void lcd_move_get_e_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e); }
#if E_MANUAL > 1
void lcd_move_get_e0_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e0); }
void lcd_move_get_e1_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e1); }
#if E_MANUAL > 2
void lcd_move_get_e2_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e2); }
#if E_MANUAL > 3
void lcd_move_get_e3_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e3); }
#if E_MANUAL > 4
void lcd_move_get_e4_amount() { _lcd_move_distance_menu(E_AXIS, lcd_move_e4); }
#endif // E_MANUAL > 4
#endif // E_MANUAL > 3
#endif // E_MANUAL > 2
#endif // E_MANUAL > 1

/**
*
* "Prepare" > "Move Axis" submenu
*
*/

#define _MOVE_XYZ_ALLOWED true
#define _MOVE_XY_ALLOWED true

void lcd_move_menu() {
	START_MENU();
	MENU_BACK_MULTI(MMSG_PREPARE[wtvar_language]);

	if (_MOVE_XYZ_ALLOWED) {
		if (_MOVE_XY_ALLOWED) {
			MENU_ITEM_MULTI(submenu, MMSG_MOVE_X[wtvar_language], lcd_move_get_x_amount);
			MENU_ITEM_MULTI(submenu, MMSG_MOVE_Y[wtvar_language], lcd_move_get_y_amount);
		}

		MENU_ITEM_MULTI(submenu, MMSG_MOVE_Z[wtvar_language], lcd_move_get_z_amount);
	}
	else
		MENU_ITEM_MULTI(gcode, MMSG_AUTO_HOME[wtvar_language], PSTR("G28"));

	// Independent extruders with one E-stepper per hotend
	MENU_ITEM_MULTI(submenu, MMSG_MOVE_E[wtvar_language], lcd_move_get_e_amount);

	END_MENU();
}

/**
*
* "Control" > "Temperature" submenu
*
*/
void lcd_control_temperature_menu()
{
	START_MENU();

	//
	// ^ Control
	//
	MENU_BACK_MULTI(MMSG_CONTROL[wtvar_language]);

	//
	// Nozzle:
	// Nozzle [1-5]:
	//
#if HOTENDS == 1
	MENU_MULTIPLIER_ITEM_EDIT_CALLBACK_MULTI(int3, MMSG_NOZZLE[wtvar_language], &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
#endif // HOTENDS > 1

	//
	// Bed:
	//
#if HAS_HEATED_BED
	MENU_MULTIPLIER_ITEM_EDIT_CALLBACK_MULTI(int3, MMSG_BED[wtvar_language], &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
#endif

	END_MENU();
}

/**
*
* Functions for editing single values
*
* The "DEFINE_MENU_EDIT_TYPE" macro generates the functions needed to edit a numerical value.
*
* For example, DEFINE_MENU_EDIT_TYPE(int16_t, int3, i16tostr3rj, 1) expands into these functions:
*
*   bool _menu_edit_int3();
*   void menu_edit_int3(); // edit int16_t (interactively)
*   void menu_edit_callback_int3(); // edit int16_t (interactively) with callback on completion
*   void _menu_action_setting_edit_int3(const char * const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue);
*   void menu_action_setting_edit_int3(const char * const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue);
*   void menu_action_setting_edit_callback_int3(const char * const pstr, int16_t * const ptr, const int16_t minValue, const int16_t maxValue, const screenFunc_t callback, const bool live); // edit int16_t with callback
*
* You can then use one of the menu macros to present the edit interface:
*   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
*
* This expands into a more primitive menu item:
*   MENU_ITEM(setting_edit_int3, MSG_SPEED, PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
*
* ...which calls:
*       menu_action_setting_edit_int3(PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
*/
#define DEFINE_MENU_EDIT_TYPE(_type, _name, _strFunc, _scale) \
bool _menu_edit_ ## _name() { \
ENCODER_DIRECTION_NORMAL(); \
if ((int32_t)encoderPosition < 0) encoderPosition = 0; \
if ((int32_t)encoderPosition > maxEditValue) encoderPosition = maxEditValue; \
if (lcdDrawUpdate) \
lcd_implementation_drawedit(editLabel, _strFunc(((_type)((int32_t)encoderPosition + minEditValue)) * (1.0f / _scale))); \
if (lcd_clicked || (liveEdit && lcdDrawUpdate)) { \
_type value = ((_type)((int32_t)encoderPosition + minEditValue)) * (1.0f / _scale); \
if (editValue != NULL) *((_type*)editValue) = value; \
if (callbackFunc && (liveEdit || lcd_clicked)) (*callbackFunc)(); \
if (lcd_clicked) lcd_goto_previous_menu(); \
} \
return use_click(); \
} \
void menu_edit_ ## _name() { _menu_edit_ ## _name(); } \
void _menu_action_setting_edit_ ## _name(const char * const pstr, _type* const ptr, const _type minValue, const _type maxValue) { \
lcd_save_previous_screen(); \
lcd_refresh(); \
\
editLabel = pstr; \
editValue = ptr; \
minEditValue = minValue * _scale; \
maxEditValue = maxValue * _scale - minEditValue; \
encoderPosition = (*ptr) * _scale - minEditValue; \
} \
void menu_action_setting_edit_callback_ ## _name(const char * const pstr, _type * const ptr, const _type minValue, const _type maxValue, const screenFunc_t callback, const bool live) { \
_menu_action_setting_edit_ ## _name(pstr, ptr, minValue, maxValue); \
currentScreen = menu_edit_ ## _name; \
callbackFunc = callback; \
liveEdit = live; \
} \
FORCE_INLINE void menu_action_setting_edit_ ## _name(const char * const pstr, _type * const ptr, const _type minValue, const _type maxValue) { \
menu_action_setting_edit_callback_ ## _name(pstr, ptr, minValue, maxValue); \
} \
typedef void _name##_void

DEFINE_MENU_EDIT_TYPE(int16_t, int3, i16tostr3rj, 1);
DEFINE_MENU_EDIT_TYPE(uint8_t, int8, ui8tostr3rj, 1);
DEFINE_MENU_EDIT_TYPE(float, float3, ftostr3, 1);
DEFINE_MENU_EDIT_TYPE(float, float52, ftostr52, 100);
DEFINE_MENU_EDIT_TYPE(float, float43, ftostr43sign, 1000);
DEFINE_MENU_EDIT_TYPE(float, float5, ftostr5rj, 0.01f);
DEFINE_MENU_EDIT_TYPE(float, float51, ftostr51sign, 10);
DEFINE_MENU_EDIT_TYPE(float, float52sign, ftostr52sign, 100);
DEFINE_MENU_EDIT_TYPE(float, float62, ftostr62rj, 100);
DEFINE_MENU_EDIT_TYPE(uint32_t, long5, ftostr5rj, 0.01f);

/**
*
* Menu actions
*
*/
void _menu_action_back() { lcd_goto_previous_menu(); }
void menu_action_submenu(screenFunc_t func) { lcd_save_previous_screen(); lcd_goto_screen(func); }
void menu_action_gcode(const char* pgcode) { queue.enqueue_now_P(pgcode); }
void menu_action_function(screenFunc_t func) { (*func)(); }
void menu_action_nextpage(screenFunc_t func) { if (wtvar_sdcard_pageid < wtvar_sdcard_pagecount - 1) wtvar_sdcard_pageid++; (*func)(); }
void menu_action_prepage(screenFunc_t func) { if (wtvar_sdcard_pageid > 0) wtvar_sdcard_pageid--; (*func)(); }

#if ENABLED(SDSUPPORT)

void menu_action_listfile(const char* filename)
{
	// card.getfilename_sorted(encoderLine - 1 + wtvar_sdcard_pageid * SDCARD_MENU_PAGE_ITEMS);
	card.getfilename_sorted(wtvar_sdcard_file_index[encoderLine - 1]);

	gcodeinfo.parse(card.filename);
	lcd_save_previous_screen();
	lcd_goto_screen(wt_file_detail_menu);
}

#endif // SDSUPPORT

void menu_action_setting_edit_bool(const char* pstr, bool* ptr) { UNUSED(pstr); *ptr ^= true; lcd_refresh(); }
void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, screenFunc_t callback) {
	menu_action_setting_edit_bool(pstr, ptr);
	(*callback)();
}

#endif // ULTIPANEL

void lcd_init() {

	lcd_implementation_init();

#if ENABLED(NEWPANEL)
#if BUTTON_EXISTS(EN1)
	SET_INPUT_PULLUP(BTN_EN1);
#endif
#if BUTTON_EXISTS(EN2)
	SET_INPUT_PULLUP(BTN_EN2);
#endif
#if BUTTON_EXISTS(ENC)
	SET_INPUT_PULLUP(BTN_ENC);
#endif

#endif // !NEWPANEL

#if ENABLED(SDSUPPORT) //&& PIN_EXISTS(SD_DETECT)
	SET_INPUT_PULLUP(SD_DETECT_PIN);
	lcd_sd_status = 3; // UNKNOWN
#endif

	lcd_buttons_update();

#if ENABLED(ULTIPANEL)
	encoderDiff = 0;
#endif
}

int16_t utf8_strlen(const char* s) {
	int16_t j = 0;
	uint8_t* p = (uint8_t*)s;

	while (*p)
	{
		if (*p >= 0xE4)
		{	// ÓÐ¿ÉÄÜÊÇutf-8±àÂë
			p++;
			if ((*p&0xC0) == 0x80)
			{	// ÊÇutf-8±àÂë
				p+=2;
				j += 2;
			}
			else
			{
				p++;
				j += 2;
			}
		}
		else
		{
			j++;
			p++;
		}
	}
	return j;
}


bool lcd_blink() {
	static uint8_t blink = 0;
	static millis_t next_blink_ms = 0;
	millis_t ms = millis();
	if (ELAPSED(ms, next_blink_ms)) {
		blink ^= 0xFF;
		next_blink_ms = ms + 1000 - (LCD_UPDATE_INTERVAL) / 2;
	}
	return blink != 0;
}

/**
* Update the LCD, read encoder buttons, etc.
*   - Read button states
*   - Check the SD Card slot state
*   - Act on RepRap World keypad input
*   - Update the encoder position
*   - Apply acceleration to the encoder position
*   - Set lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NOW on controller events
*   - Reset the Info Screen timeout if there's any input
*   - Update status indicators, if any
*
*   Run the current LCD menu handler callback function:
*   - Call the handler only if lcdDrawUpdate != LCDVIEW_NONE
*   - Before calling the handler, LCDVIEW_CALL_NO_REDRAW => LCDVIEW_NONE
*   - Call the menu handler. Menu handlers should do the following:
*     - If a value changes, set lcdDrawUpdate to LCDVIEW_REDRAW_NOW and draw the value
*       (Encoder events automatically set lcdDrawUpdate for you.)
*     - if (lcdDrawUpdate) { redraw }
*     - Before exiting the handler set lcdDrawUpdate to:
*       - LCDVIEW_CLEAR_CALL_REDRAW to clear screen and set LCDVIEW_CALL_REDRAW_NEXT.
*       - LCDVIEW_REDRAW_NOW to draw now (including remaining stripes).
*       - LCDVIEW_CALL_REDRAW_NEXT to draw now and get LCDVIEW_REDRAW_NOW on the next loop.
*       - LCDVIEW_CALL_NO_REDRAW to draw now and get LCDVIEW_NONE on the next loop.
*     - NOTE: For graphical displays menu handlers may be called 2 or more times per loop,
*             so don't change lcdDrawUpdate without considering this.
*
*   After the menu handler callback runs (or not):
*   - Clear the LCD if lcdDrawUpdate == LCDVIEW_CLEAR_CALL_REDRAW
*   - Update lcdDrawUpdate for the next loop (i.e., move one state down, usually)
*
* No worries. This function is only called from the main thread.
*/
void lcd_update() {
	// static millis_t return_to_status_ms = 0;
	// wtvar_fps++;
	
	// Handle any queued Move Axis motion
	manage_manual_move();

	// Update button states for LCD_CLICKED, etc.
	// After state changes the next button update
	// may be delayed 300-500ms.
	lcd_buttons_update();

	// If the action button is pressed...
	if (LCD_CLICKED) {
		if (!wait_for_unclick) {           // If not waiting for a debounce release:
			wait_for_unclick = true;         //  Set debounce flag to ignore continous clicks
			lcd_clicked = !wait_for_user && !no_reentry; //  Keep the click if not waiting for a user-click
			wait_for_user = false;           //  Any click clears wait for user
			lcd_quick_feedback(true);        //  Always make a click sound
		}

		if (wtvar_click_cancel_heat && wait_for_heatup)
		{
			wtvar_click_cancel_heat = false;
			wait_for_heatup = false;
		}

		if (wtvar_click_cancel_emove && wtvar_wait_emove)
		{
			wtvar_wait_emove = false;
		}
	}
	else wait_for_unclick = false;

	const uint8_t sd_status = (uint8_t)IS_SD_INSERTED();
	if (sd_status != lcd_sd_status) {
		static uint8_t _sd_action;	// 0, unload, 1, load, 2, ignore
		if (lcd_sd_status == 0)
		{	// last sd disconnect, this time,sd connected
			_sd_action = 1;		
		}
		else if (lcd_sd_status == 1)
		{	// last sd connected, this time, sd disconnect
			_sd_action = 0;	
		}
		else if (lcd_sd_status == 2)
		{	// last sd control by esp32
			if (sd_status == 1)
			{	// do nothing
				_sd_action = 2;
			}
			else
			{	// sd disconnect
				// todo
				_sd_action = 0;
			}
		}
		else if (lcd_sd_status == 3)
		{	// last sd unknown
			_sd_action = sd_status;
		}

		uint8_t old_sd_status = lcd_sd_status; // prevent re-entry to this block!
		lcd_sd_status = sd_status;

		if (_sd_action == 1) {
			safe_delay(500); // Some boards need a delay to get settled
			wt_load_sd();
			LCD_MESSAGE(MMSG_SD_INSERTED[wtvar_language]);
			wtvar_sdcard_pageid = 0;
			wtvar_sdcard_pagecount = 0;
			wtvar_sdcard_filecount = 0;
			// send file list to esp32
			
		}
		else if (_sd_action == 0){
			wt_unload_sd();
			if (old_sd_status == 1 || old_sd_status == 2)
			{
				LCD_MESSAGE(MMSG_SD_REMOVED[wtvar_language]);
			}
		}

		lcd_refresh();
		lcd_implementation_init( // to maybe revive the LCD if static electricity killed it.
		);
	}

	const millis_t ms = millis();
	if (ELAPSED(ms, next_lcd_update_ms) || drawing_screen) {

		next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;

		const bool encoderPastThreshold = (ABS(encoderDiff) >= ENCODER_PULSES_PER_STEP);
		if (encoderPastThreshold || lcd_clicked) {
			if (encoderPastThreshold) {
				int32_t encoderMultiplier = 1;

				if (encoderRateMultiplierEnabled) {
					int32_t encoderMovementSteps = ABS(encoderDiff) / ENCODER_PULSES_PER_STEP;

					if (lastEncoderMovementMillis) {
						// Note that the rate is always calculated between two passes through the
						// loop and that the abs of the encoderDiff value is tracked.
						float encoderStepRate = float(encoderMovementSteps) / float(ms - lastEncoderMovementMillis) * 1000;

						if (encoderStepRate >= ENCODER_100X_STEPS_PER_SEC)     encoderMultiplier = 100;
						else if (encoderStepRate >= ENCODER_10X_STEPS_PER_SEC) encoderMultiplier = 10;

  					}

					lastEncoderMovementMillis = ms;
				} // encoderRateMultiplierEnabled

				encoderPosition += (encoderDiff * encoderMultiplier) / ENCODER_PULSES_PER_STEP;
				encoderDiff = 0;
			}
			// return_to_status_ms = ms + LCD_TIMEOUT_TO_STATUS;
			lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
		}

		// We arrive here every ~100ms when idling often enough.
		// Instead of tracking the changes simply redraw the Info Screen ~1 time a second.
		if (
			currentScreen == lcd_status_screen &&
			!lcd_status_update_delay--
			) {
			lcd_status_update_delay = 9 + 3;
			max_display_update_time--;
			lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
		}

		// then we want to use 1/2 of the time only.
		uint16_t bbr2 = planner.block_buffer_runtime() >> 1;

		if ((lcdDrawUpdate || drawing_screen) && (!bbr2 || bbr2 > max_display_update_time)) {

			// Change state of drawing flag between screen updates
			if (!drawing_screen) switch (lcdDrawUpdate) {
			case LCDVIEW_CALL_NO_REDRAW:
				lcdDrawUpdate = LCDVIEW_NONE;
				break;
			case LCDVIEW_CLEAR_CALL_REDRAW:
			case LCDVIEW_CALL_REDRAW_NEXT:
				lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
			case LCDVIEW_REDRAW_NOW:        // set above, or by a handler through LCDVIEW_CALL_REDRAW_NEXT
			case LCDVIEW_NONE:
				break;
			} // switch


		if (true) {
			if (!drawing_screen) {                        // If not already drawing pages
				u8g.firstPage();                            // Start the first page
				drawing_screen = first_page = true;         // Flag as drawing pages
			}
			// lcd_setFont(FONT_MENU);                       // Setup font for every page draw
			u8g.setColorIndex(1);                         // And reset the color
			currentScreen();                              // Draw and process the current screen
			first_page = false;

			// The screen handler can clear drawing_screen for an action that changes the screen.
			// If still drawing and there's another page, update max-time and return now.
			// The nextPage will already be set up on the next call.
			if (drawing_screen && (drawing_screen = u8g.nextPage())) {
				NOLESS(max_display_update_time, millis() - ms);
				return;
			}
		}

			lcd_clicked = false;
			// Keeping track of the longest time for an individual LCD update.
			// Used to do screen throttling when the planner starts to fill up.
			NOLESS(max_display_update_time, millis() - ms);
		}

		// Change state of drawing flag between screen updates
		if (!drawing_screen) switch (lcdDrawUpdate) {
		case LCDVIEW_CLEAR_CALL_REDRAW:
			// lcd_implementation_clear(); 
			break;
		case LCDVIEW_REDRAW_NOW:
			lcdDrawUpdate = LCDVIEW_NONE;
		case LCDVIEW_NONE:
		case LCDVIEW_CALL_REDRAW_NEXT:
		case LCDVIEW_CALL_NO_REDRAW:
		default: break;
		} // switch

	} // ELAPSED(ms, next_lcd_update_ms)

	if (wtvar_save_stage == DSSP_CHKSTS)
	{
		if (card.flag.sdprinting)
			wtvar_save_stage = DSSP_PAUSEDSD;
		else
			wtvar_save_stage = DSSP_HASPAUSED;

		// Head and foot will match if valid data was saved
		if (!++recovery.info.valid_head) ++recovery.info.valid_head; // non-zero in sequence
		recovery.info.valid_foot = recovery.info.valid_head;
	}
	else if (wtvar_save_stage == DSSP_PAUSEDSD)
	{
		card.pauseSDPrint();
		wtvar_save_stage = DSSP_WAITCLEARBUF;
	}
	else if (wtvar_save_stage == DSSP_WAITCLEARBUF)
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			// Machine state
			recovery.info.current_position = current_position;
            #if HAS_HOME_OFFSET
            recovery.info.home_offset = home_offset;
            #endif
			wtvar_save_stage = DSSP_GOSETPOS;
		}
	}
	else if (wtvar_save_stage == DSSP_HASPAUSED)
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			// Machine state
			recovery.info.current_position = resume_position;
			wtvar_save_stage = DSSP_GOSETPOS;
		}
	}
	else if (wtvar_save_stage == DSSP_GOSETPOS)
	{
		queue.enqueue_now_P(PSTR("G28 R0 XY"));
		wtvar_save_stage = DSSP_WAITPOS;
	}
	else if (wtvar_save_stage == DSSP_WAITPOS)
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			wtvar_save_stage = DSSP_SAVEPARA;
		}
	}
	else if (wtvar_save_stage == DSSP_SAVEPARA)
	{
        HOTEND_LOOP() recovery.info.target_temperature[e] = thermalManager.temp_hotend[e].target;

		// Elapsed print job time
		recovery.info.print_job_elapsed = print_job_timer.duration();

		// SD file position
		card.getAbsFilename(recovery.info.sd_filename);
		recovery.info.sdpos = card.getIndex();

		card.openJobRecoveryFile(false);
		(void)card.saveJobRecoveryInfo();
	
		wtvar_save_stage = DSSP_SHUTNOW;
	}
	else if (wtvar_save_stage == DSSP_SHUTNOW)
	{
		while (1)
		{
			delay(500);
#if ENABLED(USE_WATCHDOG)
		HAL_watchdog_refresh();
#endif
		} // Wait for reset
	}

	// »Ö¸´´òÓ¡
	if (wtvar_resume_stage == DSEP_PREHEAT)
	{
		_spos = recovery.info.sdpos;

		queue.enqueue_one_P(PSTR("M104 S150"));
		queue.enqueue_one_P(PSTR("M109 S100"));
		queue.enqueue_one_P(PSTR("G28 R0 X Y"));
        queue.enqueue_now_P(PSTR("M420 S1"));
		queue.enqueue_now_P(PSTR("M420 V"));

		wtvar_resume_stage = DSEP_START;
	}
	else if (wtvar_resume_stage == DSEP_START)
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
        {
            SERIAL_ECHO("heat\r\n");
            HOTEND_LOOP() 
            {
                thermalManager.setTargetHotend(recovery.info.target_temperature[e], e);
            }
            
            wtvar_resume_stage = DSEP_WAITHEATING;
        }
	}
	else if (wtvar_resume_stage == DSEP_WAITHEATING)
	{
		if ((thermalManager.degHotend(0) >= thermalManager.degTargetHotend(0) - 2) &&
			(thermalManager.degHotend(0) <= thermalManager.degTargetHotend(0) + 2) &&
			(thermalManager.degHotend(0) > 180))
		{
			destination = current_position;
            destination.e += 30 / planner.e_factor[active_extruder];
            planner.buffer_line(destination, 3, active_extruder);
            current_position = destination;

            wtvar_resume_stage = DSEP_WAITGOHOME;
		}
	}
	else if (wtvar_resume_stage == DSEP_WAITGOHOME)
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			SERIAL_ECHO("SETPOS\r\n");
			wtvar_resume_stage = DSEP_SETPOS;
		}
	}
	else if (wtvar_resume_stage == DSEP_SETPOS)
	{
		// »Ö¸´ZÆ«ÖÃ
		memset(headgcode, 0, sizeof(headgcode));
		strcat(headgcode, "G92 Z");
		// dtostrf(Z_MAX_POS + recovery.info.zoffset, 3, 3, flchar);
		dtostrf(recovery.info.current_position.z, 3, 3, flchar);
		strcat(headgcode, flchar);
		queue.enqueue_one_now(headgcode);

        // ÌáÉýZÖá
		dtostrf(recovery.info.current_position.z + 20, 1, 3, str_1);
        sprintf_P(headgcode, PSTR("G1 Z%s F500"), str_1);
		queue.enqueue_one_now(headgcode);

		// »Ö¸´XY×ø±ê
		sprintf_P(headgcode, PSTR("G1 X%s Y%s F3000"),
			dtostrf(recovery.info.current_position.x, 1, 3, str_1),
			dtostrf(recovery.info.current_position.y, 1, 3, str_2)
		);
		queue.enqueue_one_now(headgcode);

        // »Ö¸´Z×ø±ê
		dtostrf(recovery.info.current_position.z, 1, 3, str_1);
		sprintf_P(headgcode, PSTR("G1 Z%s F500"), str_1);
		queue.enqueue_one_now(headgcode);

		// »Ö¸´feedrate
		sprintf_P(headgcode, PSTR("G1 F%d"), recovery.info.feedrate);
        queue.enqueue_one_now(headgcode);

		// Restore E position with G92.9
		sprintf_P(headgcode, PSTR("G92.9 E%s"), dtostrf(recovery.info.current_position.e, 1, 3, str_1));
        queue.enqueue_one_now(headgcode);

		// Relative axis modes
		gcode.axis_relative = recovery.info.axis_relative;

		wtvar_resume_stage = DSEP_WAITBUFFER;
	}
	else if (wtvar_resume_stage == DSEP_WAITBUFFER)
	{
		if ((planner.has_blocks_queued() == false) && (queue.length == 0))
		{
			wtvar_resume_stage = DSEP_OPENFILE;
			SERIAL_ECHO("openfile\r\n");
		}
	}
	else if (wtvar_resume_stage == DSEP_OPENFILE)
	{
		char *fn = recovery.info.sd_filename;

		// Resume the SD file from the last position
		sprintf_P(headgcode, PSTR("M23 %s"), fn);
        queue.enqueue_one_now(headgcode);

        // card.openFileRead(fn);

		sprintf_P(headgcode, PSTR("M24 S%ld T%ld"), _spos, recovery.info.print_job_elapsed);
        queue.enqueue_one_now(headgcode);

		wtvar_resume_stage = DSEP_COMPLETE;
		SERIAL_ECHO("resume complete\r\n");
	}
	else if (wtvar_resume_stage == DSEP_COMPLETE)
	{
		wtvar_resume_stage = DSEP_NOTHING;
		lcd_return_to_status();
	}

}

void lcd_finishstatus(const bool persist = false) {

#if !(ENABLED(LCD_PROGRESS_BAR) && (PROGRESS_MSG_EXPIRE > 0))
	UNUSED(persist);
#endif

	lcd_refresh();

}


bool lcd_hasstatus() { return (lcd_status_message[0] != '\0'); }

void lcd_setstatus(const char * const message, const bool persist) {
	if (lcd_status_message_level > 0) return;

	// Here we have a problem. The message is encoded in UTF8, so
	// arbitrarily cutting it will be a problem. We MUST be sure
	// that there is no cutting in the middle of a multibyte character!

	// Get a pointer to the null terminator
	const char* pend = message + strlen(message);

	//  If length of supplied UTF8 string is greater than
	// our buffer size, start cutting whole UTF8 chars
	while ((pend - message) > MAX_MESSAGE_LENGTH) {
		--pend;
		while (!START_OF_UTF8_CHAR(*pend)) --pend;
	};

	// At this point, we have the proper cut point. Use it
	uint8_t maxLen = pend - message;
	strncpy(lcd_status_message, message, maxLen);
	lcd_status_message[maxLen] = '\0';

	lcd_finishstatus(persist);
}

void lcd_setstatusPGM(const char * const message, int8_t level) {
	if (level < 0) level = lcd_status_message_level = 0;
	if (level < lcd_status_message_level) return;
	lcd_status_message_level = level;

	// Here we have a problem. The message is encoded in UTF8, so
	// arbitrarily cutting it will be a problem. We MUST be sure
	// that there is no cutting in the middle of a multibyte character!

	// Get a pointer to the null terminator
	const char* pend = message + strlen_P(message);

	//  If length of supplied UTF8 string is greater than
	// our buffer size, start cutting whole UTF8 chars
	while ((pend - message) > MAX_MESSAGE_LENGTH) {
		--pend;
		while (!START_OF_UTF8_CHAR(pgm_read_byte(pend))) --pend;
	};

	// At this point, we have the proper cut point. Use it
	uint8_t maxLen = pend - message;
	strncpy_P(lcd_status_message, message, maxLen);
	lcd_status_message[maxLen] = '\0';

	lcd_finishstatus(level > 0);
}

#include <stdarg.h>
void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) {
	if (level < lcd_status_message_level) return;
	lcd_status_message_level = level;
	va_list args;
	va_start(args, fmt);
	vsnprintf_P(lcd_status_message, MAX_MESSAGE_LENGTH, fmt, args);
	va_end(args);
	lcd_finishstatus(level > 0);
}

void lcd_setalertstatusPGM(const char * const message) {
	lcd_setstatusPGM(message, 1);
#if ENABLED(ULTIPANEL)
	lcd_return_to_status();
#endif
}

void lcd_reset_alert_level() { lcd_status_message_level = 0; }

#if HAS_LCD_CONTRAST

void set_lcd_contrast(const int16_t value) {
	lcd_contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
	u8g.setContrast(lcd_contrast);
}

#endif

/**
* Setup Rotary Encoder Bit Values (for two pin encoders to indicate movement)
* These values are independent of which pins are used for EN_A and EN_B indications
* The rotary encoder part is also independent to the chipset used for the LCD
*/
#if defined(EN_A) && defined(EN_B)
#define encrot0 0
#define encrot1 2
#define encrot2 3
#define encrot3 1
#endif

#define GET_SHIFT_BUTTON_STATES(DST) \
uint8_t new_##DST = 0; \
WRITE(SHIFT_LD, LOW); \
WRITE(SHIFT_LD, HIGH); \
for (int8_t i = 0; i < 8; i++) { \
new_##DST >>= 1; \
if (READ(SHIFT_OUT)) SBI(new_##DST, 7); \
WRITE(SHIFT_CLK, HIGH); \
WRITE(SHIFT_CLK, LOW); \
} \
DST = ~new_##DST; //invert it, because a pressed switch produces a logical 0


/**
* Read encoder buttons from the hardware registers
* Warning: This function is called from interrupt context!
*/
void lcd_buttons_update() {
	static uint8_t lastEncoderBits;
	const millis_t now = millis();
	if (ELAPSED(now, next_button_update_ms)) {

#if ENABLED(NEWPANEL)
		uint8_t newbutton = 0;

#if BUTTON_EXISTS(EN1)
		if (BUTTON_PRESSED(EN1)) newbutton |= EN_A;
#endif
#if BUTTON_EXISTS(EN2)
		if (BUTTON_PRESSED(EN2)) newbutton |= EN_B;
#endif
#if BUTTON_EXISTS(ENC)
		if (BUTTON_PRESSED(ENC)) newbutton |= EN_C;
#endif
#if BUTTON_EXISTS(BACK)
		if (BUTTON_PRESSED(BACK)) newbutton |= EN_D;
#endif

		buttons = newbutton;

#else // !NEWPANEL

		GET_SHIFT_BUTTON_STATES(buttons);

#endif

	} // next_button_update_ms

	// Manage encoder rotation
#if ENABLED(REVERSE_MENU_DIRECTION) && ENABLED(REVERSE_ENCODER_DIRECTION)
#define ENCODER_DIFF_CW  (encoderDiff -= encoderDirection)
#define ENCODER_DIFF_CCW (encoderDiff += encoderDirection)
#elif ENABLED(REVERSE_MENU_DIRECTION)
#define ENCODER_DIFF_CW  (encoderDiff += encoderDirection)
#define ENCODER_DIFF_CCW (encoderDiff -= encoderDirection)
#elif ENABLED(REVERSE_ENCODER_DIRECTION)
#define ENCODER_DIFF_CW  (encoderDiff--)
#define ENCODER_DIFF_CCW (encoderDiff++)
#else
#define ENCODER_DIFF_CW  (encoderDiff++)
#define ENCODER_DIFF_CCW (encoderDiff--)
#endif
#define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: ENCODER_DIFF_CW; break; case _E2: ENCODER_DIFF_CCW; }

	uint8_t enc = 0;
	if (buttons & EN_A) enc |= B01;
	if (buttons & EN_B) enc |= B10;
	if (enc != lastEncoderBits) {
		switch (enc) {
		case encrot0: ENCODER_SPIN(encrot3, encrot1); break;
		case encrot1: ENCODER_SPIN(encrot0, encrot2); break;
		case encrot2: ENCODER_SPIN(encrot1, encrot3); break;
		case encrot3: ENCODER_SPIN(encrot2, encrot0); break;
		}
		lastEncoderBits = enc;
	}
}

void lcd_redraw_now()
{
	lcdDrawUpdate = LCDVIEW_REDRAW_NOW;	
	wait_for_user = false;
}




