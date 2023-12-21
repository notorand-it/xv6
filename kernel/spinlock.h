// Mutual exclusion lock.
struct spinlock {

  // You can think of this as a boolean value
  // 0 -> Unlocked
  // 1 -> Locked
  uint locked;       // Is the lock held?


// These values are only used for debugging it does not have impact on the lock
  // For debugging:
  char *name;        // Name of lock.

  // Every CPU has a struct associated with it called CPU
  // This is just a ptr to that struct
  struct cpu *cpu;   // The cpu holding the lock.
};

