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

#ifndef ULTRALCD_H
#define ULTRALCD_H

#include "../inc/MarlinConfig.h"
#include "../sd/cardreader.h"
#include "ultralcd_impl_DOGM.h"

#define MAX_MESSAGE_LENGTH 30

#define SDCARD_MENU_PAGE_ITEMS	6

// Function pointer to menu functions.
typedef void (*screenFunc_t)();
typedef void (*menuAction_t)();

////////////////////////////////////////////
//////////// Menu System Actions ///////////
////////////////////////////////////////////

#define menu_action_back(dummy) _menu_action_back()
void _menu_action_back();
void menu_action_submenu(screenFunc_t data);
void menu_action_gcode(const char* pgcode);
void menu_action_function(menuAction_t data);

#define DECLARE_MENU_EDIT_TYPE(_type, _name) \
bool _menu_edit_ ## _name(); \
void menu_edit_ ## _name(); \
void menu_edit_callback_ ## _name(); \
void _menu_action_setting_edit_ ## _name(const char * const pstr, _type* const ptr, const _type minValue, const _type maxValue); \
void menu_action_setting_edit_ ## _name(const char * const pstr, _type * const ptr, const _type minValue, const _type maxValue); \
void menu_action_setting_edit_callback_ ## _name(const char * const pstr, _type * const ptr, const _type minValue, const _type maxValue, const screenFunc_t callback=NULL, const bool live=false); \
typedef void _name##_void

DECLARE_MENU_EDIT_TYPE(int16_t, int3);
DECLARE_MENU_EDIT_TYPE(uint8_t, int8);
DECLARE_MENU_EDIT_TYPE(float, float3);
DECLARE_MENU_EDIT_TYPE(float, float52);
DECLARE_MENU_EDIT_TYPE(float, float43);
DECLARE_MENU_EDIT_TYPE(float, float5);
DECLARE_MENU_EDIT_TYPE(float, float51);
DECLARE_MENU_EDIT_TYPE(float, float52sign);
DECLARE_MENU_EDIT_TYPE(float, float62);
DECLARE_MENU_EDIT_TYPE(uint32_t, long5);

void menu_action_setting_edit_bool(const char* pstr, bool* ptr);
void menu_action_setting_edit_callback_bool(const char* pstr, bool* ptr, screenFunc_t callbackFunc);

#if ENABLED(SDSUPPORT)
void menu_action_sdfile(CardReader& theCard);
void menu_action_listfile(const char* filename);
void menu_action_nextpage(screenFunc_t func);
void menu_action_prepage(screenFunc_t func);
#endif

extern bool encoderRateMultiplierEnabled;
extern millis_t lastEncoderMovementMillis;

////////////////////////////////////////////
//////////// Menu System Macros ////////////
////////////////////////////////////////////
/**
* MENU_ITEM generates draw & handler code for a menu item, potentially calling:
*
*   lcd_implementation_drawmenu_[type](sel, row, label, arg3...)
*   menu_action_[type](arg3...)
*
* Examples:
*   MENU_ITEM(back, MSG_WATCH, 0 [dummy parameter] )
*   or
*   MENU_BACK(MSG_WATCH)
*     lcd_implementation_drawmenu_back(sel, row, PSTR(MSG_WATCH))
*     menu_action_back()
*
*   MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause)
*     lcd_implementation_drawmenu_function(sel, row, PSTR(MSG_PAUSE_PRINT), lcd_sdcard_pause)
*     menu_action_function(lcd_sdcard_pause)
*
*   MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999)
*   MENU_ITEM(setting_edit_int3, MSG_SPEED, PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
*     lcd_implementation_drawmenu_setting_edit_int3(sel, row, PSTR(MSG_SPEED), PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
*     menu_action_setting_edit_int3(PSTR(MSG_SPEED), &feedrate_percentage, 10, 999)
*
*/
extern bool lcd_clicked, wait_for_unclick;
extern bool screen_changed, defer_return_to_status;
#define _MENU_ITEM_PART_1(TYPE, ...) \
if (_menuLineNr == _thisItemNr) { \
if (encoderLine == _thisItemNr && lcd_clicked) { \
lcd_clicked = false

#define _MENU_ITEM_PART_2(TYPE, PLABEL, ...) \
menu_action_ ## TYPE(__VA_ARGS__); \
if (screen_changed) return; \
} \
if (lcdDrawUpdate) \
lcd_implementation_drawmenu_ ## TYPE(encoderLine == _thisItemNr, _lcdLineNr, PLABEL, ## __VA_ARGS__); \
} \
++_thisItemNr

