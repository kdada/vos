#include "page.h"



// 页链接节点
typedef struct {
    QuadWord base;
    QuadWord *pre;
    QuadWord *next;
} PageLinkNode;

//已经使用的页数量
QuadWord usedPageCount = 0;
// 已经使用的
PageLinkNode *used = 0;
// 尚未使用的
PageLinkNode *free = 0;

//内存实际总量
QuadWord ramSize = 0;
//内存可用总页数
QuadWord pageCount = 0;
//分页大小(4k)
const QuadWord pageSize = 0x1000;

// 初始化内存映射信息,MemoryMapBlock指向的Block数组的最后一项的字段值应当全部为0,用于标识Block数组结束
void InitMemory(MemoryMapBlock *mmb) {
    while (!((mmb->base == 0) && (mmb->length == 0) && (mmb->type == 0))) {
        if (mmb->type == 1 || mmb->type == 3) {
            ramSize += mmb->length;
            QuadWord remainder = mmb->base % pageSize;
            QuadWord base = mmb->base;
            Long length = mmb->length;
            if (remainder != 0) {
                // 按pageSize对齐
                QuadWord interval = pageSize - remainder;
                if (length <= interval) {
                    continue;
                }
                base += interval;
                length -= interval;
            }
            pageCount += length/pageSize;
            //构造可用页链表
        }
        ++mmb;
    }
}

// 申请指定数量(count)的内存页,返回指向每一页的内存地址的数组
QuadWord* AllocPage(QuadWord count) {
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