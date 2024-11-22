#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

// Implementación de getppid
uint64
sys_getppid(void)
{
    struct proc *curproc = myproc();
    if (curproc->parent) // Si el proceso tiene un padre
        return curproc->parent->pid;
    return -1; // Si no tiene padre
}

// Implementación de getancestor
uint64
sys_getancestor(void)
{
    int level;

    // Obtener el argumento del nivel
    argint(0, &level);

    // Validar que el nivel no sea negativo
    if (level < 0)
        return -1;

    struct proc *p = myproc(); // Proceso actual

    // Recorre hacia arriba por los ancestros
    for (int i = 0; i < level; i++) {
        if (!p->parent) // Si no hay más ancestros válidos
            return -1;
        p = p->parent; // Avanzar al proceso padre
    }

    return p->pid; // Retornar el PID del ancestro
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
