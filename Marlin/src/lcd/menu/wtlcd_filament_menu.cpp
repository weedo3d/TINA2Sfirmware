/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)


static PauseMode advanced_pause_mode = PAUSE_MODE_PAUSE_PRINT;
static uint8_t hotend_status_extruder = 0;

static const char* advanced_pause_header() {
	switch (advanced_pause_mode) {
	case PAUSE_MODE_LOAD_FILAMENT:
		return MMSG_FILAMENTLOAD[wtvar_language];
	case PAUSE_MODE_UNLOAD_FILAMENT:
		return MMSG_FILAMENTOUT[wtvar_language];
	default: break;
	}
	return MMSG_FILAMENT_CHANGE_HEADER_PAUSE[wtvar_language];
}

// Portions from STATIC_ITEM...
#define HOTEND_STATUS_ITEM() do { \
if (_menuLineNr == _thisItemNr) { \
if (lcdDrawUpdate) { \
lcd_implementation_drawmenu_static(_lcdLineNr, MMSG_FILAMENT_CHANGE_NOZZLE[wtvar_language], false, true); \
lcd_implementation_hotend_status(_lcdLineNr, hotend_status_extruder); \
} \
if (_skipStatic && encoderLine <= _thisItemNr) { \
encoderPosition += ENCODER_STEPS_PER_MENU_ITEM; \
++encoderLine; \
} \
lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT; \
} \
++_thisItemNr; \
}while(0)

void lcd_advanced_pause_resume_print() {
	pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
}

void lcd_advanced_pause_extrude_more() {
	pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
}

void lcd_advanced_pause_option_menu() {
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_OPTION_HEADER[wtvar_language], true, false);
	MENU_ITEM_MULTI(function, MMSG_FILAMENT_CHANGE_OPTION_RESUME[wtvar_language], lcd_advanced_pause_resume_print);
	MENU_ITEM_MULTI(function, MMSG_FILAMENT_CHANGE_OPTION_PURGE[wtvar_language], lcd_advanced_pause_extrude_more);
	END_MENU();
}

void lcd_advanced_pause_init_message() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_INIT_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_INIT_2[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_INIT_3[wtvar_language]);
	//STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

void lcd_advanced_pause_unload_message() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_UNLOAD_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_UNLOAD_2[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_UNLOAD_3[wtvar_language]);
	//STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

// 等待预热喷嘴
void lcd_advanced_pause_wait_for_nozzles_to_heat() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_HEATING_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_HEATING_2[wtvar_language]);
	STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

// 按下按键开始预热喷嘴
void lcd_advanced_pause_heat_nozzle() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_HEAT_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_HEAT_2[wtvar_language]);
	STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

// 等待插入料丝
void lcd_advanced_pause_insert_message() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_INSERT_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_INSERT_2[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_INSERT_3[wtvar_language]);
	//STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

// 等待料丝载入
void lcd_advanced_pause_load_message() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_LOAD_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_LOAD_2[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_LOAD_3[wtvar_language]);
	//STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

void lcd_advanced_pause_purge_message() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_PURGE_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_PURGE_2[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_PURGE_3[wtvar_language]);
	//STATIC_ITEM(" ");
	HOTEND_STATUS_ITEM();
	END_SCREEN();
}

void lcd_advanced_pause_resume_message() {
	START_SCREEN();
	STATIC_ITEM_P(advanced_pause_header(), true, true);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_RESUME_1[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_RESUME_2[wtvar_language]);
	STATIC_ITEM_MULTI(MMSG_FILAMENT_CHANGE_RESUME_3[wtvar_language]);
	END_SCREEN();
}

FORCE_INLINE screenFunc_t ap_message_screen(const PauseMessage message) {
	switch (message) {
	case PAUSE_MESSAGE_WAITING: return lcd_advanced_pause_init_message;
	case PAUSE_MESSAGE_UNLOAD: return lcd_advanced_pause_unload_message;
	case PAUSE_MESSAGE_INSERT: return lcd_advanced_pause_insert_message;
	case PAUSE_MESSAGE_LOAD: return lcd_advanced_pause_load_message;
	case PAUSE_MESSAGE_PURGE: return lcd_advanced_pause_purge_message;
	case PAUSE_MESSAGE_RESUME: return lcd_advanced_pause_resume_message;
	case PAUSE_MESSAGE_HEAT: return lcd_advanced_pause_heat_nozzle;
	case PAUSE_MESSAGE_HEATING: return lcd_advanced_pause_wait_for_nozzles_to_heat;
	case PAUSE_MESSAGE_OPTION: pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
		return lcd_advanced_pause_option_menu;
#if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
	case ADVANCED_PAUSE_MESSAGE_CONTINUOUS_PURGE: return lcd_advanced_pause_continuous_purge_menu;
#endif
	case PAUSE_MESSAGE_STATUS:
	default: break;
	}
	return NULL;
}

void lcd_advanced_pause_show_message(
	const PauseMessage message,
	const PauseMode mode/*=PAUSE_MODE_PAUSE_PRINT*/,
	const uint8_t extruder/*=active_extruder*/
) {
	advanced_pause_mode = mode;
	hotend_status_extruder = extruder;
	const screenFunc_t next_screen = ap_message_screen(message);
	if (next_screen) {
		defer_return_to_status = true;
		lcd_goto_screen(next_screen);
	}
	else
		lcd_return_to_status();
}

#endif // ADVANCED_PAUSE_FEATURE