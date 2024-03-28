#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "defs.h"
#include "mutex.h"

mutex_table_t mutex_table;

void mutexinit(void) {
    initlock(&mutex_table.access_lock, "Init mutex table access lock");

    acquire(&mutex_table.access_lock);
    for (int i = 0; i < NMUTEX; i++) {
        initsleeplock(&mutex_table.table[i].lock, "Init ith mutex sleep lock");
        mutex_table.table[i].open_md_cnt = 0;
    }
    release(&mutex_table.access_lock);
}

int mutex_create(void) {
    acquire(&mutex_table.access_lock);

    struct proc *curr_proc = myproc();
    for (int i = 0; i < NMUTEX; i++) {
        if (mutex_table.table[i].open_md_cnt == 0) {
            mutex_table.table[i].open_md_cnt = 1;
            release(&mutex_table.access_lock);

            acquire(&curr_proc->lock);
            for (int j = 0; j < NOMUTEX; j++) {
                if (curr_proc->omutex[j] == 0) {
                    curr_proc->omutex[j] = &mutex_table.table[i];
                    release(&curr_proc->lock);
                    return j;
                }
            }
            release(&curr_proc->lock);
            return -1; // NOMUTEX lim exceeded
        }
    }

    release(&mutex_table.access_lock);
    return -2; // NMUTEX lim exceeded
}

int mutex_destroy(int md) {
    if (md < 0 || md >= NMUTEX) return -1; // Out of bounds

    acquire(&mutex_table.access_lock);

    if (mutex_table.table[md].open_md_cnt == 0) {
        release(&mutex_table.access_lock);
        return -2; // Non-existing mutex
    }

    struct proc *curr_proc = myproc();

    acquire(&curr_proc->lock);
    if (curr_proc->omutex[md] == 0) { // Mutex for this proc exists, but not opened yet
        release(&curr_proc->lock);
        return -3;
    }

    if (holdingsleep(&curr_proc->omutex[md]->lock))
        releasesleep(&curr_proc->omutex[md]->lock); // TODO: подумай, чет большой лок для спинлока

    curr_proc->omutex[md] = 0;
    mutex_table.table[md].open_md_cnt--;
    release(&curr_proc->lock);

    release(&mutex_table.access_lock);
    return 0;
}

int mutex_lock(int md) {
    if (md < 0 || md >= NMUTEX) return -1; // Out of bounds
    if (mutex_table.table[md].open_md_cnt == 0) return -2; // Non-existing mutex
    if (holdingsleep(&mutex_table.table[md].lock)) return -3; // Trying to lock already locked mutex

    acquiresleep(&mutex_table.table[md].lock);
    return 0;
}

int mutex_unlock(int md) {
    if (md < 0 || md >= NMUTEX) return -1; // Out of bounds
    if (mutex_table.table[md].open_md_cnt == 0) return -2; // Non-existing mutex
    if (!holdingsleep(&mutex_table.table[md].lock)) return -3; // Trying to unlock already unlocked (or undefined) mutex

    releasesleep(&mutex_table.table[md].lock);
    return 0;
}