#include "idt.h"
#include "../asm/func.h"
#include "../display/print.h"

typedef struct {
    Word offsetL;
    Word selector;
    Byte zero;
    Byte attribute;
    Word offsetM;
    DoubleWord offsetH;
    DoubleWord reserved;
} __attribute__((packed)) IDTEntry;

typedef struct {
    Word limit;
    QuadWord base;
} __attribute__((packed)) IDTRegister;

typedef struct {
    QuadWord ip;
    QuadWord cs;
    QuadWord flags;
    QuadWord sp;
    QuadWord ss;
} __attribute__((packed)) Frame;

const Byte IDTAttributePrivlegeLow = 0 << 5;
const Byte IDTAttributePrivlegeHigh = 3 << 5;
const Byte IDTAttributeTypeInterrupt = 0xE;
const Byte IDTAttributeTypeTrap = 0xF;

IDTEntry idtEntries[32] = {{0}};
IDTRegister idtReg;

void setIDTEntry(Byte i, QuadWord offset, Byte attribute) {
    IDTEntry *entry = &idtEntries[i / 8];
    entry->selector = 0x8;
    entry->attribute = attribute | 0x80;
    entry->offsetL = offset;
    entry->offsetM = offset >> 16;
    entry->offsetH = offset >> 32;
    entry->zero = 0;
    entry->reserved = 0;
}

void setInterrupt(Byte i, void offset(Frame *frame)) {
    setIDTEntry(i, (QuadWord)offset,
                IDTAttributePrivlegeLow | IDTAttributeTypeInterrupt);
}

__attribute__((interrupt)) __attribute__((target("no-sse,no-mmx"))) void
divideByZero(Frame *frame) {
    Print("Interrupt\n");
}

// 初始化 IDT
void InitIDT() {
    setInterrupt(0, divideByZero);
    setInterrupt(1, divideByZero);
    setInterrupt(2, divideByZero);
    setInterrupt(3, divideByZero);
    setInterrupt(4, divideByZero);
    setInterrupt(5, divideByZero);
    setInterrupt(6, divideByZero);
    setInterrupt(7, divideByZero);
    setInterrupt(8, divideByZero);
    setInterrupt(9, divideByZero);
    setInterrupt(10, divideByZero);
    setInterrupt(11, divideByZero);
    setInterrupt(12, divideByZero);
    setInterrupt(13, divideByZero);
    setInterrupt(14, divideByZero);
    setInterrupt(15, divideByZero);
    setInterrupt(16, divideByZero);
    setInterrupt(17, divideByZero);
    setInterrupt(18, divideByZero);
    setInterrupt(19, divideByZero);
    setInterrupt(20, divideByZero);
    setInterrupt(21, divideByZero);
    setInterrupt(22, divideByZero);
    setInterrupt(23, divideByZero);
    setInterrupt(24, divideByZero);
    setInterrupt(25, divideByZero);
    setInterrupt(26, divideByZero);
    setInterrupt(27, divideByZero);
    setInterrupt(28, divideByZero);
    setInterrupt(29, divideByZero);
    setInterrupt(30, divideByZero);
    setInterrupt(31, divideByZero);

    idtReg.limit = sizeof(idtEntries) - 1;
    idtReg.base = (QuadWord)&idtEntries[0];
    LoadIDTRegister((Byte *)&idtReg);
}
