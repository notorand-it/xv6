#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define ERROR_MESSAGE "Error: Unable to retrieve kernel messages\n"
#define PAGESIZE 1024

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "go") == 0) {
        for (int i = 0; i < 100; ++i) {
            int pid = fork();
            if (pid == 0) {
                exec("vm", argv);
                exit(0);
            } else {
                wait(&pid);
            }
        }
    }

    char message[PAGESIZE];

    if (dmesg(message, PAGESIZE) < 0) {
        printf(ERROR_MESSAGE);
        exit(0);
    }

    printf("%s", message);

    exit(0);
}
