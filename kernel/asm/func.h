#include "../common/type.h"

#ifndef HFAsmFunc
#define HFAsmFunc

// 向指定端口(port)输出4字节的数据(data)
void PortOutWD(Word port, DoubleWord data);

// 从指定端口(port)获取4字节数据
DoubleWord PortInWD(Word port);

// 从指定地址(from)复制指定数量(count)的字节到另外一个地址(to)
void MemoryCopy(Byte *from, Byte *to, QuadWord count);

// 获取当前LME4T的基地址
Byte *GetLME4TBase();

// 设置当前LME4T的基地址
void SetLME4TBase(Byte *base);

// 获取当前栈指针
Byte *GetStackPointer();

// 设置 GDTR
void LoadGDTRegister(Byte *addr);

// 设置当前栈指针
void SetStackPointer(Byte *sp);

// 设置 CS
void SetCS(Word cs);

// 设置 SS
void SetSS(Word ss);

// 设置 DS
void SetDS(Word ds);

// 设置 ES
void SetES(Word es);

// 设置 FS
void SetFS(Word fs);

// 设置 GS
void SetGS(Word gs);

#endif
