/**
* Copyright (C) 2022 WEEDO3D Perron
 */

/**
* WEEDO 自用指令
* 主板发往打印机的指令，以W开头，后跟1至3位数字
* W1 联机打印开始
* W2 联机打印结束
* W3 联机打印中止
* W4 联机打印暂停
* W5 联机打印恢复
* W6 SPARK已连接
* W7 SPARK已断开
* W100 欢迎界面1 欢迎使用 xxxx
* W001 欢迎界面2 第1步 载入料丝
* W002 欢迎界面3 第2步 插入TF卡
* W003 欢迎界面4 第3步 选择文件打印
* W106 显示正在下载界面
* W107 关闭正在下载界面
* W200 查询文件是否存在，存在返回true，不存在返回false, W200 test.gco
* W201 查询打印机状态，返回 打印机状态, 正在打印的文件， 已打印时间， 打印的文件字节量
* W202 打开现有文件并添加数据，暂时无效
* W203 打印机立即重启

* W209 取消打印
* W210 连接工厂默认WIFI网
* W216 保存参数至SD卡gcode文件
* W217 从SD卡gcode文件中读取参数并保存至主板内存
* W230 恢复默认参数并重启
* W300 设置WIFI网ssid
* W301 设置WIFI网pwd
* W302 连接WIFI网络
*
*
*
* 打印机发往主板的指令，以WTCMD:开头，后跟大写英文单词
* WTCMD:IP			读取ESP32的IP地址
* WTCMD:RESTART		ESP32立即重启
* WTCMD:VERSION		读取ESP32的硬件和固件版本
* WTCMD:OTADEFAULT	从默认网址OTA升级
* WTCMD:OTAURI		从指定网址OTA升级
* WTCMD:OTAFORCE	从指定网络OTA至指定版本
* WTCMD:WIFIDEFAULT	连接工厂默认WIFI网
* WTCMD:JOIN		连接到指定的WIFI网
*
*/

#include "../MarlinCore.h"
#include "../gcode/parser.h"
#include "../gcode/gcode.h"
#include "../HAL/STM32F1/watchdog.h"
#include "../sd/cardreader.h"
#include "../libs/nozzle.h"
#include "../libs/duration_t.h"
#include "../libs/buzzer.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../module/configuration_store.h"
#include "../module/printcounter.h"
#include "../module/probe.h"
#include "../module/stepper.h"
#include "../feature/pause.h"
#include "../feature/host_actions.h"
#include "../lcd/ultralcd.h"
#include "../wifi/WIFI_Manager.h"
#include "../wifi/WIFI_Serial.h"

#include "WTCMD.h"
#include "WTUtilty.h"
#include "WTPacket.h"
#include "WTGcodeinfo.h"

#if ENABLED(POWER_LOSS_RECOVERY)
#include "../feature/powerloss.h"
#endif

#define SD_CONFIG_FILE "config.sav"

#define ACTION_ON_PAUSE "pause"
#define ACTION_ON_PAUSED "paused"
#define ACTION_ON_RESUME "resume"
#define ACTION_ON_CANCEL "cancel"
#define ACTION_ON_DISCONNECT "disconnect"
#define ACTION_ON_START "start"
#define ACTION_ON_FINISH "finish"

#define EEPROM_HOST_MODE 1000

#define STATUS_CHECK_TIME_INTERVAL 1000
#define STATUS_TOGGLE_TIME_INTERVAL 5000

extern void wtlcd_welcome1();
extern void wtlcd_welcome2();
extern void wtlcd_welcome3();
extern void wtlcd_welcome4();
extern void menu_action_submenu(screenFunc_t func);
extern void wtlcd_ShowUploadingScreen();
extern void wt_restart();
extern void lcd_sdcard_stop();
extern void lcd_return_to_status();
extern void lcd_main_menu();
extern CardReader card;

WT_STATUS wt_machineStatus = WT_STATUS::IDLE;

WT_MAIN_ACTION wt_mainloop_action = WT_MAIN_ACTION::WMA_IDLE;

uint8_t wt_onlineprinting = SPARK_IDLE; // 0 = not printing, 1 = paused, 2 = printing, 3 = lost

bool isOTAUpdating = false;
char downloadFilename[FILENAME_LENGTH + 2];
char downloadPercent[5];
uint8_t download_state; // 0, Downloading, 1, Download End, 2, Download Fail
char parsedString[30];
char WifiErrorMessage[50];

uint8_t wt_active_extruder;

extern xyze_pos_t resume_position;

uint32_t wt_status_check = STATUS_CHECK_TIME_INTERVAL;
uint32_t wt_status_togle = STATUS_TOGGLE_TIME_INTERVAL;

