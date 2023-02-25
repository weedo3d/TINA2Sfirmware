/**
* Copyright (C) 2022 WEEDO3D Perron
*/

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "../MarlinCore.h"

#define WTGL_PAYLOAD_LENGTH		64
#define TestError

#define UNICODE_FILENAME_LENGTH	54

#define STRLEN_IPADDRESS		16
#define STRLEN_APNAME			32
#define STRLEN_VERSION			20
#define STRLEN_SERIAL			20
#define STRLEN_MESSAGE			50
#define STRLEN_SSID				64

typedef struct
{
	char buffer[UNICODE_FILENAME_LENGTH];
    uint8_t len;
}wt_fileinfo_t;

typedef struct 
{
	long printTime;
	long printTimeLeft;
	double completion;
	long filepos;
	long filesize;
	wt_fileinfo_t fileinfo;
	long date;
	uint16_t index;
	bool remote;			
} wt_job_info_t;

class WIFI_Manager
{
public:
	WIFI_Manager();

	void Init(void);
	void Update();
	void SetFilename(const char* fname);
	void SetPrintTime(const char* time);
	void ShowLog(const char* message);
	void ShowLogPair(const char *format, ...);
	void ShowTemperature(void);
    void PayloadProcess(uint16_t addr, uint8_t *data, uint8_t data_length);
	void GetWifiInfo(void);
	void SetSDState(uint8_t state);
	void Set_AP_SSID(const char * ssid);
	void Set_AP_PWD(const char * pwd);
	void AP_Join(void);
	void isBusy(bool state);
	void FileListBegin(void);
	void FileListEnd(void);
	void FIleListSend(const char* line);
	void Retart(void);
	void LCD_TF_Update(void);
	void ResetWifiConfig(void);
	void SendFileDetail(void);
	void LogReset(void);
	void ShowErrorMessage(const char* msg);

	bool busy = false;
	bool firstload = true;
    uint8_t currentID;
    wt_job_info_t jobinfo;
	char ipAddress[STRLEN_IPADDRESS];		// IP Address
	bool getIPAddress = false;				// Get IP Address
	char apName[STRLEN_APNAME];				// AP Name
	uint8_t tbState;						// 0, UNKNOWN, 1, UNREGISTERED, 2, DISCONNECT, 3, CONNECTED
	char esp32_version[STRLEN_VERSION];		// ESP32WIFI Firmware Version
	char esp32_serial[STRLEN_SERIAL];		// ESP32WIFI Serial
	char esp32_message[STRLEN_MESSAGE];		// ESP32WIFI Message
	int16_t esp32_message_id;				// ESP32WIFI Message ID
	uint8_t wifi_signal;					// ESP32WIFI Signal
	uint8_t printer_busy_state;						// 1, Printer is busy

private:
	void InitMenu();
	void reset();
	bool testingMode;
    int lasttemp_nozzle0 = -1;
	int lasttemp_bed = -1;
	uint16_t temp_nozzle0;
	uint16_t temp_bed;

};

extern WIFI_Manager wifi_manager;

#endif