#define MENU_ITEM_P(TYPE, PLABEL, ...) do { \
_skipStatic = false; \
_MENU_ITEM_PART_1(TYPE, ## __VA_ARGS__); \
_MENU_ITEM_PART_2(TYPE, PLABEL, ## __VA_ARGS__); \
}while(0)

#define MENU_ITEM(TYPE, LABEL, ...) MENU_ITEM_P(TYPE, PSTR(LABEL), ## __VA_ARGS__)

#define _MENU_ITEM_PART_2_v(TYPE, PLABEL, VLABLE, ...) \
menu_action_ ## TYPE(__VA_ARGS__); \
if (screen_changed) return; \
} \
if (lcdDrawUpdate) \
lcd_implementation_drawmenu_v ## TYPE(encoderLine == _thisItemNr, _lcdLineNr, PLABEL, VLABLE, ## __VA_ARGS__); \
} \
++_thisItemNr

#define MENU_ITEM_v(TYPE, PLABEL, VLABLE, ...) do { \
_skipStatic = false; \
_MENU_ITEM_PART_1(TYPE, ## __VA_ARGS__); \
_MENU_ITEM_PART_2_v(TYPE, PLABEL, VLABLE, ## __VA_ARGS__); \
}while(0)

#define MENU_ITEM_V(TYPE, LABEL, VLABEL, ...) MENU_ITEM_v(TYPE, PSTR(LABEL), VLABEL, ## __VA_ARGS__)

#define MENU_ITEM_V_MULTI(TYPE, LABEL, VLABEL, ...) MENU_ITEM_v(TYPE, LABEL, VLABEL, ## __VA_ARGS__)

#define MENU_BACK(LABEL) MENU_ITEM(back, LABEL, 0)

#define MENU_ITEM_MULTI(TYPE, LABEL, ...) MENU_ITEM_P(TYPE, LABEL, ## __VA_ARGS__)
#define MENU_BACK_MULTI(LABEL) MENU_ITEM_MULTI(back, LABEL, 0)

// Used to print static text with no visible cursor.
// Parameters: label [, bool center [, bool invert [, char *value] ] ]
extern int8_t encoderLine, screen_items;
extern uint32_t encoderPosition;
#define STATIC_ITEM_P(LABEL, ...) \
if (_menuLineNr == _thisItemNr) { \
if (_skipStatic && encoderLine <= _thisItemNr) { \
encoderPosition += ENCODER_STEPS_PER_MENU_ITEM; \
++encoderLine; \
} \
if (lcdDrawUpdate) \
lcd_implementation_drawmenu_static(_lcdLineNr, LABEL, ## __VA_ARGS__); \
} \
++_thisItemNr

#define STATIC_ITEM(LABEL, ...) STATIC_ITEM_P(PSTR(LABEL), ## __VA_ARGS__)

#define STATIC_ITEM_MULTI(LABEL, ...) STATIC_ITEM_P(LABEL, ## __VA_ARGS__)


/**
* MENU_MULTIPLIER_ITEM generates drawing and handling code for a multiplier menu item
*/
#define MENU_MULTIPLIER_ITEM(TYPE, LABEL, ...) do { \
_MENU_ITEM_PART_1(TYPE, ## __VA_ARGS__); \
encoderRateMultiplierEnabled = true; \
lastEncoderMovementMillis = 0; \
_MENU_ITEM_PART_2(TYPE, PSTR(LABEL), ## __VA_ARGS__); \
}while(0)


#define MENU_ITEM_DUMMY() do { _thisItemNr++; }while(0)
#define MENU_ITEM_EDIT(TYPE, LABEL, ...) MENU_ITEM(setting_edit_ ## TYPE, LABEL, PSTR(LABEL), ## __VA_ARGS__)
#define MENU_ITEM_EDIT_CALLBACK(TYPE, LABEL, ...) MENU_ITEM(setting_edit_callback_ ## TYPE, LABEL, PSTR(LABEL), ## __VA_ARGS__)

#define MENU_MULTIPLIER_ITEM_EDIT(TYPE, LABEL, ...) MENU_MULTIPLIER_ITEM(setting_edit_ ## TYPE, LABEL, PSTR(LABEL), ## __VA_ARGS__)
#define MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(TYPE, LABEL, ...) MENU_MULTIPLIER_ITEM(setting_edit_callback_ ## TYPE, LABEL, PSTR(LABEL), ## __VA_ARGS__)

#define MENU_MULTIPLIER_ITEM_MULTI(TYPE, LABEL, ...) do { \
_MENU_ITEM_PART_1(TYPE, ## __VA_ARGS__); \
encoderRateMultiplierEnabled = true; \
lastEncoderMovementMillis = 0; \
_MENU_ITEM_PART_2(TYPE, LABEL, ## __VA_ARGS__); \
}while(0)
#define MENU_ITEM_EDIT_CALLBACK_MULTI(TYPE, LABEL, ...) MENU_ITEM_MULTI(setting_edit_callback_ ## TYPE, LABEL, LABEL, ## __VA_ARGS__)
#define MENU_MULTIPLIER_ITEM_EDIT_MULTI(TYPE, LABEL, ...) MENU_MULTIPLIER_ITEM_MULTI(setting_edit_ ## TYPE, LABEL, LABEL, ## __VA_ARGS__)
#define MENU_MULTIPLIER_ITEM_EDIT_CALLBACK_MULTI(TYPE, LABEL, ...) MENU_MULTIPLIER_ITEM_MULTI(setting_edit_callback_ ## TYPE, LABEL, LABEL, ## __VA_ARGS__)
#define MENU_ITEM_EDIT_MULTI(TYPE, LABEL, ...) MENU_ITEM_MULTI(setting_edit_ ## TYPE, LABEL, LABEL, ## __VA_ARGS__)

extern int8_t encoderTopLine;
constexpr int8_t menu_bottom = LCD_HEIGHT;
#define SCREEN_OR_MENU_LOOP() \
int8_t _menuLineNr = encoderTopLine, _thisItemNr; \
for (int8_t _lcdLineNr = 0; _lcdLineNr < menu_bottom; _lcdLineNr++, _menuLineNr++) { \
_thisItemNr = 0

/**
* START_SCREEN  Opening code for a screen having only static items.
*               Do simplified scrolling of the entire screen.
*
* START_MENU    Opening code for a screen with menu items.
*               Scroll as-needed to keep the selected line in view.
*/
void scroll_screen(const uint8_t limit, const bool is_menu);
#define START_SCREEN() \
scroll_screen(menu_bottom, false); \
bool _skipStatic = false; \
SCREEN_OR_MENU_LOOP()

#define START_MENU() \
scroll_screen(1, true); \
bool _skipStatic = true; \
SCREEN_OR_MENU_LOOP()

#define END_SCREEN() \
} \
screen_items = _thisItemNr

#define END_MENU() \
} \
screen_items = _thisItemNr; \
UNUSED(_skipStatic)

/****************************/

void lcd_init();
void lcd_update();
void lcd_setalertstatusPGM(const char* message);

// #include "Marlin.h"
#include "../MarlinCore.h"
#include "../feature/pause.h"

int16_t utf8_strlen(const char* s);
bool lcd_hasstatus();
void lcd_setstatus(const char* message, const bool persist=false);
void lcd_setstatusPGM(const char* message, const int8_t level=0);
void lcd_setalertstatusPGM(const char* message);
void lcd_reset_alert_level();
void lcd_reset_status();
void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...);
void lcd_kill_screen();
void kill_screen(const char* lcd_msg);

extern uint8_t lcdDrawUpdate;
inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

#if HAS_BUZZER
  void lcd_buzz(const long duration, const uint16_t freq);
#endif

void lcd_quick_feedback(const bool clear_buttons); // Audible feedback for a button click - could also be visual

#if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
  void dontExpireStatus();
#endif

extern uint8_t progress_bar_percent;

#if ENABLED(ADC_KEYPAD)
  uint8_t get_ADC_keyValue();
#endif

#if ENABLED(DOGLCD)
  extern int16_t lcd_contrast;
  void set_lcd_contrast(const int16_t value);
#endif

#if ENABLED(SHOW_BOOTSCREEN)
  void lcd_bootscreen();
#endif

#define LCD_UPDATE_INTERVAL 100
#define BUTTON_EXISTS(BN) (defined(BTN_## BN) && BTN_## BN >= 0)
#define BUTTON_PRESSED(BN) !READ(BTN_## BN)

extern bool defer_return_to_status;



extern int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
  extern bool lcd_external_control;
#else
  constexpr bool lcd_external_control = false;
#endif

#if ENABLED(LCD_BED_LEVELING)
  extern bool lcd_wait_for_move;
#else
  constexpr bool lcd_wait_for_move = false;
#endif

void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder=0);

void lcd_completion_feedback(const bool good=true);

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  extern uint8_t active_extruder;
  void lcd_advanced_pause_show_message(const PauseMessage message,
                                        const PauseMode mode=PAUSE_MODE_PAUSE_PRINT,
                                        const uint8_t extruder=active_extruder);
#endif

#if ENABLED(G26_MESH_VALIDATION)
  void lcd_chirp();
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  void lcd_mesh_edit_setup(const float &initial);
  float lcd_mesh_edit();
  void lcd_z_offset_edit_setup(const float &initial);
  float lcd_z_offset_edit();
#endif

#if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
  extern millis_t previous_lcd_status_ms;
#endif

bool lcd_blink();

#if ENABLED(REPRAPWORLD_KEYPAD) // is also ULTIPANEL and NEWPANEL

  #define REPRAPWORLD_BTN_OFFSET 0 // bit offset into buttons for shift register values

  #define BLEN_REPRAPWORLD_KEYPAD_F3     0
  #define BLEN_REPRAPWORLD_KEYPAD_F2     1
  #define BLEN_REPRAPWORLD_KEYPAD_F1     2
  #define BLEN_REPRAPWORLD_KEYPAD_DOWN   3
  #define BLEN_REPRAPWORLD_KEYPAD_RIGHT  4
  #define BLEN_REPRAPWORLD_KEYPAD_MIDDLE 5
  #define BLEN_REPRAPWORLD_KEYPAD_UP     6
  #define BLEN_REPRAPWORLD_KEYPAD_LEFT   7

  #define EN_REPRAPWORLD_KEYPAD_F3      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F3))
  #define EN_REPRAPWORLD_KEYPAD_F2      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F2))
  #define EN_REPRAPWORLD_KEYPAD_F1      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_F1))
  #define EN_REPRAPWORLD_KEYPAD_DOWN    (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_DOWN))
  #define EN_REPRAPWORLD_KEYPAD_RIGHT   (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_RIGHT))
  #define EN_REPRAPWORLD_KEYPAD_MIDDLE  (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_MIDDLE))
  #define EN_REPRAPWORLD_KEYPAD_UP      (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_UP))
  #define EN_REPRAPWORLD_KEYPAD_LEFT    (_BV(REPRAPWORLD_BTN_OFFSET + BLEN_REPRAPWORLD_KEYPAD_LEFT))

  #define REPRAPWORLD_KEYPAD_MOVE_Z_DOWN  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_F3)
  #define REPRAPWORLD_KEYPAD_MOVE_Z_UP    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_F2)
  #define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_DOWN)
  #define REPRAPWORLD_KEYPAD_MOVE_X_RIGHT (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_RIGHT)
  #define REPRAPWORLD_KEYPAD_MOVE_Y_UP    (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_UP)
  #define REPRAPWORLD_KEYPAD_MOVE_X_LEFT  (buttons_reprapworld_keypad & EN_REPRAPWORLD_KEYPAD_LEFT)

  #if ENABLED(ADC_KEYPAD)
    #define KEYPAD_HOME EN_REPRAPWORLD_KEYPAD_F1
    #define KEYPAD_EN_C EN_REPRAPWORLD_KEYPAD_MIDDLE
  #else
    #define KEYPAD_HOME EN_REPRAPWORLD_KEYPAD_MIDDLE
    #define KEYPAD_EN_C EN_REPRAPWORLD_KEYPAD_F1
  #endif
  #define REPRAPWORLD_KEYPAD_MOVE_HOME    (buttons_reprapworld_keypad & KEYPAD_HOME)
  #define REPRAPWORLD_KEYPAD_MOVE_MENU    (buttons_reprapworld_keypad & KEYPAD_EN_C)

  #define REPRAPWORLD_KEYPAD_PRESSED      (buttons_reprapworld_keypad & ( \
                                            EN_REPRAPWORLD_KEYPAD_F3 | \
                                            EN_REPRAPWORLD_KEYPAD_F2 | \
                                            EN_REPRAPWORLD_KEYPAD_F1 | \
                                            EN_REPRAPWORLD_KEYPAD_DOWN | \
                                            EN_REPRAPWORLD_KEYPAD_RIGHT | \
                                            EN_REPRAPWORLD_KEYPAD_MIDDLE | \
                                            EN_REPRAPWORLD_KEYPAD_UP | \
                                            EN_REPRAPWORLD_KEYPAD_LEFT) \
                                          )

