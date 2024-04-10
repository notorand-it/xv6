struct cycle_queue {
    char data[PGSIZE * QNPAGES];
    int head, tail;
    struct spinlock lock;
};