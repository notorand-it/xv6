#include "kernel/types.h"
#include "user/user.h"

int
main(void) {
    int ppid = getppid();
    if (ppid < 0) {
        printf("Error: getppid falló\n");
        exit(1);
    }
    printf("El ID del proceso padre es: %d\n", ppid);
    exit(0);
}

