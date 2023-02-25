/**
* Copyright (C) 2022 WEEDO3D Perron
*/

#ifndef WTCMD_H
#define WTCMD_H

#include "stdint.h"

#define SPARK_IDLE		0
#define SPARK_PAUSED	1
#define SPARK_PRINTING	2
#define SPARK_LOST		3

enum WT_STATUS : char {
	IDLE = 0, 
	PRINTING = 1,
	PAUSE = 2,
	ABORT = 3,
	FINISH = 4,
	ERROR = 5,
	DOWNLOADING = 6
};

enum WT_MAIN_ACTION : char
{
	WMA_IDLE = 0,
	WMA_PAUSE = 1,
	WMA_RESUME = 2
};

extern WT_STATUS wt_machineStatus;

extern WT_MAIN_ACTION wt_mainloop_action;

extern uint8_t download_state;						// 0, Downloading, 1, Download End, 2, Download Fail

void WTCMD_Process();

// ESP32立即重启
void WT_ESP32_Restart();

// 从默认网址OTA升级
void WT_OTA_Default();

// 连接工厂默认WIFI网
void WT_WIFI_Default();

// 向Spark发送暂停打印请求
void wt_online_pause();

// 向Spark发送恢复打印请求
void wt_online_resume();

// 向Spark发送取消打印请求
void wt_online_cancel();

// restart
void wt_restart();

// get machine status
void GetMachineStatus();

// send host action start message
void host_action_start();

// send host action finish message
void host_action_finish();

// main loop action
void wt_loopaction(void);

// got bootload
void wt_bootload();

// load sd card
void wt_load_sd(void);

// unload sd card
void wt_unload_sd(void);

// set quiet mode
void wt_quiet_mode(uint8_t modeon);

void wt_led_init(void);

void wt_led_set(uint8_t state);

void wt_powerloss_set(uint8_t state);

void wt_send_params();

#endif

