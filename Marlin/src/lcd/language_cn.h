/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Chinese
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_CN_H
#define LANGUAGE_CN_H

#define MAPPER_NON         // For direct ascii codes
#define DISPLAY_CHARSET_ISO10646_CN
//#define CHARSIZE 3

//#define WELCOME_MSG                         "\xa4\xa5\xa6\xa7"
//#define MSG_SD_INSERTED                     "\xa8\xa9\xaa\xab"			// 卡已插入
//#define MSG_SD_REMOVED                      "\xa8\xa9\xac\xad"			// 卡已拔出
//#define MSG_MAIN                            "\xae\xaf\xb0"
//#define MSG_AUTOSTART                       "\xb1\xb2\xb3\xb4"
//#define MSG_DISABLE_STEPPERS                "\xb5\xb6\xb7\xb8\xb9\xba"
//#define MSG_DEBUG_MENU                      _UxGT("Menu depurar")
#define MSG_PROGRESS_BAR_TEST               _UxGT("Prueba barra avance")
//#define MSG_AUTO_HOME                       "\xbb\xbc\xbd"
//#define MSG_AUTO_HOME_X                     _UxGT("Origen X")
//#define MSG_AUTO_HOME_Y                     _UxGT("Origen Y")
//#define MSG_AUTO_HOME_Z                     _UxGT("Origen Z")
//#define MSG_TMC_Z_CALIBRATION               "Calibrate Z"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Next Point")
#define MSG_LEVEL_BED_DONE                  "Leveling Done!"
//#define MSG_SET_HOME_OFFSETS                "\xbe\xbf\xbb\xbc\xbd\xc0\xc1"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets applied"
//#define MSG_SET_ORIGIN                      "\xbe\xbf\xbc\xbd"
#define MSG_PREHEAT_1                       "\xc3\xc4 PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " \xc5\xc6"
#define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" End")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " \xc4\xc7"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " \xbe\xbf"
#define MSG_PREHEAT_2                       "\xc3\xc4 ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " \xc5\xc6"
#define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" End")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " \xbe\xc6"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " \xbe\xbf"
//#define MSG_COOLDOWN                        "\xc8\xc9"
//#define MSG_SWITCH_PS_ON                    "\xb9\xcb\xca\xb3"
//#define MSG_SWITCH_PS_OFF                   "\xb9\xcb\xb5\xb6"
//#define MSG_EXTRUDE                         "\xcc\xad"
//#define MSG_RETRACT                         "\xbb\xcd"
//#define MSG_MOVE_AXIS                       "\xc1\xb2\xce"
//#define MSG_BED_LEVELING                    "\xcf\xe0\xc4\xc7"
//#define MSG_LEVEL_BED                       "\xb1\xb2\xcf\xd0"			// 自动调平
//#define MSG_MOVING                          _UxGT("Moviendo...")
#define MSG_FREE_XY                         _UxGT("Libre XY")
//#define MSG_MOVE_X                          "\xc1\xb2 X"
//#define MSG_MOVE_Y                          "\xc1\xb2 Y"
//#define MSG_MOVE_Z                          "\xc1\xb2 Z"
//#define MSG_MOVE_E                          "\xcc\xad\xba"
//#define MSG_MOVE_01MM                       "\xc1\xb2 0.1mm"
//#define MSG_MOVE_1MM                        "\xc1\xb2 1mm"
//#define MSG_MOVE_10MM                       "\xc1\xb2 10mm"
//#define MSG_SPEED                           "\xd1\xd2"
#define MSG_BED_Z                           _UxGT("Plataforma Z")
//#define MSG_NOZZLE                          "\xd3\xd4"
//#define MSG_BED                             "\xc4\xc7"
//#define MSG_FAN_SPEED                       "\xd5\xd6\xd1\xd2"
//#define MSG_FLOW                            "\xcc\xad\xd1\xd2"
//#define MSG_CONTROL                         "\xd8\xd9\xee\xef"
//#define MSG_MIN                             LCD_STR_THERMOMETER " \xda\xdb"
//#define MSG_MAX                             LCD_STR_THERMOMETER " \xda\xdc"
//#define MSG_FACTOR                          LCD_STR_THERMOMETER " \xdd\xde"
//#define MSG_AUTOTEMP                        "\xb1\xb2\xd8\xc9"
//#define MSG_ON                              "\xb3 "  // intentional space to shift wide symbol to the left
//#define MSG_OFF                             "\xb5 "  // intentional space to shift wide symbol to the left
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          _UxGT("Seleccionar")
#define MSG_ACC                             "Accel"
#define MSG_JERK                            "Jerk"
#if IS_KINEMATIC
  #define MSG_VA_JERK                       "Va-jerk"
  #define MSG_VB_JERK                       "Vb-jerk"
  #define MSG_VC_JERK                       "Vc-jerk"
