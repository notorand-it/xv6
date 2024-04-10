#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"
#include "cycle_queue.h"

void queue_add(struct cycle_queue *q, char a) {
    acquire(&q->lock);
    q->data[q->tail++] = a;
    if (q->tail == PGSIZE * QNPAGES) {
        q->tail = 0;
    }
    if (q->head == q->tail) {
        q->head++;
    }
    release(&q->lock);
}