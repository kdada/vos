#ifndef HFCommonType
#define HFCommonType

// 基本数据类型
#define true 1
#define false 0
#define null (void *)0
typedef unsigned char Bool;          // 1字节,布尔类型
typedef unsigned char Byte;          // 1字节,无符号整数类型
typedef char Char;                   // 1字节,有符号整数类型
typedef unsigned short Word;         // 2字节,无符号整数类型
typedef short Short;                 // 2字节,有符号整数类型
typedef unsigned int DoubleWord;     // 4字节,无符号整数类型
typedef int Int;                     // 4字节,有符号整数类型
typedef unsigned long long QuadWord; // 8字节,无符号整数类型
typedef long long Long;              // 8字节,有符号整数类型
typedef float Float;                 // 4字节,有符号浮点类型
typedef double Double;               // 8字节,有符号浮点类型

// 错误数据类型
typedef QuadWord ErrorCode; // 错误码类型

#endif