#else
  #define MSG_VA_JERK                       "Vx-jerk"
  #define MSG_VB_JERK                       "Vy-jerk"
  #define MSG_VC_JERK                       "Vz-jerk"
#endif
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-retract"
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_STEPS_PER_MM                    "Steps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_E1STEPS                         "E1steps/mm"
#define MSG_E2STEPS                         "E2steps/mm"
#define MSG_E3STEPS                         "E3steps/mm"
#define MSG_E4STEPS                         "E4steps/mm"
#define MSG_E5STEPS                         "E5steps/mm"
//#define MSG_TEMPERATURE                     "\xc9\xd2"			// 温度
#define MSG_MOTION                          "\xdf\xb2"
#define MSG_FILAMENT                        "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_ADVANCE_K                       _UxGT("Avance K")
#define MSG_CONTRAST                        "LCD contrast"
#define MSG_STORE_EEPROM                    "Store memory"
#define MSG_LOAD_EEPROM                     "Load memory"
//#define MSG_RESTORE_FAILSAFE                "\xe7\xe8\xad\xe9\xbe\xbf"
#define MSG_REFRESH                         "Refresh"
//#define MSG_WATCH                           "\xec\xed\xee\xef"				// 信息界面 
//#define MSG_PREPARE                         "\xa4\xa5\xee\xef"
//#define MSG_TUNE                            "\xcf\xf0"
//#define MSG_PAUSE_PRINT                     "\xf1\xf2\xca\xf3"
//#define MSG_RESUME_PRINT                    "\xf4\xf5\xca\xf3"
//#define MSG_STOP_PRINT                      "\xf2\xf6\xca\xf3"
//#define MSG_CARD_MENU                       "SD\xa8\xee\xef"
//#define MSG_CANCEL_AUTOOP                   "\xf2\xf6\xcc\xad / \xbb\xcd"
//#define MSG_NO_CARD                         "\xf9\xa8"
#define MSG_DWELL                           "Sleep..."
#define MSG_USERWAIT                        "Wait for user..."
#define MSG_RESUMING                        "Resuming print"
#define MSG_PRINT_ABORTED                   "\xca\xf3\xf2\xf6"			//打印停止
#define MSG_NO_MOVE                         "No move."
#define MSG_KILLED                          "KILLED. "
#define MSG_STOPPED                         "STOPPED. "
#define MSG_CONTROL_RETRACT                 "Retract mm"
#define MSG_CONTROL_RETRACT_SWAP            "Swap Re.mm"
#define MSG_CONTROL_RETRACTF                "Retract  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Hop mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "S UnRet mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoRetr."
#define MSG_FILAMENTCHANGE                  "Change filament"
#define MSG_INIT_SDCARD                     "Init. SD card"
#define MSG_CNG_SDCARD                      "Change SD card"
#define MSG_ZPROBE_OUT                      "Z probe out. bed"
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Auto-Prueba")
#define MSG_BLTOUCH_RESET                   _UxGT("Reiniciar BLTouch")
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
//#define MSG_ZPROBE_ZOFFSET                  "\xbe\xbfZ\xce\xc0\xc1"	
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_HEATING_FAILED_LCD              "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 _UxGT("Error: Temp Min Plat")
#define MSG_ERR_Z_HOMING                    MSG_HOME _UxGT(" ") MSG_X MSG_Y _UxGT(" ") MSG_FIRST
#define MSG_HALTED                          _UxGT("PRINTER HALTED")
#define MSG_PLEASE_RESET                    _UxGT("Please reset")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_END_HOUR                        "hours"
#define MSG_END_MINUTE                      "minutes"
#define MSG_HEATING                         _UxGT("Heating...")
#define MSG_BED_HEATING                     _UxGT("Bed heating...")
#define MSG_DELTA_CALIBRATE                 "Delta Calibration"
#define MSG_DELTA_CALIBRATE_X               "Calibrate X"
#define MSG_DELTA_CALIBRATE_Y               "Calibrate Y"
#define MSG_DELTA_CALIBRATE_Z               "Calibrate Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Calibrate Center"
#define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Auto Calibracion")
#define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Est. Altura Delta")
#define MSG_INFO_MENU                       _UxGT("Inf. Impresora")
#define MSG_INFO_PRINTER_MENU               _UxGT("Inf. Impresora")
#define MSG_INFO_STATS_MENU                 _UxGT("Estadisticas Imp.")
#define MSG_INFO_BOARD_MENU                 _UxGT("Inf. Controlador")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistores")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extrusores")
#define MSG_INFO_BAUDRATE                   _UxGT("Baudios")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocolo")
#define MSG_CASE_LIGHT                      _UxGT("Luz cabina")
//#define MSG_OFFSET_Z						"\xbe\xbfZ\xce\xc0\xc1"				// 设置Z轴偏移
//#define MSG_SET_BEEP						"\xbe\xbf Beep"					// 设置Beep

