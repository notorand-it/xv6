#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
#include "run.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace()
{
  int mask;
  argint(0, &mask);
  if (mask < 0)
      return -1;

  struct proc *pro = myproc();
  printf("trace pid: %d\n", pro->pid);
  pro->trace_mask = mask;
  return 0;
}

// Get the number of bytes of free memory
uint64
sys_get_free_memory()
{
  struct run *r;
  struct {
    struct spinlock lock;
    struct run *freelist;
  } kmem;
  uint64 pages = 0;

  acquire(&kmem.lock);
  r = kmem.freelist;
  while (r) {
    pages++;
    r = r->next;   
  }
  release(&kmem.lock);

  return pages * PGSIZE;
}

// Get the num of proccesses
uint64
sys_get_proccesses_num()
{
  struct proc *p;
  struct proc proc[NPROC];
  uint64 num = 0;

  for(p = proc; p < &proc[NPROC]; p++) {
    if(p->state != UNUSED) 
      num++;
  }

  return num;
}

uint64
sys_sysinfo()
{
  uint64 parameter;
  argaddr(0, &parameter);
  if(parameter < 0)
    return -1;
  
  struct sysinfo info;
  info.freemem = sys_get_free_memory();
  info.nproc = sys_get_proccesses_num();

  struct proc *p = myproc();
  if (copyout(p->pagetable, parameter, (char *)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}
