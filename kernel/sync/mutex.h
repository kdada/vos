#include "../common/type.h"

#ifndef HFSyncMutex
#define HFSyncMutex

// 互斥锁
typedef struct {

} Mutex;

// 锁
void Lock(Mutex *mutex);
// 解锁
void Unlock(Mutex *mutex);

#endif
