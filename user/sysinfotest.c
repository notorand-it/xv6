#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  struct sysinfo info;
  sysinfo (&info);
  printf("Free element: %lu bytes\nUnused process: %lu\nNumber of opening files: %lu\n", info.freemem,info.nproc, info.nopenfiles);
  printf("sysinfotest: OK\n");
  exit(0);
}