#endif

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
  bool is_lcd_clicked();
  void wait_for_release();
#endif


#if ENABLED(NEWPANEL) // Uses digital switches, not a shift register

  // Wheel spin pins where BA is 00, 10, 11, 01 (1 bit always changes)
  #define BLEN_A 0
  #define BLEN_B 1

  #define EN_A _BV(BLEN_A)
  #define EN_B _BV(BLEN_B)

  #if BUTTON_EXISTS(ENC)
    #define BLEN_C 2
    #define EN_C _BV(BLEN_C)
  #endif

  #if BUTTON_EXISTS(BACK)
    #define BLEN_D 3
    #define EN_D _BV(BLEN_D)
    #define LCD_BACK_CLICKED (buttons & EN_D)
  #endif

#endif // NEWPANEL

extern volatile uint8_t buttons;  // The last-checked buttons in a bit array.
void lcd_buttons_update();


#if ENABLED(REPRAPWORLD_KEYPAD)
  #ifdef EN_C
    #define LCD_CLICKED ((buttons & EN_C) || REPRAPWORLD_KEYPAD_MOVE_MENU)
  #else
    #define LCD_CLICKED REPRAPWORLD_KEYPAD_MOVE_MENU
  #endif
#elif defined(EN_C)
  #define LCD_CLICKED (buttons & EN_C)