//#define MSG_MACHINE_INFO					"\xbe\xa5\xec\xed     "			// 设备信息
#if LCD_WIDTH > 19
  #define MSG_INFO_PRINT_COUNT              _UxGT("Conteo de impresion")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completadas")
  #define MSG_INFO_PRINT_TIME               _UxGT("Tiempo total de imp.")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Impresion mas larga")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Total de Extrusion")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Impresiones")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completadas")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Mas larga")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extrusion")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Temperatura minima")
#define MSG_INFO_MAX_TEMP                   _UxGT("Temperatura maxima")
#define MSG_INFO_PSU                        _UxGT("Fuente de poder")
#define MSG_DRIVE_STRENGTH                  _UxGT("Potencia driver")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Escribe DAC EEPROM")
//#define MSG_INFO_MACHINE					"\xbe\xa5"						// 设备
//#define MSG_INFO_VERSION					"\x95\x96"						// 版本
//#define MSG_WIFI_CANCELPRINTING				"\xf2\xf6Wifi\xca\xf3"			// 停止Wifi打印
//#define MSG_FILAMENTIN                      "\xe4\xab\x92\x94     "				// 载入料丝
//#define MSG_FILAMENTOUT                     "\x93\xe4\x92\x94     "				// 卸载料丝
//#define MSG_ZPROBE_TITLE                   "Z\xce\xc0\xc1(mm)"				// 设置Z偏移

//#define	MSG_FILAMENT_CHANGE_HEADER_LOAD		"\xe4\xab\x92\x94     "
//#define MSG_FILAMENT_CHANGE_HEADER_UNLOAD	 "\x93\xe4\x92\x94     "
//#define MSG_DELTA_Z_OFFSET_CALIBRATE		MSG_OFFSET_Z
//#define MSG_FILAMENT_CHANGE_HEADER_PAUSE    "\xca\xf3\xf1\xf2     "			// 打印暂停
//#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "\xf4\xf5\xfe\xff     "			// 继续操作
//#define MSG_FILAMENT_CHANGE_OPTION_PURGE    "\xe4\xab\xe0\xe1\x92\x94"		// 载入更多料丝
//#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "\xf4\xf5"						// 继续
//#define MSG_FILAMENT_CHANGE_NOZZLE          " \xd3\xd4\xc9\xd2:"			// 喷嘴温度
//#define MSG_ERR_HOMING_FAILED               "\xbb\xbc\xbd\x9f\x9e"			// 回原点失败
//#define MSG_ERR_PROBING_FAILED              "\xb1\xb2\xcf\xd0\x9f\x9e"		// 自动调平失败

//#define MSG_FILAMENT_CHANGE_INIT_1          "\xfc\xfd\xb3\xb4     "			// 等待开始
//#define MSG_FILAMENT_CHANGE_INIT_2          "\x92\x94  "					// 料丝
//#define MSG_FILAMENT_CHANGE_INIT_3          "\xfe\xff  "					// 操作

//#define MSG_FILAMENT_CHANGE_UNLOAD_1        "\xfc\xfd  "					// 等待
//#define MSG_FILAMENT_CHANGE_UNLOAD_2         "\x93\xe4\x92\x94     "

