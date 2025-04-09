#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
sys_calculate(void) {
    int x, y;
    uint64 op_addr, result_addr;
    char op;
    int res = 0;

    argint(0, &x);
    argint(1, &y);
    argaddr(2, &op_addr);
    argaddr(3, &result_addr);

    if (op_addr == 0 || result_addr == 0)
        return -1;

    if (copyin(myproc()->pagetable, (char*)&op, op_addr, 1) < 0)
        return -1;

    switch (op) {
        case '+': res = x + y; break;
        case '-': res = x - y; break;
        case '*': res = x * y; break;
        case '/':
            if (y == 0) return -1;
            res = x / y;
            break;
        default: return -1;
    }

    if (copyout(myproc()->pagetable, result_addr, (char*)&res, sizeof(res)) < 0)
        return -1;

    return 0;
}
