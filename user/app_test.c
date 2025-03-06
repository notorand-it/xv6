#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int v = test();
    printf("API & system test: %d\n", v);
    exit(0);
}