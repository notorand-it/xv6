#include "user.h"
#include "kernel/types.h"

int main() {
    int a = 5, b = 7;
    int result = add(a, b);
    printf("add(%d, %d) = %d\n", a, b, result);
    exit(0);
}
