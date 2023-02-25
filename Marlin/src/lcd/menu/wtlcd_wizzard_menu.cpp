/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"
#include "../../module/configuration_store.h"

extern void wtlcd_language_en();
extern void wtlcd_language_cn();
extern void wtlcd_language_fr();
extern void wtlcd_language_es();
extern void wtlcd_language_it();
extern void wtlcd_language_kana();
extern void wtlcd_language_pt();
extern void wtlcd_language_nl();
extern void wtlcd_language_tr();
extern void wtlcd_language_de();
extern void wtlcd_language_kr();
extern void wtlcd_language_ru();
extern void wt_load_sdcard();

void wtlcd_welcome1();
void wtlcd_welcome2();
void wtlcd_welcome3();
void wtlcd_welcome4();

void wtlcd_goto_welcome1()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome1);
}

void wtlcd_goto_welcome2()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome2);
}

void wtlcd_goto_welcome4()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome4);
}

void wtlcd_quitwelcome()
{
	if (wtvar_showWelcome)
	{
		wtvar_showWelcome = 0;
		(void)settings.save();
	}
	wtvar_wizzard_mode = 0;
	lcd_reset_status();
	lcd_return_to_status();
}

void wtlcd_welcome1()
{
	START_MENU();
	// show language selector in wizzard menu
	STATIC_ITEM_MULTI(MMSG_LANGUAGE_MENU[0], false, false);

	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_EN[0], wtlcd_language_en);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_FR[2], wtlcd_language_fr);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_DE[3], wtlcd_language_de);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_ES[4], wtlcd_language_es);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_CN[1], wtlcd_language_cn);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_IT[5], wtlcd_language_it);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_KANA[6], wtlcd_language_kana);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_PT[7], wtlcd_language_pt);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_NL[8], wtlcd_language_nl);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_TR[9], wtlcd_language_tr);
	
	END_MENU();

	wtvar_wizzard_mode = 1;
}

void wtlcd_welcome2()
{
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_WELCOME_SCREEN_2[wtvar_language], false, false);

	MENU_ITEM_MULTI(gcode, MMSG_BUTTON_NEXT[wtvar_language], PSTR("M104 T0 S210\r\nM701 T0\r\nM104 S0\r\nW102"));
	MENU_ITEM_MULTI(function, MMSG_WELCOME_SKIP[wtvar_language], wtlcd_quitwelcome);
	END_MENU();

	if (wtvar_showWelcome)
	{
		wtvar_showWelcome = 0;
		(void)settings.save();
	}
}

void wtlcd_welcome3()
{
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_WELCOME_SCREEN_3[wtvar_language], false, false);

	MENU_ITEM_MULTI(function, MMSG_BUTTON_NEXT[wtvar_language], wtlcd_goto_welcome4);
	MENU_ITEM_MULTI(function, MMSG_WELCOME_SKIP[wtvar_language], wtlcd_quitwelcome);
	END_MENU();
}

void wtlcd_welcome4()
{
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_WELCOME_SCREEN_4[wtvar_language], false, false);

	MENU_ITEM_MULTI(function, MMSG_CARD_MENU[wtvar_language], wt_load_sdcard);
	MENU_ITEM_MULTI(function, MMSG_WELCOME_SKIP[wtvar_language], wtlcd_quitwelcome);
	END_MENU();
}
