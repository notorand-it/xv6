#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    printf("yo soy tu padre - dijo el proceso %d\n", getppid());
    
    printf("ancestro (0) %d\n", getancestor(0));
    printf("ancestro (1) %d\n", getancestor(1));
    printf("ancestro (2) %d\n", getancestor(2));
    printf("ancestro (10) %d\n", getancestor(10));
    exit(0);
}