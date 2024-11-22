#include "../kernel/types.h"
#include "user.h"


int main() {
    printf("PID actual: %d\n", getpid());
    printf("PPID: %d\n", getppid());

    for (int i = 0; i <= 3; i++) {
        int ancestor = getancestor(i);
        if (ancestor == -1) {
            printf("Nivel %d: No hay ancestro\n", i);
        } else {
            printf("Nivel %d: PID %d\n", i, ancestor);
        }
    }

    exit(0); // Terminar con éxito
    return 0; // Solo por convención, aunque exit detiene la ejecución
}
