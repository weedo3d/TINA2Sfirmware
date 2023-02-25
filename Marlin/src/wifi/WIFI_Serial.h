/**
* Copyright (C) 2022 WEEDO3D Perron
*/

/*
* WIFI 串口对象
*/


#ifndef WIFI_SERIAL_H
#define WIFI_SERIAL_H

#include "WIFI_ADDR.h"

class WIFI_Serial
{
public:
	WIFI_Serial();

	static void Process();
	static void SendString(uint16_t addr, const char* str);
	static void SendInt16(uint16_t addr, uint16_t data);
	static void SendInt32(uint16_t addr, uint32_t data);
    static void SendByte(uint16_t addr, uint8_t data);
    static void SendCmd(uint16_t addr);
    static void SendPosition(uint16_t addr, float x, float y, float z);
    static void SendFloat(uint16_t addr, float data);

private:

};

extern WIFI_Serial gserial;

#endif