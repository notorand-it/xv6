// Mutual exclusion spin locks.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

// Ref -> https://youtu.be/gQdflOUZQvA?list=PLbtzT1TYeoMhTPzyTZboW_j7TPAnjv9XB

// General Note 

// 1) Spinlocks should not be held for long periods of time.
//  Plan to release lock soon after aq, the thread should not like go to sleep or get time sliced 

// 2) Locks are used to protect shared data 
// aquire()
// : {Critical Section}
// relase()


// When lock is first created, once name is set it does not change
void
initlock(struct spinlock *lk, char *name)
{
  lk->name = name;
  lk->locked = 0;
  lk->cpu = 0;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
void
acquire(struct spinlock *lk)
{

  // We cant use simple operation of doing something like if lock == 0 { lock = 1//execute critical section }
  // This is because multipe threads can access and accidentally set the resource to 1 and think both of them own the lock
  // Instead we want to use amoswap which is present in RISC-V.
  // What this will do is 
  // We wil set value of lock to 1 and return the old value of lock
  // So 2 things can happen
  // if value was 0, we just now set it to 1 and we got the lock too 
  // If value was 1, we set to 1 -> which does nothing and we get a response back saying value was 1. So we just continue spinning


// Why we need to disable interrupts is explained well in the youtube video I linked
// He gave an example where if we have keyboard input handler and another thread that is trying to read fromm a buffer.
// So for example if thread is reading from bufer and at the same time keyboard key is pressed , 
// the tread will be interrupted and the keyboard input handler will be executed, but the keyboard handler will try to aquire the lock and will busy wait 
// because the thread is holding the lock and the thread will never release the lock because it is waiting for the keyboard handler to finish
// As a result we have a deadlock

// Other benifits of disabling interrupts 
// 1) Do not hold sping lock for long periods of time
  //  a) No time slicing while holding lock
  // b) No interupt processing while holding lock

  push_off(); // disable interrupts to avoid deadlock.
  
  // If we are already hollding the lock then something wrong has hapned so we panic as something rly went wrong lol
  if(holding(lk))
    panic("acquire");

  // On RISC-V, sync_lock_test_and_set turns into an atomic swap:
  //   a5 = 1
  //   s1 = &lk->locked
  //   amoswap.w.aq a5, a5, (s1)
  // We loop with != 0 because once we get the old value as 0, that means we set the lock to 1 and got 0 value back to lock was free
  while(__sync_lock_test_and_set(&lk->locked, 1) != 0)
    ;

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen strictly after the lock is acquired.
  // On RISC-V, this emits a fence instruction.
  __sync_synchronize();

  // Record info about lock acquisition for holding() and debugging.
  // Every core has a struct associcated with it 
  // mycpu() returns pointer associated with a strcuture
  lk->cpu = mycpu();
}

// Release the lock.
void
release(struct spinlock *lk)
{

  // Again if we are already holding the lock then something wrong has hapned so we panic as something rly went wrong lol
  if(!holding(lk))
    panic("release");

// We set the cpu to 0 as we are no longer holding the lock
  lk->cpu = 0;

  // Tell the C compiler and the CPU to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other CPUs before the lock is released,
  // and that loads in the critical section occur strictly before
  // the lock is released.
  // On RISC-V, this emits a fence instruction.
  __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code doesn't use a C assignment, since the C standard
  // implies that an assignment might be implemented with
  // multiple store instructions.
  // On RISC-V, sync_lock_release turns into an atomic swap:
  //   s1 = &lk->locked
  //   amoswap.w zero, zero, (s1)
  // We set the lock to 0, so that other threads can access it
  __sync_lock_release(&lk->locked);

// Enable interrupts if they were on when the lock was acquired.
  pop_off();
}

// Check whether this cpu is holding the lock.
// Interrupts must be off.
int
holding(struct spinlock *lk)
{

  // Simply checks if current cpu is holding the lock
  int r;
  r = (lk->locked && lk->cpu == mycpu());
  return r;
}

// push_off/pop_off are like intr_off()/intr_on() except that they are matched:
// it takes two pop_off()s to undo two push_off()s.  Also, if interrupts
// are initially off, then push_off, pop_off leaves them off.

// This will manage the enabling and disabling of interrupts to avoid deadlocks 
// This is explained well in the youtube video I linked
// For pushoff 
// We do the following 
// We inrement noff counter by 1 
// If this was first call to aquire we store the state of interrupts in intena i.e. if they are currently enabled

void
push_off(void)
{

  // Prev status of interrupts
  int old = intr_get();

// turn off interrupts 
  intr_off();


  if(mycpu()->noff == 0)
    mycpu()->intena = old;
  mycpu()->noff += 1;
}


// We drement noff counter by 1
// If noff counter is 0 and interrupts were enabled before we call pushoff then we enable interrupts

void
pop_off(void)
{

  // Get ptr to cpu struct
  struct cpu *c = mycpu();

  // If interrupts are alaredy enabled then we panic as something went wrong
  if(intr_get())
    panic("pop_off - interruptible");
  
  // If noff counter is 0 then we panic as something went wrong
  // BEcause why would we call popoff if we did not call pushoff
  if(c->noff < 1)
    panic("pop_off");

  c->noff -= 1;

  // If noff counter is 0 and interrupts were enabled before we call pushoff then we enable interrupts
  if(c->noff == 0 && c->intena)
    intr_on();
}



  // On RISC-V, sync_lock_test_and_set turns into an atomic swap:
  //   a5 = 1
  //   s1 = &lk->locked
  //   amoswap.w.aq a5, a5, (s1)

  // Explanation From GPT4

// 2. **Assembly Instructions**:
//    - `a5 = 1`: This sets the register `a5` to 1. This value (1) typically represents the state of acquiring a lock.
//    - `s1 = &lk->locked`: This instruction stores the address of `lk->locked` in register `s1`. Here, `lk` is presumably a pointer to a lock structure, and `locked` is a field within that structure indicating the lock's status.
//    - `amoswap.w.aq a5, a5, (s1)`: This is the atomic swap operation. The `amoswap` instruction is an atomic memory operation in RISC-V. Here's the breakdown:
//      - `w`: Indicates the operation is on a word (32-bit).
//      - `aq`: Stands for "Acquire". It ensures memory ordering, meaning subsequent loads and stores will be executed only after this atomic operation completes. This is crucial for maintaining consistency in multi-threaded environments.
//      - `a5, a5, (s1)`: The operation swaps the value at the memory location pointed to by `s1` (the lock status) with the value in `a5` (which is set to 1). The original value at `s1` is also stored back in `a5`.

// 3. **Functionality in Context of Locking**:
//    - This atomic operation is used to acquire a lock. The swap operation is atomic, ensuring that no other thread can simultaneously change the state of `lk->locked`. If the lock was free (i.e., `lk->locked` was 0), the swap sets it to 1, effectively locking it. The original value (presumably 0 if the lock was free) is returned in `a5`.
//    - If `a5` returns 0 after the operation, it indicates that the lock was successfully acquired. If it returns 1, it means the lock was already held by another thread.

// In summary, this code is a part of a locking mechanism using atomic operations in RISC-V architecture. It ensures thread-safe lock acquisition by atomically swapping the lock state in a way that prevents race conditions.