/**
* Copyright (C) 2022 WEEDO3D Perron
*/

#include <stdarg.h>

#include "../MarlinCore.h"
#include "../libs/duration_t.h"
#include "../sd/cardreader.h"
#include "../module/temperature.h"
#include "../module/printcounter.h"
#include "../module/probe.h"
#include "../module/configuration_store.h"
#include "../lcd/ultralcd.h"
#include "../wtlib/WTCMD.h"
#include "../gcode/queue.h"
#include "../wtlib/WTGcodeinfo.h"
#include "../wtlib/WTUtilty.h"
#include "WIFI_Manager.h"
#include "WIFI_Serial.h"

#define WIFI_CHECK_INTERVAL		3000

static char wifi_ssid[STRLEN_SSID];						// ESP32 SSID
static char wifi_pwd[STRLEN_SSID];						// ESP32 PWD

static uint32_t wifi_loop_check = WIFI_CHECK_INTERVAL;

extern void lcd_return_to_status();
extern void wtlcd_ShowUploadingScreen();
extern void lcd_factory_settings();
extern void wtlcd_ShowWifiErrorScreen();
extern char downloadFilename[FILENAME_LENGTH];
extern char downloadPercent[5];
extern char WifiErrorMessage[50];

WIFI_Manager wifi_manager;

WIFI_Manager::WIFI_Manager()
{

}

void WIFI_Manager::Init(void)
{
	testingMode = false;
	tbState = 0;
	esp32_message_id = -1;
	getIPAddress = false;
	wifi_signal = 0;
	firstload = true;
	printer_busy_state = 0;
	// wifi_config_mode = 0;
	strcpy(ipAddress, MSG_UNKNOWN);		
	strcpy(apName, MSG_UNKNOWN);		
	strcpy(esp32_serial, MSG_UNKNOWN);
	strcpy(esp32_version, MSG_UNKNOWN);
	GetWifiInfo();
}

void WIFI_Manager::Update()
{
	gserial.Process();

	uint32_t nowtime = getcurrenttime();
	if (nowtime > wifi_loop_check)
	{	// check status screen show time type per second
		wifi_loop_check = nowtime + WIFI_CHECK_INTERVAL;

		gserial.SendByte(REG_PRINTER_STATE,  (uint8_t)wt_machineStatus);
		gserial.SendByte(REG_OCTOPRINT_STATE,  (uint8_t)wt_onlineprinting);
		gserial.SendByte(REG_SDCARD_STATE, lcd_sd_status);
		gserial.SendByte(REG_BUSY, printer_busy_state);
		ShowTemperature();

		if (getIPAddress) gserial.SendCmd(REG_WIFI_SIGNAL);

		if (wt_machineStatus == WT_STATUS::PRINTING || wt_machineStatus == WT_STATUS::PAUSE)
		{	// in printing stage
			uint8_t percent_current = card.percentDone();
			gserial.SendByte(REG_PRINTING_PERCENT, percent_current);
			duration_t elapsed = print_job_timer.duration();
			uint32_t elapse_time = elapsed.second();
			gserial.SendInt32(REG_PRINTING_ELAPSE, elapse_time);
			
			if (elapse_time > 0 && percent_current > 0)
			{
				uint32_t remain_time = elapse_time * 100;
				remain_time = remain_time  / percent_current;
				remain_time = remain_time - elapse_time;
				gserial.SendInt32(REG_PRINTING_REMAIN, remain_time > 0 ? remain_time : 0);
				// SERIAL_ECHOLN(remain_time);
			}
			else
			{
				gserial.SendInt32(REG_PRINTING_REMAIN, 0);
			}

			// for debug
			// SERIAL_ECHOPGM("send percent:");
			// SERIAL_ECHO(percent_current);
		}
		else
		{	// other state

		}
	}
}


void WIFI_Manager::ShowLog(const char* message)
{
	// if (screenCurrent == screenLevelbed || screenCurrent == screenRestoreSetting || screenCurrent == screenResumePrinting)
	{
        gserial.SendString(REG_MESSAGE, message);
	}
}