#else
  #define LCD_CLICKED false
#endif

#define LCD_MESSAGE(x)			lcd_setstatusPGM(x)		
#define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

#if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
  void lcd_reselect_last_file();
#endif

#if ENABLED(ULTIPANEL) && ENABLED(SDSUPPORT)
  extern bool abort_sd_printing;
#else
  constexpr bool abort_sd_printing = false;
#endif

/**
* REVERSE_MENU_DIRECTION
*
* To reverse the menu direction we need a general way to reverse
* the direction of the encoder everywhere. So encoderDirection is
* added to allow the encoder to go the other way.
*
* This behavior is limited to scrolling Menus and SD card listings,
* and is disabled in other contexts.
*/
#if ENABLED(REVERSE_MENU_DIRECTION)
int8_t encoderDirection = 1;
#define ENCODER_DIRECTION_NORMAL() (encoderDirection = 1)
#define ENCODER_DIRECTION_MENUS() (encoderDirection = -1)
#else
#define ENCODER_DIRECTION_NORMAL() ;
#define ENCODER_DIRECTION_MENUS() ;
#endif

void lcd_sdcard_stop();
void lcd_sdcard_resume();
void lcd_sdcard_pause();
void lcd_return_to_status();
void lcd_goto_previous_menu();
void lcd_redraw_now();
bool use_click();
void _lcd_synchronize();
void manual_move_to_current(AxisEnum axis);
void lcd_goto_previous_menu_no_defer();

