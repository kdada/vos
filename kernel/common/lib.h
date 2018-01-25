//公共库方法
#include "type.h"

#ifndef HFCommonLib
#define HFCommonLib

// 整数转换为字符串
Char *LongToString(Long value);

// 整数转换为十六进制字符串
Char *QuadWordToHex(QuadWord value);

// 字符串转换为整数
Long StringToLong(Char *value);

// 十六进制字符串转换为整数
QuadWord HexToQuadWord(Char *value);

#endif
