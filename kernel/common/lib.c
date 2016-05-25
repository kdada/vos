#include "lib.h"

Char tempArea[100];
Char hexString[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// 整数转换为字符串
Char* LongToString(Long value){
    if (value == 0) {
        return "0";
    }
    Byte pos = sizeof(tempArea)-1;
    tempArea[pos] = 0;
    --pos;
    Bool minus = false;
    if (value < 0) {
        minus = true;
        value = -value;
    }
    while (value > 0) {
        tempArea[pos] = value % 10 + '0';
        --pos;
        value /= 10;       
    }
    if (minus) {
        tempArea[pos] = '-';
        --pos;
    }
    ++pos;
    return tempArea + pos;
}

// 整数转换为十六进制字符串
Char* QuadWordToHex(QuadWord value){
    if (value == 0) {
        return "0";
    }
    Byte pos = sizeof(tempArea)-1;
    tempArea[pos] = 0;
    --pos;
    while (value > 0) {
        tempArea[pos] = hexString[value % 16];
        --pos;
        value /= 16;       
    }
    ++pos;
    return tempArea + pos;
}

// 字符串转换为整数
Long StringToLong(Char* value){
    Long v = 0;
    Bool minus = false;
    if (*value == '-') {
        minus = true;
        ++value;
    }
    while (*value != 0 && *value <= '9' && *value >= '0') {
        v *= 10;
        v += *value - '0';
        ++value;
    }
    if (*value != 0) {
        v = 0;
    }
    v *= minus?-1:1;
    return v;
}

// 十六进制字符串转换为整数
QuadWord HexToQuadWord(Char* value){
    QuadWord v = 0;
    while (*value != 0) {
        Char t = *value;
        if (t >='0' && t <= '9') {
            t -= '0';
        } 
        t &= 0xef;
        if (t >= 'A' && t <= 'F') {
            t = t - 'A' + 10;
        } else {
            break;
        }
        v = v * 16 + t;
        ++value;
    }
    if (*value != 0) {
        v = 0;
    }
    return v;
}