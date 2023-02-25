/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"
#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../module/configuration_store.h"
#include "../../libs/numtostr.h"
#include "../../module/temperature.h"
#include "../../wifi/WIFI_Manager.h"

extern void wtlcd_loadWifiMenu();
extern void wtlcd_LanguageMenu();
extern void lcd_control_temperature_menu();
void wtlcd_Load_led_set_menu();
void wtlcd_Load_quiet_mode_menu();
void wtlcd_Load_powerloss_menu();
void wt_zoffset_prepare_function();
void wtlcd_control_info();

/**
*
* "Control" submenu
*
*/

void lcd_factory_settings() {
	settings.reset();
	settings.save();				// 恢复出厂设置
	wifi_manager.LogReset();
	lcd_completion_feedback();
	wt_restart();
}

void lcd_control_menu()
{
	START_MENU();

	MENU_BACK_MULTI(MMSG_MAIN[wtvar_language]);
	MENU_ITEM_MULTI(submenu, MMSG_TEMPERATURE[wtvar_language], lcd_control_temperature_menu);
	MENU_ITEM_MULTI(submenu, MMSG_ZPROBE_ZOFFSET[wtvar_language], wt_zoffset_prepare_function);
	MENU_ITEM_MULTI(function, MMSG_NETWORK_MENU[wtvar_language], wtlcd_loadWifiMenu);
	MENU_ITEM_MULTI(submenu, MMSG_LANGUAGE_MENU[wtvar_language], wtlcd_LanguageMenu);
	MENU_ITEM_MULTI(submenu, MMSG_LED_CONTROL[wtvar_language], wtlcd_Load_led_set_menu);
	MENU_ITEM_MULTI(submenu, MMSG_QUIET_MODE[wtvar_language], wtlcd_Load_quiet_mode_menu);
	MENU_ITEM_MULTI(submenu, MMSG_POWERLOSS_SET[wtvar_language], wtlcd_Load_powerloss_menu);
	MENU_ITEM_MULTI(submenu, MMSG_MACHINE_INFO[wtvar_language], wtlcd_control_info);
	MENU_ITEM_MULTI(function, MMSG_RESTORE_DEFAULTS[wtvar_language], lcd_factory_settings);

	END_MENU();
}


void wtlcd_control_info()
{
	if (use_click())
		return lcd_goto_previous_menu();

	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_MACHINE_INFO[wtvar_language], true, true);

	STATIC_ITEM_MULTI(MMSG_INFO_MACHINE[wtvar_language], false, false, MACHINE_NAME);

	STATIC_ITEM_MULTI(MMSG_INFO_HARDWARE[wtvar_language], false, false, HARDWARE_VERSION);

	STATIC_ITEM_MULTI(MMSG_INFO_VERSION[wtvar_language], false, false, SHORT_BUILD_VERSION);

	END_SCREEN();
}

// zoffset参数设置界面
float zoffset_beginZpos = 0;
void wt_zoffset_menu()
{
	if (use_click())
	{
		probe.offset.z += current_position[Z_AXIS] - zoffset_beginZpos;
		(void)settings.save();
		queue.enqueue_now_P(PSTR("G1 Z50 F500"));
		wifi_manager.isBusy(false);
		return lcd_goto_screen(lcd_control_menu);
	}

	ENCODER_DIRECTION_NORMAL();
	if (encoderPosition) 
	{

		// Start with no limits to movement
		float min = current_position[Z_AXIS] - 100,
			max = current_position[Z_AXIS] + 100;

		// Get the new position
		const float diff = float((int32_t)encoderPosition) * move_menu_scale;

		current_position[Z_AXIS] += diff;
		if ((int32_t)encoderPosition < 0)
			NOLESS(current_position[Z_AXIS], min);
		else
			NOMORE(current_position[Z_AXIS], max);

		manual_move_to_current(Z_AXIS);
		lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
	}
	encoderPosition = 0;
	if (lcdDrawUpdate) 
	{
		//const float pos = NATIVE_TO_LOGICAL(current_position[Z_AXIS], Z_AXIS) - zoffset_beginZpos + probe.offset.z;
		const float pos = current_position[Z_AXIS] - zoffset_beginZpos + probe.offset.z;
		lcd_implementation_drawedit(MMSG_ZPROBE_TITLE[wtvar_language], ftostr42sign(pos));
	}
}

// 载入zoffset界面
void wt_zoffset_prepare_menu()
{
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
	_lcd_synchronize();

	safe_delay(50);
	_lcd_synchronize();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
	drawing_screen = screen_changed = true;

	if (queue.length == 0)
	{
		move_menu_scale = 0.05f;
		zoffset_beginZpos = current_position[Z_AXIS];
		lcd_goto_screen(wt_zoffset_menu);
	}
}

