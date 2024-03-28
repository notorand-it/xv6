#ifndef XV6_RISCV_OS_LAB1_MUTEX_H
#define XV6_RISCV_OS_LAB1_MUTEX_H

struct mutex {
    int open_md_cnt;
    struct sleeplock lock;
};

#endif //XV6_RISCV_OS_LAB1_MUTEX_H
