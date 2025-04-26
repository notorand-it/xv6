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
  uint64 lock; // Locking data: MSB flag + mhartid
  uint64 name; // Debugging 8-char string kstr
} spinlockv2;