// zoffset界面载入前准备
void wt_zoffset_prepare_function()
{
	queue.enqueue_now_P(PSTR("G90"));
	queue.enqueue_now_P(PSTR("G28 Z F500"));
	queue.enqueue_now_P(PSTR("G28 X Y F2000"));
	queue.enqueue_now_P(PSTR("G29"));
	queue.enqueue_now_P(PSTR("G1 X50 Y50 F2000"));
	queue.enqueue_now_P(PSTR("G1 Z0 F500"));

	wifi_manager.isBusy(true);
	lcd_goto_screen(wt_zoffset_prepare_menu);
}

void wt_led_turn_on()
{
	wt_led_set(1);
	lcd_return_to_status();
}

void wt_led_turn_off()
{
	wt_led_set(0);
	lcd_return_to_status();
}

void wt_led_set_menu()
{
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_LED_CONTROL[wtvar_language], false, false);

	encoderTopLine = 0;

	MENU_ITEM(function, MMSG_LCD_ON[wtvar_language], wt_led_turn_on);
	MENU_ITEM(function, MMSG_LCD_OFF[wtvar_language], wt_led_turn_off);


	END_MENU();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

void wtlcd_Load_led_set_menu()
{
	if (wtvar_led_state)
		lcd_goto_screen(wt_led_set_menu, 1);
	else
		lcd_goto_screen(wt_led_set_menu, 2);
}


void wtlcd_endstop_info()
{
	if (use_click())
		return lcd_goto_previous_menu();

	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_LCD_ENDSTOPS[wtvar_language], true, true);

	if (READ(X_MAX_PIN))
	{
		STATIC_ITEM("X: ", false, false, MMSG_LCD_ON[wtvar_language]);
	}
	else
	{
		STATIC_ITEM("X: ", false, false, MMSG_LCD_OFF[wtvar_language]);
	}

	if (READ(Y_MIN_PIN))
	{
		STATIC_ITEM("Y: ", false, false, MMSG_LCD_OFF[wtvar_language]);
	}
	else
	{
		STATIC_ITEM("Y: ", false, false, MMSG_LCD_ON[wtvar_language]);
	}

	if (READ(Z_MAX_PIN))
	{
		STATIC_ITEM("Z: ", false, false, MMSG_LCD_ON[wtvar_language]);
	}
	else
	{
		STATIC_ITEM("Z: ", false, false, MMSG_LCD_OFF[wtvar_language]);
	}

	if (READ(Z_MIN_PIN))
	{
		STATIC_ITEM_MULTI(MMSG_ENDSTOP_PROBE[wtvar_language], false, false, MMSG_LCD_OFF[wtvar_language]);
	}
	else
	{
		STATIC_ITEM_MULTI(MMSG_ENDSTOP_PROBE[wtvar_language], false, false, MMSG_LCD_ON[wtvar_language]);
	}

	END_SCREEN();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

void wt_quiet_mode_on()
{
	wt_quiet_mode(1);
	lcd_return_to_status();
}

void wt_quiet_mode_off()
{
	wt_quiet_mode(0);
	lcd_return_to_status();
}

void wt_quiet_mode_menu()
{
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_QUIET_MODE[wtvar_language], false, false);
	encoderTopLine = 0;

	MENU_ITEM(function, MMSG_LCD_ON[wtvar_language], wt_quiet_mode_on);
	MENU_ITEM(function, MMSG_LCD_OFF[wtvar_language], wt_quiet_mode_off);


	END_MENU();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

void wtlcd_Load_quiet_mode_menu()
{
	if (wtvar_quiet_mode)
		lcd_goto_screen(wt_quiet_mode_menu, 1);
	else
		lcd_goto_screen(wt_quiet_mode_menu, 2);
}

void wt_powerloss_on()
{
	wt_powerloss_set(1);
	lcd_return_to_status();
}

void wt_powerloss_off()
{
	wt_powerloss_set(0);
	lcd_return_to_status();
}

void wt_powerloss_menu()
{
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_POWERLOSS_SET[wtvar_language], false, false);
	encoderTopLine = 0;

	MENU_ITEM(function, MMSG_LCD_ON[wtvar_language], wt_powerloss_on);
	MENU_ITEM(function, MMSG_LCD_OFF[wtvar_language], wt_powerloss_off);


	END_MENU();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

void wtlcd_Load_powerloss_menu()
{
	if (wtvar_enablepowerloss)
		lcd_goto_screen(wt_powerloss_menu, 1);
	else
		lcd_goto_screen(wt_powerloss_menu, 2);
}