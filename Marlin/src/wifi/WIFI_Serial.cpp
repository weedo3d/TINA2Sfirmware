/**
* Copyright (C) 2022 WEEDO3D Perron
*/

/*
* WTGL 串口对象
*/

#include "../MarlinCore.h"
#include "WIFI_Serial.h"
#include "../wtlib/WTPacket.h"
#include "../core/serial.h"

InPacket inp;       // 接收WTGL发来数据对象
OutPacket outp;     // 向WTGL发送数据包对象

void WIFI_Serial::SendString(uint16_t addr, const char* str)
{
	outp.Reset();
    outp.payload_append_int(addr);
    outp.payload_append_string(str);
    outp.Send();
}

void WIFI_Serial::SendInt16(uint16_t addr, uint16_t data)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.payload_append_int(data);
    outp.Send();
}

void WIFI_Serial::SendInt32(uint16_t addr, uint32_t data)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.payload_append_int32(data);
    outp.Send();
}

void WIFI_Serial::SendByte(uint16_t addr, uint8_t data)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.appendByte(data);
    outp.Send();
}

void WIFI_Serial::SendCmd(uint16_t addr)
{
    outp.Reset();
    outp.payload_append_int(addr);
    outp.Send();
}

void WIFI_Serial::SendPosition(uint16_t addr, float x, float y, float z)
{
    outp.Reset();
	outp.payload_append_int(addr);
	outp.payload_append_float(x);
	outp.payload_append_float(y);
	outp.payload_append_float(z);
	outp.Send();
}

void WIFI_Serial::SendFloat(uint16_t addr, float data)
{
    outp.Reset();
	outp.payload_append_int(addr);
	outp.payload_append_float(data);
	outp.Send();
}


WIFI_Serial::WIFI_Serial()
{

}

// 接收并处理串口数据
void WIFI_Serial::Process()
{
	uint8_t b;

    // 超过1s未完成接收数据，进行复位
    if (inp.State() != PS_START && inp.isTimeout())
    {
        inp.Reset();
    }

	while (WIFI_SERIAL.available() > 0)
	{
		b = (uint8_t)WIFI_SERIAL.read();
		inp.Receiver(b);
	}
}


