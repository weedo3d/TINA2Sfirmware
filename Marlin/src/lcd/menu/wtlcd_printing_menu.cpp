/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"
#include "../../wtlib/WTGcodeinfo.h"
#include "../../core/macros.h"
#include "../../module/printcounter.h"
#include "../../gcode/queue.h"
#include "../../module/configuration_store.h"

uint8_t wtvar_sdcard_filelist[SDCARD_MENU_PAGE_ITEMS][60];
uint8_t wtvar_sdcard_listcount = 0;

extern void wt_resume_screen();

void wt_sdcard_menu();

#if ENABLED(SDSUPPORT)

void lcd_sdcard_pause() {
	card.pauseSDPrint();
	print_job_timer.pause();
// #if ENABLED(PARK_HEAD_ON_PAUSE)
// 	queue.enqueue_one_P(PSTR("M125"));
// #endif
	lcd_reset_status();
	lcd_return_to_status();

	wt_machineStatus = WT_STATUS::PAUSE;
	wt_mainloop_action = WMA_PAUSE;	
}

void lcd_sdcard_resume() {
// #if ENABLED(PARK_HEAD_ON_PAUSE)
// 	queue.enqueue_now_P(PSTR("M24"));
// #else
// 	card.startFileprint();
// 	print_job_timer.start();
// #endif
	lcd_reset_status();
	lcd_return_to_status();
	lcd_setstatus((const char*)card.utf8_filename, true);

	wt_machineStatus = WT_STATUS::PRINTING;
	wt_mainloop_action = WMA_RESUME;	
}

bool abort_sd_printing; // =false

void lcd_sdcard_stop()
{
	wait_for_heatup = wait_for_user = false;
	//abort_sd_printing = true;

	lcd_setstatusPGM(PSTR(MSG_PRINT_ABORTED), -1);
	lcd_return_to_status();

	wtvar_gohome = 1;
	(void)settings.save();

	wt_restart();
}

#endif // SDSUPPORT

#if ENABLED(POWER_LOSS_RECOVERY)

// 续打
static void lcd_power_loss_recovery_resume() {
	lcd_goto_screen(wt_resume_screen);
	
	wtvar_resume_stage = DSEP_PREHEAT;
}

// 取消续打
static void lcd_power_loss_recovery_cancel() {
	card.removeJobRecoveryFile();
	card.autostart_index = 0;
	lcd_return_to_status();
}

// 续打菜单
void lcd_job_recovery_menu() {
	defer_return_to_status = true;
	START_MENU();
	STATIC_ITEM_MULTI(MMSG_POWER_LOSS_RECOVERY[wtvar_language], true, false);
	MENU_ITEM_MULTI(function, MMSG_RESUME_PRINT[wtvar_language], lcd_power_loss_recovery_resume);
	MENU_ITEM_MULTI(function, MMSG_STOP_PRINT[wtvar_language], lcd_power_loss_recovery_cancel);
	END_MENU();
}

#endif // POWER_LOSS_RECOVERY

// 关机保存中界面
void wt_safe_off_menu()
{
	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_SAVED_OFF[wtvar_language], true, false);
	STATIC_ITEM_MULTI(MMSG_SAVED_OFF_1[wtvar_language], false, false);
	STATIC_ITEM_MULTI(MMSG_SAVED_OFF_2[wtvar_language], false, false);
	STATIC_ITEM_MULTI(MMSG_SAVED_OFF_3[wtvar_language], false, false);
	END_SCREEN();
}

// 载入关机保存中界面
extern uint8_t job_saveoff;
void wt_saveoff()
{	
	wtvar_save_stage = DSSP_CHKSTS;

	menu_action_submenu(wt_safe_off_menu);
}

