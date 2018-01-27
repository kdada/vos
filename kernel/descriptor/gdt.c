#include "gdt.h"
#include "../asm/func.h"

typedef struct {
    Word limitL;
    Word baseL;
    Byte baseM;
    Byte access;
    Byte flags;
    Byte baseH;
} __attribute__((packed)) GDTEntry;

typedef struct {
    Word limit;
    QuadWord base;
} __attribute__((packed)) GDTRegister;

const Byte GDTAccessPrivlegeLow = 0 << 5;
const Byte GDTAccessPrivlegeHigh = 3 << 5;
const Byte GDTAccessExecutable = 1 << 3;
const Byte GDTAccessConfirming = 1 << 2;
const Byte GDTAccessUsable = 1 << 1;

const Byte GDTFlagLongMode = 1 << 5;

const Byte GDTNull = 0;
const Byte GDTKernelCode = 8;
const Byte GDTKernelData = 16;

GDTEntry entries[3] = {{0}};
GDTRegister reg;

void setEntry(Byte i, Byte access, Byte flags) {
    GDTEntry *entry = &entries[i / 8];
    entry->access = access | 0x90;
    entry->flags = flags;
}

// 初始化 GDT
void InitGDT() {
    setEntry(GDTKernelCode,
             GDTAccessPrivlegeLow | GDTAccessExecutable | GDTAccessUsable,
             GDTFlagLongMode);
    setEntry(GDTKernelData, GDTAccessPrivlegeLow | GDTAccessUsable, 0);
    reg.limit = sizeof(entries) - 1;
    reg.base = (QuadWord)&entries[0];
    LoadGDTRegister((Byte *)&reg);

    /* SetCS(GDTKernelCode); */
    SetSS(GDTKernelData);
    SetDS(GDTKernelData);
    SetES(GDTKernelData);
    SetFS(GDTKernelData);
    SetGS(GDTKernelData);
}
