#include "../common/type.h"

#ifndef HFMemoryPage
#define HFMemoryPage
// 物理地址中低于1M的部分被视为保留地址,其中的物理内存保留给实模式使用,在long mode下该部分物理内存不被分配



// 内存映射块
typedef struct {
    QuadWord base;      // 基地址
    QuadWord length;    // 长度
    DoubleWord type;    // 内存类型(1:物理内存(可用),2:保留内存(不可用),3:ACPI可回收内存(可用),4:ACPI NVS内存(不可用),5:包含坏内存的区域(不可用).其他类型均认为是不可用内存)
    DoubleWord reserved;// 保留字段
} MemoryMapBlock;


// 初始化内存映射信息,MemoryMapBlock指向的Block数组的最后一项的字段值应当全部为0,用于标识Block数组结束
void InitMemory(MemoryMapBlock *mmb);

// 申请指定数量(count)的内存页,若申请成功则将内存地址填充到memArr指向的数组中并返回0,否则返回错误码
ErrorCode AllocPage(QuadWord count,QuadWord *memArr);

// 释放指定数量(count)的内存页,页面地址的数组为addr
void FreePage(QuadWord *addr,QuadWord count);

// 获取物理内存大小
QuadWord GetMemorySize();
// 获取总页数
QuadWord GetPageCount();
// 获取可用页数
QuadWord GetFreePageCount();
#endif