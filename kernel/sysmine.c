#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"

uint64
sys_getppid(void) 
{

    return myproc()->parent->pid; // Retorna el PID del proceso padre
    
}