#include "page.h"



// 内核加载的物理地址(由连接器确定地址,不能直接使用)
const Byte LinkerKernelLoadAddr;
// 内核起始虚拟地址(由连接器确定地址,不能直接使用)
const Byte LinkerKernelStartVirtualAddr;
// 内核结束虚拟地址(由连接器确定地址,不能直接使用)
const Byte LinkerKernelEndVirtualAddr;


// 最小可用的物理内存地址(1M以下保留)
const QuadWord MinUsablePhysicalMemory = 0x100000;
// 内存分页大小(4k)
const QuadWord PageSize = 0x1000;

// 内核加载的物理地址
QuadWord KernelLoadAddr = 0;
// 内核虚拟地址
QuadWord KernelVirtualAddr = 0;
// 内核占用的页数
QuadWord KernelUsedPage = 0;



//内存实际总量
QuadWord ramSize = 0;
//内存可用总页数
QuadWord pageCount = 0;


// 页链接节点
typedef struct sPageLinkNode {
    QuadWord base;//内存物理基地址
    struct sPageLinkNode *pre;//上一个页节点
    struct sPageLinkNode *next;//下一个页节点
} PageLinkNode;

// 已经使用的页数量
QuadWord usedPageCount = 0;
// 已经使用的页链
PageLinkNode used = {0,0,0};
// 尚未使用的页链
PageLinkNode free = {0,0,0};




// 初始化内存映射信息,MemoryMapBlock指向的Block数组的最后一项的字段值应当全部为0,用于标识Block数组结束
void InitMemory(MemoryMapBlock *mmb) {
    /*初始化内核基本信息*/
    KernelLoadAddr = &LinkerKernelLoadAddr;
    KernelVirtualAddr = &LinkerKernelStartVirtualAddr;
    KernelUsedPage = (&LinkerKernelEndVirtualAddr - &LinkerKernelStartVirtualAddr + PageSize - 1) / PageSize;

    
    /*初始化内存*/
    used.pre = used.next = &used;    
    free.pre = free.next = &free;
    PageLinkNode *node = (PageLinkNode*)(KernelVirtualAddr + KernelUsedPage * PageSize);
    for (;!((mmb->base == 0) && (mmb->length == 0) && (mmb->type == 0));++mmb) {
        if (mmb->type == 1 || mmb->type == 3) {
            ramSize += mmb->length;
            QuadWord base = mmb->base;
            QuadWord length = mmb->length;
            if ((base + length) <= MinUsablePhysicalMemory) {
                // 忽略低于 MinUsablePhysicalMemory 的物理内存
                continue;
            }
            if (base < MinUsablePhysicalMemory) {
                // 忽略低于 MinUsablePhysicalMemory 的物理内存
                length -= MinUsablePhysicalMemory - base;
                base = MinUsablePhysicalMemory;
            }
            QuadWord remainder = base % PageSize;
            if (remainder != 0) {
                // 按PageSize对齐
                QuadWord interval = PageSize - remainder;
                if (length <= interval) {
                    continue;
                }
                base += interval;
                length -= interval;
            }
            QuadWord count = length/PageSize; 
            pageCount += count; 
            //构造可用页链表
            while (count > 0) {
                //构建node
                node->base = base;
                node->pre = free.pre;
                node->next = &free;
                
                free.pre->next = node;
                free.pre = node;
                
                ++node;
                --count;
            }
        }
    }
    //将内核已使用的内存页转为已使用
    
}

// 申请指定数量(count)的内存页,若申请成功则将内存地址填充到memArr指向的数组中并返回0,否则返回错误码
ErrorCode AllocPage(QuadWord count,QuadWord *memArr) {
    return 0;
}

// 释放指定数量(count)的内存页,页面地址的数组为addr
void FreePage(QuadWord *addr,QuadWord count) {
    
}


// 获取可用内存大小
QuadWord GetMemorySize() {
    return ramSize;
}
// 获取总页数
QuadWord GetPageCount() {
    return pageCount;
}
// 获取可用页数
QuadWord GetFreePageCount() {
    return pageCount - usedPageCount;
}