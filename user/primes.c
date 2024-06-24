#include "kernel/types.h"
#include "user/user.h"

#define MAX 256

void filter(int *p) {
  int n, pr;

  if (read(p[0], &pr, sizeof(pr)) <= 0) {
    close(p[0]);
    exit(0);
  }

  printf("%d ", pr);

  int fd[2];
  pipe(fd);

  if (fork() == 0) {
    close(p[0]);
    close(fd[1]);
    filter(fd);
  } else {
    close(fd[0]);
    while (read(p[0], &n, sizeof(n)) > 0) {
      if (n % pr != 0) {
        write(fd[1], &n, sizeof(n));
      }
    }
    close(p[0]);
    close(fd[1]);
  }
}

int main() {
  int p[2];
  pipe(p);

  if (fork() == 0) {
    close(p[1]);
    int n;
    read(p[0], &n, sizeof(n));
    printf("%d ", n);
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
      close(p[0]);
      close(fd[1]);
      filter(fd);
    } else {
      close(fd[0]);
      while (read(p[0], &n, sizeof(n)) > 0) {
        if (n % 2 != 0) {
          write(fd[1], &n, sizeof(n));
        }
      }
      close(p[0]);
      close(fd[1]);
    }
  } else {
    close(p[0]);
    for (int i = 2; i < MAX; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
  }
  exit(0);
}