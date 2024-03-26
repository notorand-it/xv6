#ifndef XV6_RISCV_OS_LAB1_PROCINFO_H
#define XV6_RISCV_OS_LAB1_PROCINFO_H

enum procstate {
    UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE
};

typedef struct procinfo {
    char name[16];
    int parent_id;
    enum procstate state;
} procinfo_t;

#endif
