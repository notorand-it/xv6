# Spinlockv2
An alternate and possibly more efficient implementation of a spinlock for RISC-V

## Implementation
We started from a few simple facts:  
1. the lock itself is a single bit of information;  
1. we need to keep the hart id for safety reasons;  
1. we can easily test a single bit if it is the sign bit (MSB) in a word/double word;  
1. short strings can fit in the 8 bytes of a pointer.

We flip the sign bit (64th) to toggle the lock and we use the lower 63 bits for the hard it.  
We use the double word next to the lock doubleword to ease possible debugging.  
Code is `acquirev2()` and `releasev2()`.  
