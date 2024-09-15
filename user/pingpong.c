#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  int p[2];
  int q[2];

  char buf[1];
  int n;

  pipe(p);
  pipe(q);
  if (fork() == 0) {
    close(q[1]);
    close(p[0]);

    for (int i = 0; i < 10; i++) {
      n = read(q[0], buf, sizeof(buf));
      write(p[1], buf, n);
    }
  } else {
    close(p[1]);
    close(q[0]);

    write(q[1], "B", 1);
    for (int i = 0; i < 10; i++) {
      n = read(p[0], buf, sizeof(buf));
      write(1, buf, n);
      write(q[1], buf, n);
    }
  }

  exit(0);
}
