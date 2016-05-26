#include "page.h"


// 页链接节点(每页大小为4k)
typedef struct {
    QuadWord base;
    QuadWord *pre;
    QuadWord *next;
} PageLinkNode;

// 已经使用的
PageLinkNode *used = 0;
// 尚未使用的
PageLinkNode *free = 0;
//内存总数
QuadWord ramSize = 0；

// 初始化内存映射信息,MemoryMapBlock指向的Block数组的最后一项的字段值应当全部为0,用于标识Block数组结束
void InitMemory(MemoryMapBlock *mmb) {
    QuadWord ramSize = 0;
    
    while (!((mm->base == 0) && (mm->length == 0) && (mm->type == 0))) {
        if (mm->type)
        ++count
        ++mm;
    }
}

// 申请指定数量(count)的内存页,返回指向每一页的内存地址的数组
QuadWord* AllocPage(QuadWord count) {
    return 0
}

// 释放指定数量(count)的内存页,页面地址的数组为addr
void FreePage(QuadWord *addr,QuadWord count) {
    
}