void WIFI_Manager::ShowLogPair(const char *format, ...)
{
	char buffer[50];
	va_list arg_ptr;
	va_start(arg_ptr, format); 

	ZERO(buffer);
	vsprintf(buffer, format, arg_ptr);

	va_end(arg_ptr);

	// if (screenCurrent == screenLevelbed || screenCurrent == screenRestoreSetting || screenCurrent == screenResumePrinting)
	// {
    //     gserial.SendString(REG_MESSAGE, buffer);
	// }
}

void WIFI_Manager::ShowTemperature(void)
{
    temp_nozzle0 = thermalManager.degHotend(0);
	#if HAS_HEATED_BED
	temp_bed = thermalManager.degBed();
	#endif

	if (lasttemp_nozzle0 != temp_nozzle0)
	{
		if (temp_nozzle0 >= 0)
			gserial.SendInt16(REG_NOZZLE0_CURRENT, temp_nozzle0);
		else
			gserial.SendInt16(REG_NOZZLE0_CURRENT, 0);
	}

    if (lasttemp_bed != temp_bed)
	{
		if (temp_bed >= 0)
			gserial.SendInt16(REG_BED_CURRENT, temp_bed);
		else
			gserial.SendInt16(REG_BED_CURRENT, 0);
	}

	lasttemp_nozzle0 = temp_nozzle0;
	lasttemp_bed = temp_bed;
}

