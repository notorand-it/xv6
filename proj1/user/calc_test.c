// calc-test.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// struct for a pair of nums & op: int a, int b, char op
struct calc {
  int a;
  int b;
  char op;
};

// to init an array of struct calc:
struct calc tests[] = {
  {10, 4, '-'},
  {34, -9, '+'},
  {8, 12, '*'},
  {56, 6, '/'},
  {56, 6, '&'},
  {5, 0, '/'}
};
// its type when passed to run_tests is: 
void run_tests(struct calc *tests, int n) {
  for (int i = 0; i < n; i++) {
    int res = 0;
    char op = tests[i].op;
    int error_code = calculate(tests[i].a, tests[i].b, &op, &res);
    printf("%d %d\n", res, error_code);
  }
}

int main(int argc, char *argv[]) {
  struct calc tests[] = {
    {10, 4, '-'},
    {34, -9, '+'},
    {8, 12, '*'},
    {56, 6, '/'},
    {56, 6, '&'},
    {5, 0, '/'}
  };

  int n = sizeof(tests) / sizeof(tests[0]);
  run_tests(tests, n);

  exit(0);
}