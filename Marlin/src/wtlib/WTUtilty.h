#ifndef WT_UTILTY_H
#define WT_UTILTY_H

#include <stdbool.h>

bool startsWith(const char *str1, const char *str2);

uint16_t hexStringConvert(const char *hexStr, uint8_t *pBinArray);

// 解析长字符串中指定字符串对应的值
uint8_t extractFront(const char* source, char* ident, char* valString);

// utf8转unicode
uint16_t Utf8ToUnicode(const char* pInput, char* pOutput);

// unicode转utf8
uint16_t UnicodeToUtf8(uint8_t* pInput, uint8_t* pOutput, uint16_t inputlen, uint16_t outbufferlen);

// 版本比较
bool VersionCompare(char* oldVersion, char* newVersion);

uint32_t str2uint32(const uint8_t* str);

uint16_t str2uint16(const uint8_t* str);

float str2float(const uint8_t* str);

#endif