void WIFI_Manager::PayloadProcess(uint16_t addr, uint8_t *data, uint8_t data_length)
{
	if (addr == ADDR_WIFI_IP)
	{	// wifi ip
		ZERO(ipAddress);
        uint8_t _len = strlen((const char*)data);
        strncpy(ipAddress, (const char*)data, _len < STRLEN_IPADDRESS ? _len : STRLEN_IPADDRESS);
		if (_len > 5)
		{
			if (!getIPAddress)
			{
				getIPAddress = true;
				gserial.SendString(REG_DEVICE_VERSION, SHORT_BUILD_VERSION);
				gserial.SendString(REG_HARDWARE_VERSION, HARDWARE_VERSION);
			}
		}
		else
		{
			getIPAddress = false;
		}

		lcd_setstatus(ipAddress);

		if (firstload)
		{
			firstload = false;
		}
	}
	else if (addr == ADDR_WIFI_AP)
	{	// wifi ap name
		ZERO(apName);
        uint8_t _len = strlen((const char*)data);
        strncpy(apName, (const char*)data, _len < STRLEN_APNAME ? _len : STRLEN_APNAME);
	}
	else if (addr == ADDR_WIFI_CLOUD)
	{	// clound status
		if (data_length == 1)
		    tbState = data[0];
	}
	else if (addr == ADDR_WIFI_VERSION)
	{	// wifi firmware version
		ZERO(esp32_version);
        uint8_t _len = strlen((const char*)data);
        strncpy(esp32_version, (const char*)data, _len < STRLEN_VERSION ? _len : STRLEN_VERSION);
	}
	else if (addr == ADDR_WIFI_SERIAL)
	{	// wifi serial
		ZERO(esp32_serial);
        uint8_t _len = strlen((const char*)data);
        strncpy(esp32_serial, (const char*)data, _len < STRLEN_SERIAL ? _len : STRLEN_SERIAL);
	}
	else if (addr == ADDR_WIFI_MESSAGE)
	{	// wifi message
		ZERO(esp32_message);
        uint8_t _len = strlen((const char*)data);
        strncpy(esp32_message, (const char*)data, _len < STRLEN_MESSAGE ? _len : STRLEN_MESSAGE);
        esp32_message_id++;
        if (esp32_message_id > 1000) esp32_message_id = 0;

		
	}
	else if (addr == ADDR_ERROR_MESSAGE)
	{	// wifi error message
		ZERO(WifiErrorMessage);
        uint8_t _len = strlen((const char*)data);
        strncpy(WifiErrorMessage, (const char*)data, _len < STRLEN_MESSAGE ? _len : STRLEN_MESSAGE);
		wtlcd_ShowWifiErrorScreen();
	}
	else if (addr == ADDR_WIFI_SIGNAL)
	{	// wifi signal
		if (data_length == 1)
		    wifi_signal = data[0];
	}
	else if (addr == ADDR_FILE_LIST)
	{	// send file list
		FileListBegin();
		card.lsRoot();
		FileListEnd();
	}
	else if (addr == ADDR_FILE_DELETE)
	{	// delete sdcard file
		if (card.flag.mounted)
		{
			SERIAL_ECHOLNPAIR("file delete:", (const char*)data);
			card.removeFile((const char*)data);
			FileListBegin();
			card.lsRoot();
			FileListEnd();
		}
		else
			SERIAL_ECHOLN("error: sdcard not ready");
	}
	else if (addr == ADDR_PRINT_START)
	{	// print sdcard file
		if (card.flag.mounted)
		{
			if (card.checkFileExist((char*)data))
			{
				gcodeinfo.parse((char*)data);
				card.openAndPrintFile(gcodeinfo.info.filename);
				lcd_return_to_status();
				lcd_redraw_now();
				lcd_reset_status();
				wt_machineStatus = WT_STATUS::PRINTING;
				wifi_manager.isBusy(true);
				wifi_manager.SendFileDetail();
			}
			else
			{
				SERIAL_ECHOLNPAIR("open file fail: ", (const char*)data);
			}
		}
		else
			SERIAL_ECHOLN("error: sdcard not ready");

		
	}
	else if (addr == ADDR_PRINT_PAUSE)
	{	// print pause
		lcd_sdcard_pause();
	}
	else if (addr == ADDR_PRINT_RESUME)
	{	// print resume
		lcd_sdcard_resume();
	}
	else if (addr == ADDR_PRINT_CANCEL)
	{	// print cancel
		lcd_sdcard_stop();
	}
	else if (addr == ADDR_PRINT_GCODE)
	{	// run a gcode
		queue.enqueue_one_now((const char*)data);
	}
	else if (addr == ADDR_GLOBAL_REBOOT)
	{	// reboot the machine
		wt_restart();
	}
	else if (addr == ADDR_GLOBAL_VERSION)
	{	// send firmeare version
		gserial.SendString(REG_DEVICE_VERSION, SHORT_BUILD_VERSION);
		gserial.SendString(REG_HARDWARE_VERSION, HARDWARE_VERSION);
	}
	else if (addr == ADDR_GLOBAL_MOUNT_SD)
	{	// motor module load sdcard
		wt_load_sd();
	}
	else if (addr == ADDR_GLOBAL_UNMOUNT_SD)
	{	// motor module unload sdcard
		wt_unload_sd();
	}
	else if (addr == ADDR_GLOBAL_BOOTLOADER)
	{	// reboot to bootloader
		wt_bootload();
	}
	else if (addr == ADDR_DOWNLOAD_START)
	{	// goto download menu
		if (strlen((const char*)data) <= FILENAME_LENGTH)
			strcpy(downloadFilename, (const char*)data);
		else
			strncpy(downloadFilename, (const char*)data, FILENAME_LENGTH);
		
		download_state = 0;
		wt_machineStatus = WT_STATUS::DOWNLOADING;
		sprintf(downloadPercent, PERCENT_STRING, 0);
		wtlcd_ShowUploadingScreen();
	}
	else if (addr == ADDR_DOWNLOAD_PERCENT)
	{	// show download percent
		if (data_length == 1)
		{
			sprintf(downloadPercent, PERCENT_STRING, data[0]);
			// SERIAL_ECHOLNPAIR("download percent: ", downloadPercent);
		}
	}
	else if (addr == ADDR_DOWNLOAD_END)
	{	// download end
		download_state = 1;
		wt_machineStatus = WT_STATUS::IDLE;
	}
	else if (addr == ADDR_DOWNLOAD_FAIL)
	{	// download fail
		download_state = 2;
		wt_machineStatus = WT_STATUS::ERROR;
	}
	else if (addr == ADDR_GLOBAL_OPTION)
	{	// 查询打印机设置参数
		gserial.SendByte(REG_OPTION_LED, wtvar_led_state);
		gserial.SendByte(REG_OPTION_SILENCE, wtvar_quiet_mode);
		gserial.SendFloat(REG_ZOFFSET, probe.offset.z);
	}
	else if (addr == ADDR_OPTION_LED)
	{	// 设置LED开关
		if (data_length == 1)
		{
			wt_led_set(data[0]);
		}
	}
	else if (addr == ADDR_OPTION_SILENCE_MODE)
	{	// 设置LED开关
		if (data_length == 1)
		{
			wt_quiet_mode(data[0]);
		}
	}
	else if (addr == ADDR_GLOBAL_ENDSTOP)
	{
		uint8_t xmax, ymin, zmin, zmax, estate = 0;

		xmax = READ(X_MAX_PIN);
		ymin = !READ(Y_MIN_PIN);
		zmin = !READ(Z_MIN_PIN);
		zmax = READ(Z_MAX_PIN);

		estate = (xmax << 6) + (ymin << 5) + (zmin << 3) + (zmax << 2);

		gserial.SendByte(REG_ENDSTOP_STATE, estate);
	}
	else if (addr == ADDR_OPTION_ZOFFSET)
	{   // zoffset偏置
        if (data_length == 4)
		{
		    probe.offset.z = str2float(data);
			(void)settings.save();
		}
	}
	else if (addr == ADDR_GLOBAL_PARAMS)
	{
		wt_send_params();
	}
	else if (addr == ADDR_GLOBAL_RESET)
	{
		lcd_factory_settings();
	}
}

