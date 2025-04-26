#pragma once
#include "types.h"

// Mutual exclusion lock.
typedef struct {
  uint64 lock; // Locking data: MSB flag + mhartid
  uint64 name; // Debugging 8-char string
} spinlock;
