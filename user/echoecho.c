#include "kernel/types.h"
#include "user/user.h"

#define C(x) ((x) - '@')
#define BUFFER_SIZE 512

int main(void) {
  char buffer[BUFFER_SIZE];
  int index = 0;

  while (index < BUFFER_SIZE && read(0, &buffer[index], 1) == 1) {
    if (buffer[index] == C('D')) {
      break;
    }
    index++;
  }
  if (index > 0) {
    write(1, buffer, index);
  }
  exit(0);
}