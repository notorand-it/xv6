#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"


// This means that this vairable is used for sync. i.e. it's being look at by multiple cores
volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.


// Explanation can be found -> https://youtu.be/6hhJ6JN95As
void
main()
{

  // This is the init of the first core, only after this core is done executing will the other cores be able to do their int
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    
    // Just prints boot msg
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");


    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process

//  Tells compiler to not do the optimization
//  This makes sure everything above this line
//  is executed first and anything after this is done 
//  Later. 
//  Basically this stops compiler optimizations, because
//  we don't want started to be set to 1 randomly rn
    __sync_synchronize();


    // Set started to 1, that means that the other cpu cores can do their init. 
    started = 1;
  } 
  // This is the block for other cores i.e. everything but cpuid 0
  else {

    // Busy waiting until core 0 is done it it's stuff
    while(started == 0)
      ;

    // Look at explanation in core 0's __sync_synchronize() call.
    __sync_synchronize();

    printf("hart %d starting\n", cpuid());
    
    
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

// All cores call the schedular this is so that they can look for proccesses to execute 
  scheduler();        
}
