#include "kernel/param.h"
#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user/user.h"

void ugetpid_test();
void pgaccess_test();
void pgdirty_test();

int
main(int argc, char *argv[])
{
  ugetpid_test();
  pgaccess_test();
  pgdirty_test();
  printf("pgtbltest: all tests succeeded\n");
  exit(0);
}

char *testname = "???";

void
err(char *why)
{
  printf("pgtbltest: %s failed: %s, pid=%d\n", testname, why, getpid());
  exit(1);
}

void
ugetpid_test()
{
  int i;

  printf("ugetpid_test starting\n");
  testname = "ugetpid_test";

  for (i = 0; i < 64; i++) {
    int ret = fork();
    if (ret != 0) {
      wait(&ret);
      if (ret != 0)
        exit(1);
      continue;
    }
    if (getpid() != ugetpid())
      err("missmatched PID");
    exit(0);
  }
  printf("ugetpid_test: OK\n");
}

void
pgaccess_test()
{
  char *buf;
  unsigned int abits;
  printf("pgaccess_test starting\n");
  testname = "pgaccess_test";
  buf = malloc(32 * PGSIZE);
  if (pgaccess(buf, 32, &abits) < 0)
    err("pgaccess failed");
  buf[PGSIZE * 1] += 1;
  buf[PGSIZE * 2] += 1;
  buf[PGSIZE * 30] += 1;
  if (pgaccess(buf, 32, &abits) < 0)
    err("pgaccess failed");
  if (abits != ((1 << 1) | (1 << 2) | (1 << 30)))
    err("incorrect access bits set");
  free(buf);
  printf("pgaccess_test: OK\n");
}


void
pgdirty_test()
{
  char *buf;
  unsigned int dbits;
  printf("pgdirty_test starting\n");
  testname = "pgdirty_test";
  buf = malloc(32 * PGSIZE);
  if (pgdirty(buf, 32, &dbits) < 0)
    err("pgdirty failed");
  buf[PGSIZE * 0] += 1;
  buf[PGSIZE * 5] += 1;
  buf[PGSIZE * 6] += 1;
  buf[PGSIZE * 27] += 1;
  buf[PGSIZE * 31] += 1;

  printf("buf:20 %d, address %x\n", buf[PGSIZE * 20], &buf[PGSIZE * 20]); //read-only operation. dirty bit should not be set 

  if (pgdirty(buf, 32, &dbits) < 0)
    err("pgdirty failed");
  if (dbits != ((1 << 0) | (1 << 5) | (1 << 6) | (1 << 27) | (1 << 31)))
    err("incorrect dirty bits set");
  free(buf);
  printf("pgdirty_test: OK\n");
}