enum DSSP_ENUM
{
	DSSP_NOTHING = 0,
	DSSP_CHKSTS = 1,
	DSSP_PAUSEDSD = 2,
	DSSP_WAITCLEARBUF = 3,
	DSSP_GOSETPOS = 4,
	DSSP_WAITPOS = 5,
	DSSP_SAVEPARA = 6,
	DSSP_SHUTNOW = 7,
	DSSP_HASPAUSED = 8
};

enum DSEP_ENUM
{
	DSEP_NOTHING = 0,
	DSEP_FOUNDFILE = 1,
	DSEP_WAITUSER = 2,
	DSEP_START = 3,
	DSEP_HEATING = 4,
	DSEP_WAITHEATING = 5,
	DSEP_GOHOME = 6,
	DSEP_WAITGOHOME = 7,
	DSEP_SETPOS = 8,
	DSEP_WAITSETPOS = 9,
	DSEP_WAITBUFFER = 10,
	DSEP_OPENFILE = 11,
	DSEP_COMPLETE = 12,
	DSEP_PREHEAT = 13,
	DSEP_WAITPREHEAT = 14
};

extern char lcd_status_message[MAX_MESSAGE_LENGTH + 1];
extern uint8_t wtvar_wizzard_mode;
extern bool drawing_screen;
extern float move_menu_scale;

#if HAS_TEMP_HOTEND
void watch_temp_callback_E0();
#endif // HAS_TEMP_HOTEND

extern uint16_t wtvar_sdcard_filecount;
extern uint8_t wtvar_sdcard_pageid;
extern uint8_t wtvar_sdcard_pagecount;
extern uint16_t wtvar_sdcard_file_index[SDCARD_MENU_PAGE_ITEMS];
extern uint8_t wtvar_save_stage;
extern uint8_t wtvar_resume_stage;

#endif // ULTRALCD_H
