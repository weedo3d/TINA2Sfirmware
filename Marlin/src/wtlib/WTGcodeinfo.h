/**
* Copyright (C) 2022 WEEDO3D Perron
*/

#pragma once

#ifndef __WT_GCODE_INFO_H
#define __WT_GCODE_INFO_H

#define MAX_COMMENT_LINE_LENGTH		100
#define PIC_MAX_LINE			300
#define NOPIC_MAX_LINE			30


typedef struct 
{
	char machine[20];		// �������
	char material[10];		// �Ĳ�����
	char layerheight[10];	// ��ӡ���
	char density[10];		// �����
	char temp0[10];			// ��ͷ0�¶�
	char totaltime[10];		// ��ӡʱ�� xxhxxm
	char filament[10];		// ʹ����˿ xxxg
	float f_filament;		// �����ʽ��˿����
	uint32_t i_totaltime;	// ���θ�ʽ��ӡʱ��
	char filename[27];		// �ļ���
	bool b_image;			// �Ƿ����ͼƬ
    bool b_multi;           // �Ƿ�Ϊ˫��ͷģ��
	uint8_t i_line;			// ͼƬָ������
	uint32_t i_lefttime;	// ���θ�ʽ��ӡʣ��ʱ��
    float x_min, x_max, y_min, y_max, z_min, z_max;
	bool machineMatch;		// ����ƥ��
} wt_gcode_info_t;

class WTGcodeinfo
{
public:
	WTGcodeinfo() {};

	// ��ȡ������gcode�ļ�
	void parse(char* filename);

	wt_gcode_info_t info;


private:
	char linebuffer[MAX_COMMENT_LINE_LENGTH];
	uint16_t sd_count;

	void reset(void);
	void getvalue(char* data);
};

extern WTGcodeinfo gcodeinfo;

#endif
