#include "gdt.h"
#include "../asm/func.h"

typedef struct {
    Word limitL;
    Word baseL;
    Byte baseM;
    Byte access;
    Byte granularity;
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

const Byte GDTGranularityLongMode = 1 << 5;

const Byte GDTNull = 0;
const Byte GDTKernelCode = 8;
const Byte GDTKernelData = 16;

GDTEntry gdtEntries[3] = {{0}};
GDTRegister gdtReg;

void setGDTEntry(Byte i, Byte access, Byte granularity) {
    GDTEntry *entry = &gdtEntries[i / 8];
    entry->limitL = 0;
    entry->baseL = 0;
    entry->baseM = 0;
    entry->baseH = 0;
    if (access != 0) {
        access |= 0x90;
    }
    entry->access = access;
    entry->granularity = granularity;
}

// 初始化 GDT
void InitGDT() {
    setGDTEntry(GDTNull, 0, 0);
    setGDTEntry(GDTKernelCode,
                GDTAccessPrivlegeLow | GDTAccessExecutable | GDTAccessUsable,
                GDTGranularityLongMode);
    setGDTEntry(GDTKernelData, GDTAccessPrivlegeLow | GDTAccessUsable,
                GDTGranularityLongMode);
    gdtReg.limit = sizeof(gdtEntries) - 1;
    gdtReg.base = (QuadWord)&gdtEntries[0];
    LoadGDTRegister((Byte *)&gdtReg);

    SetSS(GDTKernelData);
    SetDS(GDTKernelData);
    SetES(GDTKernelData);
    SetFS(GDTKernelData);
    SetGS(GDTKernelData);
    /* SetCS(GDTKernelCode); */
}
