/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#include "../ultralcd.h"
#include "../qrcode.h"

// wifi设备信息界面
void wtlcd_ESP32InfoMenu()
{
	if (use_click())
		return lcd_goto_previous_menu();

	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_NETWORK_INFO[wtvar_language], true, true);
	STATIC_ITEM_MULTI(MMSG_INFO_VERSION[wtvar_language], false, false, wifi_manager.esp32_version);
	STATIC_ITEM("SN: ", false, false, wifi_manager.esp32_serial);
	STATIC_ITEM("IP: ", false, false, wifi_manager.ipAddress);
	STATIC_ITEM("AP: ", false, false, wifi_manager.apName);

	if (wifi_manager.tbState == 0)
	{
		STATIC_ITEM("CLOUD: ", false, false, PSTR("UNKNOWN"));
	}
	else if (wifi_manager.tbState == 1)
	{
		STATIC_ITEM("CLOUD: ", false, false, PSTR("UNREGISTERED"));
	}
	else if (wifi_manager.tbState == 2)
	{
		STATIC_ITEM("CLOUD: ", false, false, PSTR("DISCONNECT"));
	}
	else
		STATIC_ITEM("CLOUD: ", false, false, PSTR("CONNECTED"));

	END_SCREEN();
}

// OTA界面
extern bool isOTAUpdating;
void wtlcd_WifiOTAMenu()
{
	if (use_click())
	{
		isOTAUpdating = false;
		return lcd_return_to_status();
	}

	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_NETWORK_OTA_MENU[wtvar_language], true, false);
	STATIC_ITEM_MULTI(MMSG_NETWORK_OTA_1[wtvar_language], false, false);
	STATIC_ITEM_MULTI(MMSG_NETWORK_OTA_2[wtvar_language], false, false);
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
	wt_single_line_message(4, wifi_manager.esp32_message);

	END_SCREEN();
}

// 载入OTA界面
void wtlcd_WifiOTA()
{
	WT_OTA_Default();
	isOTAUpdating = true;
	menu_action_submenu(wtlcd_WifiOTAMenu);
}

// 显示qrcode错误信息
void wtlcd_Qrcode_ErrorMenu()
{
	if (use_click())
		return lcd_goto_previous_menu();

	START_SCREEN();
	STATIC_ITEM(" ", false, false);
	STATIC_ITEM_MULTI(MMSG_NETWORK_ERROR[wtvar_language], true, false);
	END_SCREEN();
}

QRCode qrcode;
uint8_t qrcodeData[106];
static uint8_t qrcode_pixel = 2;

// 显示二维码
void wtlcd_Qrcode_ShowMenu()
{
	if (use_click())
		return lcd_goto_previous_menu();

	uint8_t x, y, qr_x, qr_y;
	uint8_t left = (LCD_PIXEL_WIDTH - qrcode.size * qrcode_pixel) >> 1;
	uint8_t top = (LCD_PIXEL_HEIGHT - qrcode.size * qrcode_pixel) >> 1;
	uint8_t right = left + qrcode.size * qrcode_pixel;
	uint8_t bottom = top + qrcode.size * qrcode_pixel;
	uint8_t _draw = 1;

    u8g.firstPage();
    do {
      for (y = 0; y < LCD_PIXEL_HEIGHT; y++)
      {
        for (x = 0; x < LCD_PIXEL_WIDTH; x++)
        {
			_draw = 1;
			if (x >= left && x < right && y >= top && y < bottom)
			{
				qr_x = (x - left) >> 1;
				qr_y = (y - top) >> 1;
				if (qrcode_getModule(&qrcode, qr_x, qr_y))
					_draw = 0;
			}

			if (_draw)
				u8g.drawPixel(x, y);
        }
      }
    } while (u8g.nextPage());
}

