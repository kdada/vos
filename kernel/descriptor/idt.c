#include "idt.h"

typedef struct {
    Word offsetL;
    Word selector;
    Byte zero;
    Byte attribute;
    Word offsetM;
    DoubleWord offsetH;
    DoubleWord reserved;
} IDTEntry;
