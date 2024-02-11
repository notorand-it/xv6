#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Trace mask must be specifed\n");
        exit(1);
    }

    if(argc < 3) {
        printf("a program must be specifed\n");
        exit(1);
    }

    /*
    get trace masks from args
    get program name and args
    fork process
      call trace(trace_mask) in child process
      exec program with programs args in the child process
    in parent, wait for child process to exit
    */
    int trace_mask = atoi(argv[1]);
    if (trace_mask == 0 && argv[1][0] != '0') {
        printf("Invalid trace mask. Mask must be an integer\n");
        exit(1);
    }

    char* program = argv[2];
    char** program_args;

    int program_args_len = argc - 2;
    int n_bytes = (program_args_len * sizeof(char*)) + 1;
    program_args = malloc(n_bytes);
    memmove(program_args, &argv[2], n_bytes);
    program_args[program_args_len] = 0;

    if (fork() == 0) {
        trace(trace_mask);
        exec(program, program_args);
    }

    wait(0);
    exit(0);
}