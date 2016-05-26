#include "../common/type.h"

#ifndef HFMemoryPage
#define HFMemoryPage

// 内存映射块
typedef struct {
    QuadWord base;      // 基地址
    QuadWord length;    // 长度
    DoubleWord type;    // 内存类型(1:物理内存(可用),2:保留内存(不可用),3:ACPI可回收内存(可用),4:ACPI NVS内存(不可用),5:包含坏内存的区域(不可用).其他类型均认为是不可用内存)
    DoubleWord reserved;// 保留字段
} MemoryMapBlock;


// 初始化内存映射信息,MemoryMapBlock指向的Block数组的最后一项的字段值应当全部为0,用于标识Block数组结束
void InitMemory(MemoryMapBlock *mmb);

// 申请指定数量(count)的内存页,返回指向每一页的内存地址的数组
QuadWord* AllocPage(QuadWord count);

// 释放指定数量(count)的内存页,页面地址的数组为addr
void FreePage(QuadWord *addr,QuadWord count);



#endif