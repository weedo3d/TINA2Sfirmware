/**
* Copyright (C) 2022 WEEDO3D Perron
 */

#ifndef WIFI_ADDR_H
#define WIFI_ADDR_H

// 主板->WIFI模块指令格式 0xFFAA, data
#define REG_WIFI_IP                 0xFFA0  // 查询IP
#define REG_WIFI_AP                 0xFFA1  // 查询AP
#define REG_WIFI_CLOUD              0xFFA2  // 查询云平台状态
#define REG_WIFI_VERSION            0xFFA3  // 查询版本
#define REG_WIFI_RESTART            0xFFA4  // WIFI模块重启
#define REG_WIFI_RESET              0xFFA5  // WIFI模块恢复出厂设置
#define REG_WIFI_OTA                0xFFA8  // WIFI模块固件在线更新
#define REG_MOTOR_OTA               0xFFA9  // 主板固件在线更新
#define REG_WIFI_SERIAL             0xFFAA  // 查询序列号
#define REG_WIFI_SIGNAL             0xFFAB  // 查询WIFI强度
#define REG_WIFI_READ_MODE          0xFFAC  // 查询WIFI配网模式
#define REG_WIFI_SET_MODE           0xFFAD  // uint8_t, 设置WIFI配网模式, 0 blufi, 1 softap
#define REG_WIFI_TF_UPDATE          0xFFAE  // WIFI固件从TF卡更新

#define REG_WIFI_SSID               0xFFB0  // char[64], WIFI网SSID
#define REG_WIFI_PWD                0xFFB1  // char[64], WIFI网password
#define REG_WIFI_JOIN               0xFFB2  // 连接指定WIFI网

#define REG_FILE_BEGIN              0xFFC0  // 文件列表开始
#define REG_FILE_NAME               0xFFC1  // char[50], 文件名
#define REG_FILE_END                0xFFC2  // 文件列表开始
#define REG_FILE_DELETE_RESULT      0xFFC3  // 文件删除结果

#define REG_SDCARD_STATE            0xFF30  // uint8_t, SD卡状态, 0, 未插入, 1, MOTOR控制, 2, WIFI控制
#define REG_NOZZLE0_HEATING         0xFF31  // uint8_t, 喷头0加热状态，0, 不加热, 1，加热, 2, 加热结束, 3, 超时
#define REG_NOZZLE1_HEATING         0xFF32  // uint8_t, 喷头1加热状态，0, 不加热, 1，加热, 2, 加热结束, 3, 超时
#define REG_BED_HEATING             0xFF33  // uint8_t, 热床加热状态，0, 不加热, 1，加热, 2, 加热结束, 3, 超时
#define REG_X_HOMED                 0xFF34  // uint8_t, X轴归位状态，0，未归位，1已归位, 2, 超时
#define REG_Y_HOMED                 0xFF35  // uint8_t, Y轴归位状态，0，未归位，1已归位, 2, 超时
#define REG_Z_HOMED                 0xFF36  // uint8_t, Z轴归位状态，0，未归位，1已归位, 2, 超时
#define REG_NOZZLE0_CURRENT         0xFF37  // uint16_t, 喷头0当前温度
#define REG_NOZZLE1_CURRENT         0xFF38  // uint16_t, 喷头1当前温度
#define REG_BED_CURRENT             0xFF39  // uint16_t, 热床当前温度
#define REG_NOZZLE0_TARGET          0xFF3A  // uint16_t, 喷头0目标温度
#define REG_NOZZLE1_TARGET          0xFF3B  // uint16_t, 喷头1目标温度
#define REG_BED_TARGET              0xFF3C  // uint16_t, 热床目标温度
#define REG_ENDSTOP_STATE           0xFF3D  // uint8_t, 状态板状态，每bit对应一个开关, 0, OFF, 1, ON, (7:0), X-min, x-max, y-min, y-max, z-min, z-max, runout0, runout1
#define REG_PRINTER_STATE           0xFF3E  // uint8_t， 打印机状态， 0, 空闲, 1, 打印, 2, 暂停, 3, 取消, 4, 完成, 5, ERROR, 6, 下载
#define REG_PRINTING_PERCENT        0xFF3F  // uint8_t, 打印进度, 0-100
#define REG_PRINTING_ELAPSE         0xFF40  // uint32_t， 已打印时间，单位秒
#define REG_PRINTING_REMAIN         0xFF41  // uint32_t, 剩余打印时间，单位秒
#define REG_PRINTING_SPEED          0xFF42  // uint16_t，打印速度，100为原速
#define REG_FILAMENT_OPERATION      0xFF43  // uint8_t, 料丝自动操作状态, 0, 加热, 1, 进丝, 2, 退丝, 3, 结束
#define REG_POSITION                0xFF44  // float * 3, 打印机当前坐标, x, y ,z
#define REG_MESSAGE                 0xFF45  // char[50], 显示消息
#define REG_ZOFFSET                 0xFF46  // float, zoffset偏置
#define REG_MOTOR_STATE             0xFF47  // uint8_t, 各电机开关状态, 每bit对应一个电机, 0, OFF, 1, ON, (7:0), X1, x2, Y1, Y2, Z1, Z2, E1, E2
#define REG_DEVICE_VERSION          0xFF49  // char[10], 电机模块固件版本
#define REG_HARDWARE_VERSION        0xFF4A  // char[10], 电机模块固件型号
#define REG_OPTION_DIAG             0xFF4B  // uint8_t, 故障诊断功能开关状态, 0, OFF, 1, ON
#define REG_OPTION_POWERLOSS        0xFF4C  // uint8_t, 断电续打功能开关状态, 0, OFF, 1, ON
#define REG_PLANNER_STATE           0xFF4D  // uint8_t, 运动管理器状态, 0, IDLE, 1, BUSY
#define REG_QUEUE_LENGTH            0xFF4E  // uint8_t, 指令缓冲区中剩余指令数量
#define REG_BUSY                    0xFF4F  // uint8_t, 运行中，请等待
#define REG_ERROR_MSG               0xFF50  // char[50], 错误消息
#define REG_CONFIG_SAVE_MSG         0xFF51  // 参数保存与恢复消息, 0, NONE, 1, open fail, 2, write fail, 3, save ok, 4, load ok
#define REG_OCTOPRINT_STATE         0xFF53  // uint8_t, octoprint联机状态, 0 - idle, 1 - pause, 2 - printing, 3 - lost
#define REG_PRINTING_FILENAME       0xFF54  // char[50], 文件名
#define REG_JOBINFO_MACHINE         0xFF55  // char[20]; JOB设备名
#define REG_JOBINFO_MATERIAL        0xFF56  // char[10]; JOB耗材类型
#define REG_JOBINFO_LAYER_HEIGHT    0xFF57  // char[10]; JOB打印层高
#define REG_JOBINFO_DENSITY         0xFF58  // char[10]; JOB填充率
#define REG_JOBINFO_TOTAL_TIME      0xFF59  // uint32_t, JOB总打印时间，单位秒
#define REG_OPTION_LED              0xFF5A  // uint8_t, LED开关状态, 0, 关, 1, 开
#define REG_OPTION_SILENCE          0xFF5B  // uint8_t, 静音模式,0, 关, 1, 开 
#define REG_PARAM_BEGIN             0xFF60  // 参数列表开始
#define REG_PARAM_DATA              0xFF61  // char[50], 参数行
#define REG_PARAM_END               0xFF62  // 参数列表结束
#define REG_LOG_RESET               0xFF63  // 清空日志记录

