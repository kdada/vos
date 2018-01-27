#include "asm/func.h"
#include "common/lib.h"
#include "common/type.h"
#include "descriptor/gdt.h"
#include "display/print.h"
#include "memory/page.h"

// 根据PCI地址获取数据 bus:8bit device:5bit function:3bit offset:6bit
DoubleWord PCIInfo(Byte bus, Byte device, Byte function, Byte offset) {
    DoubleWord addr = 0x80000000;
    addr |= ((DoubleWord)bus) << 16;
    addr |= ((DoubleWord)device) << 11;
    addr |= ((DoubleWord)function) << 8;
    addr |= offset << 2;
    PortOutWD(0xcf8, addr);
    addr = PortInWD(0xcfc);
    return addr;
}

// 内核启动函数
// 启动时, main 位于物理内存 0x100000(1Mi), 虚拟内存 0xffffffff00000000.
// memoryMap 是物理内存分段信息表，放置于物理内存的 0x0 - 0xfffff 范围内.
__attribute__((noreturn)) void main(MemoryMapBlock *memoryMap) {
    ClearScreen();
    for (Word bus = 0; bus < 256; ++bus) {
        for (Byte device = 0; device < 32; ++device) {
            for (Byte function = 0; function < 8; ++function) {
                DoubleWord info = PCIInfo((Byte)bus, device, function, 0x02);
                Byte classCode = (info & 0xff000000) >> 24;
                if (classCode == 0xff) {
                    continue;
                }
                switch (classCode) {
                case 0: {
                    Print("VGA-Compatible Device:");
                    break;
                }
                case 1: {
                    Print("Storage Device:");
                    break;
                }
                case 2: {
                    Print("Network Device:");
                    break;
                }
                case 3: {
                    Print("Display Device:");
                    break;
                }
                case 6: {
                    Print("Bridge Device:");
                    break;
                }
                case 0xc: {
                    Print("Serial Bus Device:");
                    break;
                }
                default: { Print("Other Device:"); }
                }
                Print("bus-");
                Print(QuadWordToHex(bus));
                Print(" device-");
                Print(QuadWordToHex(device));
                Print(" function-");
                Print(QuadWordToHex(function));
                Print(" type-");
                Print(QuadWordToHex(info));
                Print("\n");
            }
        }
    }
    InitMemory(memoryMap);
    MemoryMapBlock *mm = memoryMap;
    while (!((mm->base == 0) && (mm->length == 0) && (mm->type == 0))) {
        Print("Addr:");
        Print(QuadWordToHex((QuadWord)mm->base));
        Print(" Len:");
        Print(QuadWordToHex(mm->length));
        Print(" Type:");
        Print(QuadWordToHex(mm->type));
        Print("\n");
        ++mm;
    }
    Print("Memory Size:");
    Print(LongToString(GetMemorySize()));
    Print(" Page Count:");
    Print(LongToString(GetPageCount()));
    Print(" Free Count:");
    Print(LongToString(GetFreePageCount()));
    Print("\n");
    InitGDT();
    Print("Initialized GDT\n");
    while (true) {
    }
}
