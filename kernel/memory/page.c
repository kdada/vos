#include "page.h"

// 内核加载的物理地址(由连接器确定地址,不能直接使用)
const Byte LinkerKernelLoadAddr;
// 内核起始虚拟地址(由连接器确定地址,不能直接使用)
const Byte LinkerKernelStartVirtualAddr;
// 内核结束虚拟地址(由连接器确定地址,不能直接使用)
const Byte LinkerKernelEndVirtualAddr;

// 最小可用的物理内存地址(1M以下保留)
const Byte *MinUsablePhysicalMemory = (Byte *)0x100000;
// 内存分页大小(4k)
const QuadWord PageSize = 0x1000;

// 内核加载的物理地址
Byte *KernelLoadAddr = 0;
// 内核虚拟地址
Byte *KernelVirtualAddr = 0;
// 内核占用的页数
QuadWord KernelUsedPages = 0;

// 内存段分配信息
typedef struct {
    Byte *base;
    QuadWord count;
    QuadWord free;
    Byte *bitmap;
} MemorySegment;

// 最多可以有 50 个可用内存段, 超过部分忽略
MemorySegment segments[50] = {{0}};
// 有效内存段数量
Byte segCount = 0;

Bool bitFromMap(Byte *map, QuadWord pos) {
    Byte bits = map[pos / 8];
    return bits & (1 << pos % 8);
}

void setBitToMap(Byte *map, QuadWord pos, Bool bit) {
    Byte *bits = &map[pos / 8];
    if (bit) {
        *bits |= (1 << pos % 8);
    } else {
        *bits &= ~(1 << pos % 8);
    }
}

Byte *allocPages(Byte segID, QuadWord count) {
    MemorySegment *seg = &segments[segID];
    if (seg->free < count) {
        return null;
    }
    QuadWord series = 0;
    for (QuadWord pos = 0; pos < seg->count; ++pos) {
        if (bitFromMap(seg->bitmap, pos)) {
            series = 0;
            continue;
        }
        ++series;
        if (series < count) {
            continue;
        }
        seg->free -= count;
        for (; count > 0; --count) {
            setBitToMap(seg->bitmap, pos - count + 1, true);
        }
        return seg->base + PageSize * (pos - series);
    }
    return null;
}

// 申请指定数量(count)的物理内存页, 申请成功则返回物理内存地址，否则返回 null
Byte *AllocPhysicalPage(QuadWord count) {
    for (Byte i = 0; i < segCount; ++i) {
        Byte *target = allocPages(i, count);
        if (target != null) {
            return target;
        }
    }
    return null;
}

// 释放指定数量(count)的物理内存页, 起始地址为 addr 所在的内存页
void FreePhysicalPage(Byte *addr, QuadWord count) {
    for (Byte i = 0; i < segCount; ++i) {
        MemorySegment *seg = &segments[i];
        if (addr >= seg->base && addr < (seg->base + seg->count * PageSize)) {
            QuadWord pos = (addr - seg->base) / PageSize;
            for (QuadWord i = 0; i < count; ++i) {
                if (bitFromMap(seg->bitmap, pos + i)) {
                    // 释放内存
                    setBitToMap(seg->bitmap, pos + i, false);
                    ++seg->free;
                }
            }
            return;
        }
    }
}

// 获取可用内存大小
QuadWord GetMemorySize() {
    QuadWord total = 0;
    for (Byte i = 0; i < segCount; ++i) {
        total += segments[i].count * PageSize;
    }
    return total;
}
// 获取总页数
QuadWord GetPageCount() {
    QuadWord total = 0;
    for (Byte i = 0; i < segCount; ++i) {
        total += segments[i].count;
    }
    return total;
}
// 获取可用页数
QuadWord GetFreePageCount() {
    QuadWord total = 0;
    for (Byte i = 0; i < segCount; ++i) {
        total += segments[i].free;
    }
    return total;
}

// 初始化内存映射信息, MemoryMapBlock
// 指向的 Block 数组的最后一项的字段值应当全部为 0, 用于标识 Block 数组结束
// 低于 0x100000 的内存被忽略, 不能用于分配, 保留给实模式使用
// 物理内存布局:
//
//   ---------------------- 物理内存上限
//    进程可申请的内存空间
//   ---------------------- 此处地址取决于 pageCount 和 segCount
//         内存位图
//   ---------------------- 0x100000+KernelUsedPages*PageSize
//         内核内存
//   ---------------------- 0x100000
//      实模式使用的内存
//   ---------------------- 0x0
//
void InitMemory(MemoryMapBlock *mmb) {
    /*初始化内核基本信息*/
    KernelLoadAddr = (Byte *)&LinkerKernelLoadAddr;
    KernelVirtualAddr = (Byte *)&LinkerKernelStartVirtualAddr;
    KernelUsedPages =
        (&LinkerKernelEndVirtualAddr - KernelVirtualAddr + PageSize - 1) /
        PageSize;

    // 初始化内存分段位图
    Byte *bitmapStartAddr = KernelVirtualAddr + KernelUsedPages * PageSize;
    segCount = 0;
    for (; !((mmb->base == 0) && (mmb->length == 0) && (mmb->type == 0));
         ++mmb) {
        if (mmb->type != 1 && mmb->type != 3) {
            // 不可分配的内存, 直接跳过
            continue;
        }
        Byte *base = mmb->base;
        QuadWord length = mmb->length;
        if ((base + length) <= MinUsablePhysicalMemory) {
            // 忽略低于 MinUsablePhysicalMemory 的物理内存
            continue;
        }
        if (base < MinUsablePhysicalMemory) {
            // 忽略低于 MinUsablePhysicalMemory 的物理内存
            length -= MinUsablePhysicalMemory - base;
            base = (Byte *)MinUsablePhysicalMemory;
        }
        QuadWord remainder = (QuadWord)base % PageSize;
        if (remainder != 0) {
            // 按PageSize对齐
            QuadWord interval = PageSize - remainder;
            if (length <= interval) {
                continue;
            }
            base += interval;
            length -= interval;
        }
        QuadWord count = length / PageSize;
        if (count <= 0) {
            continue;
        }
        // 构造内存段
        MemorySegment *seg = &segments[segCount];
        seg->base = base;
        seg->count = count;
        seg->free = seg->count;
        seg->bitmap = bitmapStartAddr;

        ++segCount;
        // 计算下一个内存段位图的起始地址
        bitmapStartAddr += (count + 7) / 8;
    }
    QuadWord *start =
        (QuadWord *)(KernelVirtualAddr + KernelUsedPages * PageSize);
    QuadWord mapUsedPages =
        (bitmapStartAddr - (Byte *)start + PageSize - 1) / PageSize;
    for (; start < (QuadWord *)bitmapStartAddr; ++start) {
        *start = 0;
    }
    // 将内核已使用的内存页标记为已使用
    allocPages(0, KernelUsedPages + mapUsedPages);
}