//#define MSG_FILAMENT_CHANGE_INSERT_1        "\xaa\xab\x92\x94       "		// 插入料丝
//#define MSG_FILAMENT_CHANGE_INSERT_2        "\x8d\x8e\x8f\xf4\xf5       "	// 并按键
//#define MSG_FILAMENT_CHANGE_INSERT_3        "  "			

//#define MSG_FILAMENT_CHANGE_HEAT_1          "\x8e\x8f\xb3\xb4     "			// 按键
//#define MSG_FILAMENT_CHANGE_HEAT_2          "\xc3\xc4\xd3\xd4     "			// 预热喷嘴
//#define MSG_FILAMENT_CHANGE_HEATING_1       "\xc3\xc4\xd3\xd4     "			// 预热喷嘴
//#define MSG_FILAMENT_CHANGE_HEATING_2       "\xfc\xfd...    "				// 等待

//#define MSG_FILAMENT_CHANGE_LOAD_1          "\xfc\xfd  "					// 等待
//#define MSG_FILAMENT_CHANGE_LOAD_2          "\xe4\xab\x92\x94     "

//#define MSG_FILAMENT_CHANGE_PURGE_1         " "					
//#define MSG_FILAMENT_CHANGE_PURGE_2         "\xfc\xfd\xe4\xab   "				// 等待载入
//
//#define MSG_FILAMENT_CHANGE_RESUME_1        "\xfc\xfd  "						// 等待
//#define MSG_FILAMENT_CHANGE_RESUME_2        "\xe7\xe8\xca\xf3    "				// 继续打印

//#define MSG_WELCOME_CONINUE					"\xf4\xf5"
//#define MSG_WELCOME_SKIP					"\xb5\xb6"

//#define MSG_WELCOME_SCREEN_1				"Welcome to"
//#define MSG_WELCOME_SCREEN_2				"\x90 1 \xb7 \xe4\xab\x92\x94"		// 第1步 载入料丝
//#define MSG_WELCOME_SCREEN_3				"\x90 2 \xb7 \xaa\xabSD\xa8"			// 第2步 插入SD卡
//#define MSG_WELCOME_SCREEN_4				"\x90 3 \xb7 \xca\xf3SD\xa8\x8b\x8c"	// 第3步 打印SD卡文件
//#define MSG_WELCOME_MENU					"\xa2\xa3\xee\xef"						// 向导界面

//#define MSG_PRINT_PAUSED                    "\xca\xf3\xf1\xf2"					// 打印暂停
//#define MSG_PRINTING                        "\xca\xf3..."						// 打印

//#define MSG_NETWORK_MENU					"\x99\x9a\xbe\xbf"						// 网络设置
//#define MSG_NETWORK_IP						"\x97\x98\x99\x9aIP\x9b\x9c"			// 显示网络IP地址
//#define MSG_NETWORK_INFO					"\x97\x98Wifi\xbe\xa5\xec\xed"			// 显示Wifi设备信息
//#define MSG_NETWORK_CONFIG					"\xbe\xbfWifi\x99\x9a      "			// 设置wifi网络
//#define MSG_NETWORK_OTA						"Wifi\xbe\xa5OTA"						// Wifi设备OTA
//#define MSG_NETWORK_CONFIG_1				"\xca\xb3 APP \xb8\xabWIFI\xbe\xbf"		// 打开APP进入WIFI设置
//#define MSG_NETWORK_CONFIG_2				"  \x8a\xabWifi\x99\x9a\x88\x89"			// 输入WIFI网络密码
//#define MSG_NETWORK_CONFIG_3				"  \x9d\x85\x86\x87 5G Wifi"				// 不要使用5G Wifi
//#define MSG_NETWORK_CONFIG_4				" "
//#define MSG_NETWORK_OTA_1					"\x9d\x85\xb5\xb6\xb9\xcb"				// 不要关闭电源 
//#define MSG_NETWORK_OTA_2					" "

//#define MSG_UPLOADING_1						"\x82\x83\x84\xe4       "						// 正在下载
//#define MSG_UPLOADING_FAIL					"\x84\xe4\x9f\x9e       "						// 下载失败

#endif // LANGUAGE_CN_H