// 载入设备二维码
void wtlcd_LoadQrcode()
{
	if (strlen(wifi_manager.esp32_serial) > 7)
	{	// generation qrcode from esp32serial
	    char qrstr[18] = {0};
		strncpy(qrstr, wifi_manager.esp32_serial, 14);
		strncpy(&qrstr[14], HARDWARE_VERSION, 4);
		qrcode_initText(&qrcode, qrcodeData, 3, 0, qrstr);
		menu_action_submenu(wtlcd_Qrcode_ShowMenu);
	}
	else
	{	// wifi device not found
		menu_action_submenu(wtlcd_Qrcode_ErrorMenu);
	}
}

void wtlcd_wifi_Reset_Menu()
{
	if (use_click())
		return lcd_goto_previous_menu();

	START_SCREEN();
	STATIC_ITEM(" ", false, false);
	STATIC_ITEM_MULTI(MMSG_NETWORK_RESET_LINE1[wtvar_language], false, false);
	STATIC_ITEM_MULTI(MMSG_NETWORK_RESET_LINE2[wtvar_language], false, false);
	END_SCREEN();
}

void wtlcd_Load_wifi_Reset()
{
	wifi_manager.ResetWifiConfig();
	menu_action_submenu(wtlcd_wifi_Reset_Menu);
}

void wtlcd_Load_ESP32InfoMenu()
{
	wifi_manager.GetWifiInfo();
	menu_action_submenu(wtlcd_ESP32InfoMenu);
}

// wifi功能菜单
void wtlcd_WifiMenu()
{
	START_MENU();
	MENU_BACK_MULTI(MMSG_CONTROL[wtvar_language]);

	MENU_ITEM_MULTI(function, MMSG_NETWORK_INFO[wtvar_language], wtlcd_Load_ESP32InfoMenu);
	MENU_ITEM_MULTI(function, MMSG_NETWORK_QRCODE[wtvar_language], wtlcd_LoadQrcode);
	MENU_ITEM_MULTI(function, MMSG_NETWORK_RESET[wtvar_language], wtlcd_Load_wifi_Reset);
	// MENU_ITEM_MULTI(submenu, MMSG_NETWORK_OTA[wtvar_language], wtlcd_WifiOTA);

	END_MENU();
}

void wtlcd_loadWifiMenu()
{
	wifi_manager.GetWifiInfo();
	menu_action_submenu(wtlcd_WifiMenu);
}

// 正在下载界面
extern char downloadFilename[FILENAME_LENGTH];
extern char downloadPercent[5];
void wtlcd_UploadingScreen()
{
	if (use_click())
	{
		if(download_state == 1)
			return lcd_return_to_status();

		if(download_state == 2)
		{
			wifi_manager.Retart();
			safe_delay(50);
			wt_restart();
		}
	} 
		
	START_SCREEN();
	// STATIC_ITEM(" ", false, false);
	if (download_state == 0)
	{
		STATIC_ITEM_MULTI(MMSG_UPLOADING_1[wtvar_language], false, false);
	}
	else if (download_state == 1)
	{
		STATIC_ITEM_MULTI(MMSG_UPLOADING_OK[wtvar_language], false, false);
	}
	else if (download_state == 2)
	{
		STATIC_ITEM_MULTI(MMSG_UPLOADING_FAIL[wtvar_language], false, false);
	}
	STATIC_ITEM(" ", false, false);
	STATIC_ITEM(" ", false, false);
	wt_single_line_message(2, downloadFilename);
	wt_single_line_message(3, downloadPercent);

	END_SCREEN();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

// 载入正在下载界面
void wtlcd_ShowUploadingScreen()
{
	menu_action_submenu(wtlcd_UploadingScreen);
	lcd_redraw_now();
}

extern char WifiErrorMessage[50];

void wtlcd_ErrorMessageScreen()
{
	if (use_click())
	{
		return lcd_return_to_status();
	} 
		
	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_ERROR[wtvar_language], false, false);
	STATIC_ITEM(" ", false, false);
	STATIC_ITEM(" ", false, false);
	wt_single_line_message(2, WifiErrorMessage);

	END_SCREEN();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

// 显示wifi错误信息界面
void wtlcd_ShowWifiErrorScreen()
{
	menu_action_submenu(wtlcd_ErrorMessageScreen);
	lcd_redraw_now();
}