// ESP32立即重启
void WT_ESP32_Restart()
{
	SERIAL_ECHOLNPGM("WTCMD:RESTART");
}

// 从默认网址OTA升级
void WT_OTA_Default()
{
	SERIAL_ECHOLNPGM("WTCMD:OTADEFAULT");
}

// 连接工厂默认WIFI网
void WT_WIFI_Default()
{
	SERIAL_ECHOLNPGM("WTCMD:WIFIDEFAULT");
}

// WEEDO云连接状态
void WT_TB_STATE()
{
	SERIAL_EOL();
	SERIAL_ECHOPGM("WTCMD:TBSTATE ");
}

// 欢迎界面1 欢迎使用 xxxx
void Show_WelcomeScreen1()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome1);
}

// 欢迎界面2 第1步 载入料丝
void Show_WelcomeScreen2()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome2);
}

// 欢迎界面3 第2步 插入TF卡
void Show_WelcomeScreen3()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome3);
}

// 欢迎界面4 第3步 选择文件打印
void Show_WelcomeScreen4()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome4);
}

void GetMachineStatus()
{
	SERIAL_ECHOPGM("MStatus:");
	SERIAL_ECHO(wt_machineStatus);

	SERIAL_ECHOPGM(" MFile:");
#ifdef SDSUPPORT
	card.printFilename();
#endif

	char buffer[21];
	duration_t elapsed = print_job_timer.duration();
	elapsed.toString(buffer);

	SERIAL_ECHOPGM(" MTime:");
	SERIAL_ECHO(buffer);

	SERIAL_ECHOPGM(" MPercent:");
#ifdef SDSUPPORT
	card.report_status();
#endif

	SERIAL_EOL();
}

void wt_CloseUploadScreen()
{
	buzzer.tone(30, 1000);
	lcd_return_to_status();
}

void wt_spark_begin()
{
	wt_onlineprinting = SPARK_PRINTING;
	print_job_timer.start();
	lcd_setstatusPGM(MMSG_PRINTING[wtvar_language], -1);
	lcd_return_to_status();
	wt_machineStatus = WT_STATUS::PRINTING;
	wifi_manager.isBusy(true);
}

void wt_spark_end()
{
	wt_machineStatus = WT_STATUS::FINISH;
	wt_onlineprinting = SPARK_IDLE;
	wifi_manager.isBusy(false);
	print_job_timer.stop();
	queue.enqueue_one_P(PSTR("M18"));
	if (print_job_timer.duration() > 60)
		queue.enqueue_one_P(PSTR("M31"));
	else
		lcd_reset_status();
}

void wt_spark_cancel()
{
	lcd_sdcard_stop();
	while (1)
		;
}

void wt_spark_pause()
{
#if ENABLED(PARK_HEAD_ON_PAUSE)
	queue.enqueue_one_P(PSTR("M125"));
#endif
	print_job_timer.pause();

	wt_machineStatus = WT_STATUS::PAUSE;
	wt_onlineprinting = SPARK_PAUSED;

	lcd_setstatusPGM(MMSG_PRINT_PAUSED[wtvar_language], -1);
}

void wt_spark_resume()
{
#if ENABLED(PARK_HEAD_ON_PAUSE)
	resume_print(0, 0, 0, 0);
#endif
	print_job_timer.start();

	planner.synchronize();

	wt_machineStatus = WT_STATUS::PRINTING;
	wt_onlineprinting = SPARK_PRINTING;
	lcd_setstatusPGM(MMSG_PRINTING[wtvar_language], -1);
	lcd_return_to_status();
}

void wt_reset_param(void)
{
	wtvar_gohome = 0;
	wtvar_showWelcome = 1;
	(void)settings.save();

	safe_delay(200);
	wt_restart();
}

// 向Spark发送暂停打印请求
void wt_online_pause()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_PAUSE);
}

// 向Spark发送恢复打印请求
void wt_online_resume()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_RESUME);
}

// 向Spark发送取消打印请求
void wt_online_cancel()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_CANCEL);
}

