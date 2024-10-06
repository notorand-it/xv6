#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include <stdarg.h>

#define PGSIZE 4096 // bytes per page

int
freebyte(void){
  return freepg() * PGSIZE;
}

int
freekb(void){
  return freepg() * PGSIZE / 1024;
}

int
freemb(void){
  return freepg() * PGSIZE / 1024 / 1024;
}


int
main(int argc, char *argv[])
{
  printf("free memory %d Pages\n", freepg());
  printf("free memory %d Bytes\n", freebyte());
  printf("free memory %d KB\n", freekb());
  printf("free memory %d MB\n", freemb());
  
  exit(0);
}