// 刷新分页显示文件列表
void wt_sdcard_showpage()
{
	ZERO(wtvar_sdcard_filelist);
	if (wtvar_sdcard_filecount > 0 && wtvar_sdcard_pageid < wtvar_sdcard_pagecount)
	{
		uint8_t i, endpos;
		if (wtvar_sdcard_filecount >= ((wtvar_sdcard_pageid + 1) * SDCARD_MENU_PAGE_ITEMS))
		{	// has 6 items
			endpos = (wtvar_sdcard_pageid + 1) * SDCARD_MENU_PAGE_ITEMS;
		}
		else
		{
			endpos = wtvar_sdcard_filecount;
		}

		wtvar_sdcard_listcount = endpos - wtvar_sdcard_pageid * SDCARD_MENU_PAGE_ITEMS;

		for (i = wtvar_sdcard_pageid * SDCARD_MENU_PAGE_ITEMS; i < endpos;  i++)
		{
			card.getfilename_sorted(wtvar_sdcard_filecount - i - 1);

			card.get_utf8_filename();
			memcpy(wtvar_sdcard_filelist[i - wtvar_sdcard_pageid * SDCARD_MENU_PAGE_ITEMS], card.utf8_filename, 60);

			wtvar_sdcard_file_index[i - wtvar_sdcard_pageid * SDCARD_MENU_PAGE_ITEMS] = wtvar_sdcard_filecount - i - 1;
		}
	}
	else
	{
		wtvar_sdcard_listcount = 0;
	}
	encoderPosition = 0;
	encoderLine = 0;

	lcd_goto_screen(wt_sdcard_menu);
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}


// 显示分页显示文件列表
void wt_sdcard_menu()
{
	START_MENU();
	if (wtvar_sdcard_pageid == 0)
	{
		MENU_BACK_MULTI(MMSG_MAIN[wtvar_language]);
	}
	else
	{
		MENU_ITEM_MULTI(prepage, MMSG_PRE_PAGE[wtvar_language], wt_sdcard_showpage);
	}

	uint8_t i;
	for(i = 0; i < wtvar_sdcard_listcount; i++)
	{
		MENU_ITEM_MULTI(listfile, MMSG_CARD_MENU[wtvar_language], (const char*)wtvar_sdcard_filelist[i]);
	}

	if (wtvar_sdcard_pageid < wtvar_sdcard_pagecount - 1)
	{
		MENU_ITEM_MULTI(nextpage, MMSG_NEXT_PAGE[wtvar_language], wt_sdcard_showpage);
	}

	END_MENU();

}

// 载入分页显示文件列表
void wt_load_sdcard()
{
	wtvar_sdcard_filecount = card.countFilesInWorkDir();
	if (wtvar_sdcard_filecount % SDCARD_MENU_PAGE_ITEMS > 0)
		wtvar_sdcard_pagecount = wtvar_sdcard_filecount / SDCARD_MENU_PAGE_ITEMS + 1;
	else
		wtvar_sdcard_pagecount = wtvar_sdcard_filecount / SDCARD_MENU_PAGE_ITEMS;
	
	wt_sdcard_showpage();
}


// dummy function
inline void wt_fun_none() {};

// 打印gcode文件
void wt_detail_print_menu()
{
	card.openAndPrintFile(gcodeinfo.info.filename);
	lcd_return_to_status();
	lcd_reset_status();
	wt_machineStatus = WT_STATUS::PRINTING;
	wifi_manager.isBusy(true);
	wifi_manager.SendFileDetail();
}

// gcode文件信息菜单
void wt_file_detail_menu()
{
	START_MENU();

	if (gcodeinfo.info.machineMatch)
	{
		MENU_ITEM_MULTI(function, MMSG_BUTTON_PRINT[wtvar_language], wt_detail_print_menu);
		MENU_BACK_MULTI(MMSG_WELCOME_SKIP[wtvar_language]);

		// MENU_ITEM_V_MULTI(function, MMSG_INFO_MACHINE[wtvar_language], gcodeinfo.info.machine, wt_fun_none);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_HEIGHT[wtvar_language], gcodeinfo.info.layerheight, wt_fun_none);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_DENSITY[wtvar_language], gcodeinfo.info.density, wt_fun_none);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_FILAMENT[wtvar_language], gcodeinfo.info.material, wt_fun_none);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_USAGE[wtvar_language], gcodeinfo.info.filament, wt_fun_none);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_TEMP[wtvar_language], gcodeinfo.info.temp0, wt_fun_none);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_TIME[wtvar_language], gcodeinfo.info.totaltime, wt_fun_none);
	}
	else
	{
		MENU_BACK_MULTI(MMSG_WELCOME_SKIP[wtvar_language]);
		STATIC_ITEM_MULTI(MMSG_MACHINE_NOMATCH[wtvar_language], false, false);
		MENU_ITEM_V_MULTI(function, MMSG_INFO_MACHINE[wtvar_language], gcodeinfo.info.machine, wt_fun_none);
	}

	END_MENU();
}

void wt_resume_screen()
{
	START_SCREEN();
	STATIC_ITEM(" ", false, false);
	STATIC_ITEM_MULTI(MMSG_RESTORE_PRINT[wtvar_language], true, false);
	END_SCREEN();
}
