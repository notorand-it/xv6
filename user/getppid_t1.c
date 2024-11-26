#include "kernel/types.h"
#include "user/user.h"

int
main(void) {
    int ppid = getppid();
    printf("El ID del proceso padre es: %d\n", ppid);
    exit(0);
}

