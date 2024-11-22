#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void busy_wait(int seconds) {
    int start = uptime();
    while (uptime() - start < seconds)
        ;
}

int main() {
    for (int i = 0; i < 20; i++) {
        if (fork() == 0) {
            printf("Proceso hijo creado: PID = %d\n", getpid());
            busy_wait(2);
            exit(0);
        }
    }

    for (int i = 0; i < 20; i++) {
        wait(0);
    }

    exit(0);
}
