/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"
#include "../../gcode/queue.h"

extern void wtlcd_endstop_info();
extern void wtlcd_goto_welcome1();
extern void lcd_move_menu();

void wt_action_autohome();
void wt_action_levelbed();

/**
*
* "Prepare" submenu
*
*/

void lcd_prepare_menu() {
	START_MENU();

	//
	// ^ Main
	//
	MENU_BACK_MULTI(MMSG_MAIN[wtvar_language]);

	// auto filament feed
	MENU_ITEM_MULTI(gcode, MMSG_FILAMENTLOAD[wtvar_language], PSTR("M104 T0 S210\r\nM701 T0\r\nM104 S0"));

	// auto filament retract
	MENU_ITEM_MULTI(gcode, MMSG_FILAMENTOUT[wtvar_language], PSTR("M104 T0 S210\r\nM702 T0\r\nM104 S0"));

	//
	// Move Axis
	//
	MENU_ITEM_MULTI(submenu, MMSG_MOVE_AXIS[wtvar_language], lcd_move_menu);

	//
	// Auto Home
	//
	//MENU_ITEM_MULTI(gcode, MMSG_AUTO_HOME[wtvar_language], PSTR("G28"));
	MENU_ITEM_MULTI(function, MMSG_AUTO_HOME[wtvar_language], wt_action_autohome);

	//
	// Level Bed
	//
	MENU_ITEM_MULTI(function, MMSG_LEVEL_BED[wtvar_language], wt_action_levelbed);

	MENU_ITEM_MULTI(submenu, MMSG_LCD_ENDSTOPS[wtvar_language], wtlcd_endstop_info);

	//
	// Disable Steppers
	//
	MENU_ITEM_MULTI(gcode, MMSG_DISABLE_STEPPERS[wtvar_language], PSTR("M84"));


	MENU_ITEM_MULTI(function, MMSG_WELCOME_MENU[wtvar_language], wtlcd_goto_welcome1);

	END_MENU();
}

void wt_action_autohome()
{
	lcd_return_to_status();
	queue.enqueue_one_P(PSTR("G28 Z F500"));
	queue.enqueue_one_P(PSTR("G28 X Y F1000"));
	queue.enqueue_one_P(PSTR("G1 X55 Y55 F1000"));

}

void wt_action_levelbed()
{
	lcd_return_to_status();
	queue.enqueue_one_P(PSTR("G28 Z F500"));
	queue.enqueue_one_P(PSTR("G28 X Y F1000"));
	queue.enqueue_one_P(PSTR("G29"));

}