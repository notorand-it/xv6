#ifndef XV6_RISCV_OS_LAB1_DMSGBUFF_H
#define XV6_RISCV_OS_LAB1_DMSGBUFF_H

typedef struct {
    struct spinlock lock;
    char buff[NDMSGPAGE * PGSIZE];
    int head, tail;
} dmsg_buff_t;

#endif
