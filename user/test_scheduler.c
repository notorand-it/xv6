#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void
proceso_hijo(int id) {
    printf("proceso hijo: %d (PID: %d) iniciado.\n", id, getpid());
    sleep(2000);
    printf("proceso hijo %d (PID: %d) finalizado. \n", id, getpid());
    exit(0);
}

int 
main(void) {
    int i;
    int n = 20;

    printf("proceso inciado PID: %d\n", getpid());

    for(i = 0; i < n; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("error al crear el proceso %d/n", getpid());
        } else if (pid == 0) {
            proceso_hijo(i);
        }
    }
}