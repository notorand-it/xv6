#pragma once
#include "types.h"

// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

typedef struct {
  uint64 lock;
  kstr   name;
} spinlockv2[1];

#define spinlockv2_init ((spinlockv3){{0,0}})