void WIFI_Manager::GetWifiInfo(void)
{
	gserial.SendCmd(REG_WIFI_IP);
	gserial.SendCmd(REG_WIFI_AP);
	gserial.SendCmd(REG_WIFI_SERIAL);
	gserial.SendCmd(REG_WIFI_CLOUD);
	gserial.SendCmd(REG_WIFI_VERSION);
	gserial.SendCmd(REG_WIFI_READ_MODE);
}

void WIFI_Manager::SetSDState(uint8_t state)
{
	gserial.SendByte(REG_SDCARD_STATE, state);
}

void WIFI_Manager::Set_AP_SSID(const char * ssid)
{
	ZERO(wifi_ssid);
	uint8_t _len = strlen((const char*)ssid);
    strncpy(wifi_ssid, (const char*)ssid, _len < STRLEN_SSID - 1 ? _len : STRLEN_SSID - 1);
}

void WIFI_Manager::Set_AP_PWD(const char * pwd)
{
	ZERO(wifi_pwd);
	uint8_t _len = strlen((const char*)pwd);
    strncpy(wifi_pwd, (const char*)pwd, _len < STRLEN_SSID - 1 ? _len : STRLEN_SSID - 1);
}

void WIFI_Manager::AP_Join(void)
{
	gserial.SendString(REG_WIFI_SSID, wifi_ssid);
	gserial.SendString(REG_WIFI_PWD, wifi_pwd);
	gserial.SendCmd(REG_WIFI_JOIN);
}

void WIFI_Manager::isBusy(bool state)
{
	gserial.SendByte(REG_BUSY, state ? 1: 0);
	printer_busy_state = state;
}

void WIFI_Manager::ResetWifiConfig(void)
{
	gserial.SendCmd(REG_WIFI_RESET);
}

void WIFI_Manager::FileListBegin(void)
{
	gserial.SendCmd(REG_FILE_BEGIN);
	safe_delay(50);
}

void WIFI_Manager::FileListEnd(void)
{
	safe_delay(100);
	gserial.SendCmd(REG_FILE_END);
}
	
void WIFI_Manager::FIleListSend(const char* line)
{
	gserial.SendString(REG_FILE_NAME, line);
}

void WIFI_Manager::Retart(void)
{
	gserial.SendCmd(REG_WIFI_RESTART);
}

void WIFI_Manager::LCD_TF_Update(void)
{
	wt_unload_sd();
	gserial.SendCmd(REG_WIFI_TF_UPDATE);
}


void WIFI_Manager::SendFileDetail(void)
{
	gserial.SendString(REG_PRINTING_FILENAME, gcodeinfo.info.filename);
	gserial.SendString(REG_JOBINFO_MACHINE, gcodeinfo.info.machine);
	gserial.SendString(REG_JOBINFO_MATERIAL, gcodeinfo.info.material);
	gserial.SendString(REG_JOBINFO_LAYER_HEIGHT, gcodeinfo.info.layerheight);
	gserial.SendString(REG_JOBINFO_DENSITY, gcodeinfo.info.density);
	gserial.SendInt32(REG_JOBINFO_TOTAL_TIME, gcodeinfo.info.i_totaltime);
}

void WIFI_Manager::LogReset(void)
{
	gserial.SendCmd(REG_LOG_RESET);
	safe_delay(200);
}

void WIFI_Manager::ShowErrorMessage(const char* msg)
{
	gserial.SendString(REG_ERROR_MSG, msg);
}