extern uint8_t lcd_status_update_delay;
// W Command Process
void WTCMD_Process()
{
	switch (parser.codenum)
	{
	case 1: // 联机打印开始
		wt_spark_begin();
		break;

	case 2: // 联机打印结束
		wt_spark_end();
		break;

	case 3: // 联机打印中止
		wt_spark_cancel();
		break;

	case 4: // 联机打印暂停
		wt_spark_pause();
		break;

	case 5: // 联机打印恢复
		wt_spark_resume();
		break;

	case 6: // SPARK已连接
		lcd_setstatusPGM(MMSG_OCTOPRINT_OPEN, -1);
		break;

	case 7: // SPARK已断开
		lcd_setstatusPGM(MMSG_OCTOPRINT_CLOSE, -1);
		break;

	case 100: // 欢迎界面1 欢迎使用 xxxx
		Show_WelcomeScreen1();
		break;

	case 101: // 欢迎界面2 第1步 载入料丝
		Show_WelcomeScreen2();
		break;

	case 102: // 欢迎界面3 第2步 插入TF卡
		Show_WelcomeScreen3();
		break;

	case 103: // 欢迎界面4 第3步 选择文件打印
		Show_WelcomeScreen4();
		break;

	case 106: // 显示正在下载界面
		ZERO(downloadFilename);
		if (strlen(parser.string_arg) <= FILENAME_LENGTH)
			strcpy(downloadFilename, parser.string_arg);
		else
			strncpy(downloadFilename, parser.string_arg, FILENAME_LENGTH);
		wtlcd_ShowUploadingScreen();
		break;

	case 107: // 关闭正在下载界面
		wt_CloseUploadScreen();
		break;

	case 201: // 查询打印机状态，返回 打印机状态, 正在打印的文件， 已打印时间， 打印的文件字节量
		GetMachineStatus();
		break;

	case 203: // 打印机立即重启
		wt_restart();
		while (1)
			;
		break;

	case 209: // 取消打印
		lcd_sdcard_stop();
		break;

	case 210: // 连接工厂默认wifi网
		WT_WIFI_Default();
		break;

	case 211: // 自动进丝
		queue.enqueue_one_P(PSTR("M104 T0 S210"));
		queue.enqueue_one_P(PSTR("M701 T0"));
		queue.enqueue_one_P(PSTR("M104 S0"));
		break;

	case 212: // 自动退丝
		queue.enqueue_one_P(PSTR("M104 T0 S210"));
		queue.enqueue_one_P(PSTR("M702 T0"));
		queue.enqueue_one_P(PSTR("M104 S0"));
		break;

	case 230: // 恢复默认参数并重启
		wt_reset_param();
		break;

	case 300: // 设置WIFI网ssid
		wifi_manager.Set_AP_SSID(parser.string_arg);
		break;

	case 301: // 设置WIFI网pwd
		wifi_manager.Set_AP_PWD(parser.string_arg);
		break;

	case 302: // 连接WIFI网络
		wifi_manager.AP_Join();
		break;
	}
}

// restart
void wt_restart()
{
	nvic_sys_reset();
};

void calc_local_printing_time(void)
{
	float p = (float)progress_bar_percent * 0.01;
	uint32_t elapsed = print_job_timer.duration();

	long est_total;
	if (gcodeinfo.info.i_totaltime > 0)
	{
		if (p > 0)
			est_total = (long)((float)elapsed / p);
		else
			est_total = gcodeinfo.info.i_totaltime;

		long est_last = est_total - elapsed;
		long slicing_last = gcodeinfo.info.i_totaltime - elapsed;
		if (slicing_last < 0)
			slicing_last = 0;

		gcodeinfo.info.i_lefttime = (long)((float)est_last * p + (float)slicing_last * (1 - p));
	}
	else
	{
		if (p > 0)
			est_total = (long)((float)elapsed / p);
		else
			est_total = 0;

		gcodeinfo.info.i_lefttime = est_total - elapsed;
		if (gcodeinfo.info.i_lefttime < 0)
			gcodeinfo.info.i_lefttime = 0;
	}
}

// main loop action
void wt_loopaction(void)
{
	if (wt_mainloop_action == WMA_IDLE) return;

	if (wt_mainloop_action == WMA_PAUSE)
	{

		if (queue.length > 0) return;

		queue.enqueue_now_P(PSTR("M125"));
		wt_mainloop_action = WMA_IDLE;

		return;
	}

	if (wt_mainloop_action == WMA_RESUME)
	{

		if (queue.length > 0) return;

		queue.enqueue_one_P(PSTR("M24"));
		wt_mainloop_action = WMA_IDLE;
	}
}

// got bootload
void wt_bootload()
{
	// 写入更新标识
	uint16_t working_crc = 0;
	int pos = 0;
	uint8_t signal = 0x5A;
	persistentStore.access_start();
	persistentStore.write_data(pos, &signal, 1, &working_crc);
	persistentStore.access_finish();
	wt_restart();
}

void host_action_start()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_START);
}

void host_action_finish()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_FINISH);
}

