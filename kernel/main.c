#include "common/type.h"
#include "common/lib.h"
#include "display/print.h"


// 根据PCI地址获取数据 bus:8bit device:5bit function:3bit offset:6bit
DoubleWord PCIInfo(Byte bus,Byte device,Byte function,Byte offset) {
    Word portOut = 0xcf8;
    Word portIn = 0xcfc;
    DoubleWord addr = 0x80000000;
    addr |= ((DoubleWord)bus)<<16;
    addr |= ((DoubleWord)device)<<11;
    addr |= ((DoubleWord)function)<<8;
    addr |= offset << 2;
    asm ("out %1,%3\n\t"
        "mov %2,%3\n\t"
        "in %3,%0\n\t"
        :"=a"(addr)
        :"a"(addr),"c"(portIn),"d"(portOut)
        :
    );
    return addr;    
}

int main(){
    ClearScreen();
    for (Word bus = 0;bus < 256;++bus){
        for (Byte device = 0;device < 32;++device){
            for (Byte function = 0;function < 8;++function){
                DoubleWord info = PCIInfo((Byte)bus,device,function,0x02);
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
                    default: {
                        Print("Other Device:");
                    }
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
    return 0;
}

