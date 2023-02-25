/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"
#include "../../module/configuration_store.h"

extern void wtlcd_goto_welcome2();

void wtlcd_language_end()
{
	(void)settings.save();
	if (wtvar_wizzard_mode)
	{
		wtlcd_goto_welcome2();
	}
	else
	{
		lcd_reset_status();
		lcd_return_to_status();
	}
}

void wtlcd_language_en()
{
	wtvar_language = LANGUAGE_EN;
	wtlcd_language_end();
}

void wtlcd_language_cn()
{
	wtvar_language = LANGUAGE_CN;
	wtlcd_language_end();
}

void wtlcd_language_fr()
{
	wtvar_language = LANGUAGE_FR;
	wtlcd_language_end();
}

void wtlcd_language_es()
{
	wtvar_language = LANGUAGE_ES;
	wtlcd_language_end();
}

void wtlcd_language_it()
{
	wtvar_language = LANGUAGE_IT;
	wtlcd_language_end();
}

void wtlcd_language_kana()
{
	wtvar_language = LANGUAGE_KANA;
	wtlcd_language_end();
}

void wtlcd_language_pt()
{
	wtvar_language = LANGUAGE_PT;
	wtlcd_language_end();
}

void wtlcd_language_nl()
{
	wtvar_language = LANGUAGE_NL;
	wtlcd_language_end();
}

void wtlcd_language_tr()
{
	wtvar_language = LANGUAGE_TR;
	wtlcd_language_end();
}

void wtlcd_language_de()
{
	wtvar_language = LANGUAGE_DE;
	wtlcd_language_end();
}

void wtlcd_language_kr()
{
	wtvar_language = LANGUAGE_KR;
	wtlcd_language_end();
}

void wtlcd_language_ru()
{
	wtvar_language = LANGUAGE_RU;
	wtlcd_language_end();
}

// 语言选择菜单
void wtlcd_LanguageMenu()
{
	START_MENU();
	MENU_BACK_MULTI(MMSG_CONTROL[wtvar_language]);

	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_EN[wtvar_language], wtlcd_language_en);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_CN[wtvar_language], wtlcd_language_cn);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_FR[wtvar_language], wtlcd_language_fr);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_DE[wtvar_language], wtlcd_language_de);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_ES[wtvar_language], wtlcd_language_es);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_IT[wtvar_language], wtlcd_language_it);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_KANA[wtvar_language], wtlcd_language_kana);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_PT[wtvar_language], wtlcd_language_pt);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_NL[wtvar_language], wtlcd_language_nl);
	MENU_ITEM_MULTI(function, MMSG_LANGUAGE_TR[wtvar_language], wtlcd_language_tr);

	END_MENU();
}