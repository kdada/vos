#include "print.h"

void PrintString(char *str){
    Word *memAddr = (Word*)(QuadWord)0xb8000;//字符模式输出地址
    Word data = 0x0f00;
    while (*str != 0) {
        data &= 0xff00;
        data |= *str;
        *memAddr = data;
        ++memAddr;
        ++str;
    }
}

void ClearScreen(){
    Word *memAddr = (Word*)(QuadWord)0xb8000;//字符模式输出地址
    Word data = 0x0020;
    Word count = 80*25;
    while (count--) {
        *memAddr = data;
        ++memAddr;
    }
}