// WIFI模块->主板指令格式 0xFFAA, data
#define ADDR_WIFI_IP                0xEEA0  // char[16], IP地址
#define ADDR_WIFI_AP                0xEEA1  // char[32], AP名
#define ADDR_WIFI_CLOUD             0xEEA2  // uint8_t, 云平台状态, 0, UNKNOWN, 1, UNREGISTERED, 2, DISCONNECT, 3, CONNECTED
#define ADDR_WIFI_VERSION           0xEEA3  // char[20], WIFI固件版本
#define ADDR_WIFI_SERIAL            0xEEA4  // char[20], WIFI模块序列号
#define ADDR_WIFI_MESSAGE           0xEEA5  // char[50], WIFI模块发出信息
#define ADDR_WIFI_SIGNAL            0xEEA6  // uint8_t, WIFI信号强度
#define ADDR_WIFI_MODE              0xFEA7  // uint8_t, WIFI配网模式, 0 blufi, 1 softap
#define ADDR_WIFI_UPDATE_SCREEN     0xFEA8  // 显示WIFI固件更新窗口
#define ADDR_MOTOR_UPDATE_SCREEN    0xFEA9  // 显示电机固件更新窗口
#define ADDR_ERROR_MESSAGE          0xFEAA  // char[50], WIFI模块发出故障信息


#define ADDR_DOWNLOAD_START         0xEEB0  // 开始下载, char[50]
#define ADDR_DOWNLOAD_PERCENT       0xEEB1  // uint8_t, 下载进度
#define ADDR_DOWNLOAD_END           0xEEB2  // 下载完成
#define ADDR_DOWNLOAD_FAIL          0xEEB3  // 下载错误

#define ADDR_FILE_LIST              0xEEC0  // 显示文件列表
#define ADDR_FILE_DELETE            0xEEC1  // 删除文件, char[50]

#define ADDR_PRINT_START            0xEED0  // char[50], 打印文件
#define ADDR_PRINT_PAUSE            0xEED1  // 打印暂停
#define ADDR_PRINT_RESUME           0xEED2  // 打印继续
#define ADDR_PRINT_CANCEL           0xEED3  // 打印取消
#define ADDR_PRINT_GCODE            0xEED4  // char[96], GCODE

#define ADDR_GLOBAL_MOUNT_SD        0xEE01  // 电机模块加载SD卡
#define ADDR_GLOBAL_UNMOUNT_SD      0xEE02  // 电机模块卸载SD卡
#define ADDR_GLOBAL_BOOTLOADER      0xEE03  // 重启进入bootloader
#define ADDR_GLOBAL_REBOOT          0xEE04  // 主板重启
#define ADDR_GLOBAL_VERSION         0xEE05  // char[10], 主板固件版本
#define ADDR_GLOBAL_OPTION          0xEE06  // 查询打印机设置参数
#define ADDR_GLOBAL_ENDSTOP         0xEE07  // 查询endstop状态
#define ADDR_GLOBAL_PARAMS          0xEE08  // 查询固件参数
#define ADDR_GLOBAL_RESET           0xEE09  // 主板恢复出厂设置

#define ADDR_OPTION_LED             0xFE20  // uint8_t, 设置LED开关
#define ADDR_OPTION_SILENCE_MODE    0xFE21  // uint8_t, 设置静音模式
#define ADDR_OPTION_ZOFFSET         0xFF22  // float, 设置zoffset偏置

#endif
