#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "defs.h"

int sys_mutex_create(void) {
    return mutex_create();
}

int sys_mutex_destroy(void) {
    int md;
    argint(0, &md);
    return mutex_destroy(md);
}

int sys_mutex_lock(void) {
    int md;
    argint(0, &md);
    return mutex_lock(md);
}

int sys_mutex_unlock(void) {
    int md;
    argint(0, &md);
    return mutex_unlock(md);
}