// load sd card
void wt_load_sd(void)
{
	if ((uint8_t)IS_SD_INSERTED())
	{
		digitalWrite(STM_SD_BUSY, HIGH);
		card.mount();
		SERIAL_ECHOLNPGM("mount sd");
		lcd_sd_status = 1;
		wifi_manager.FileListBegin();
		card.lsRoot();
		wifi_manager.FileListEnd();
	}
	else
	{
		lcd_sd_status = 0;
	}
	wifi_manager.SetSDState(lcd_sd_status);
}

// unload sd card
void wt_unload_sd(void)
{
	if ((uint8_t)IS_SD_INSERTED())
	{
		card.release();
		safe_delay(20);
		digitalWrite(STM_SD_BUSY, LOW);
		SERIAL_ECHOLNPGM("unmount sd");
		lcd_sd_status = 2;
	}
	else
	{
		lcd_sd_status = 0;
	}
	wifi_manager.SetSDState(lcd_sd_status);
}

// set quiet mode
void wt_quiet_mode(uint8_t modeon)
{
	wtvar_quiet_mode = modeon;

	if (wtvar_quiet_mode)
	{
		planner.set_max_feedrate(0, 20);
		planner.set_max_feedrate(1, 20);
		planner.set_max_feedrate(2, 20);

		planner.set_max_acceleration(0, 400);
		planner.set_max_acceleration(1, 300);
		planner.set_max_acceleration(2, 200);

		thermalManager.soft_pwm_amount_autofan[0] = AUTO_FAN_PWM_SILENCE;
	}
	else
	{
		planner.set_max_feedrate(0, 300);
		planner.set_max_feedrate(1, 200);
		planner.set_max_feedrate(2, 20);

		planner.set_max_acceleration(0, 1000);
		planner.set_max_acceleration(1, 600);
		planner.set_max_acceleration(2, 400);

		thermalManager.soft_pwm_amount_autofan[0] = AUTO_FAN_PWM_MAX;
	}

	(void)settings.save();
}

void wt_led_init(void)
{
	pinMode(EXTEND_IO0, OUTPUT);
	digitalWrite(EXTEND_IO0, HIGH);
	wtvar_led_state = 1;
}

void wt_led_set(uint8_t state)
{
	wtvar_led_state = state;
	if (state)
		digitalWrite(EXTEND_IO0, HIGH);
	else
		digitalWrite(EXTEND_IO0, LOW);
}

void wt_powerloss_set(uint8_t state)
{
	wtvar_enablepowerloss = state;
	recovery.enable(wtvar_enablepowerloss);
	(void)settings.save();
}

void wt_send_params()
{
	char buffer[60];

	gserial.SendCmd(REG_PARAM_BEGIN);

	// M851
	gserial.SendString(REG_PARAM_DATA, "Z-Probe Offset (mm):");

	ZERO(buffer);
	sprintf(buffer, "M851 X%.2f Y%.2f Z%.2f",
					 LINEAR_UNIT(probe.offset_xy.x),
					 LINEAR_UNIT(probe.offset_xy.y),
					 probe.offset.z);
	gserial.SendString(REG_PARAM_DATA, buffer);
		
	// M203
	gserial.SendString(REG_PARAM_DATA, "Maximum feedrates (units/s):");

	ZERO(buffer);
	sprintf(buffer, "M203 X%.2f Y%.2f Z%.2f E%.2f",
					 LINEAR_UNIT(planner.settings.max_feedrate_mm_s[X_AXIS]),
					 LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Y_AXIS]),
					  LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Z_AXIS]),
					 VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS]));
	gserial.SendString(REG_PARAM_DATA, buffer);

	// M201
	gserial.SendString(REG_PARAM_DATA, "Maximum Acceleration (units/s2):");

	ZERO(buffer);
	sprintf(buffer, "M201 X%.2f Y%.2f Z%.2f E%.2f",
					 LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[X_AXIS]),
					 LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Y_AXIS]),
					 LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Z_AXIS]),
					 VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS]));
	gserial.SendString(REG_PARAM_DATA, buffer);

	// M301
	gserial.SendString(REG_PARAM_DATA, "PID settings:");

	ZERO(buffer);
	sprintf(buffer, "M301 P%.2f I%.2f D%.2f",
					 PID_PARAM(Kp, 0),
					 unscalePID_i(PID_PARAM(Ki, 0)),
					 unscalePID_d(PID_PARAM(Kd, 0)));
	gserial.SendString(REG_PARAM_DATA, buffer);

	// M413
	gserial.SendString(REG_PARAM_DATA, "Power-Loss Recovery:");

	ZERO(buffer);
	sprintf(buffer, "M413 S%d",
					 int(recovery.enabled));
	gserial.SendString(REG_PARAM_DATA, buffer);


	gserial.SendCmd(REG_PARAM_END);

}
