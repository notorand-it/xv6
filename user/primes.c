#include "kernel/types.h"
#include "user/user.h"

#define EXIT_FAILURE 1
#define INT_SIZE sizeof(int)

void perror(char* message) {
    fprintf(2, message);
}

int add_to_pipeline() {
    int p[2];
    pipe(p);

    char* child_exec_args[3];
    child_exec_args[0] = "primes";
    child_exec_args[1] = "1";
    child_exec_args[2] = 0;

    int pid = fork(), ret;

    if (pid == 0) {
        //replace fd 0 with p[0] so child process reads from pipeline
        close(0);
        dup(p[0]);

        // child no longer needs this
        close(p[0]);
        close(p[1]);

        // rough guess of fds child will have open
        // f0 -> pipeline left neighbour
        // f1 -> console write
        // f2 -> console write err
        // f3 -> [Optional] pipelint right neighbour, child might add a right neighbour to the pipeline

        // exec will not return because the child process's memory will be replaced by instructions in primes binary
        // the fds will not be replaced. If exec returns, then an error occured
        if ((ret = exec("primes", child_exec_args))) {
            fprintf(2, "EXEC FAILED result: %d, args %s %s\n", ret, child_exec_args[0], child_exec_args[1]);
            exit(1);
        }
    }

    // parent
    close(p[0]);
    
    return p[1];
}

void prime_sieve() {
    int p, n, bytes_read;
    int pipeline_read_fd = 0; // add_to_pipeline ensured the fd is 0
    int pipeline_write_fd = -1;

    read(pipeline_read_fd, &p, INT_SIZE);
    printf("prime %d\n", p);

    while (1)
    {
        bytes_read = read(pipeline_read_fd, &n, INT_SIZE);
        if (bytes_read == 0) {
            //EOF
            //close write pipe
            if (pipeline_write_fd != -1) {
                close(pipeline_write_fd);
            }
            break;
        }

        if (n%p != 0) {
            if (pipeline_write_fd == -1) {
                pipeline_write_fd = add_to_pipeline();
            }
            
            write(pipeline_write_fd, &n, INT_SIZE);
        }
    }

    close(pipeline_read_fd);
    wait(0);
}

int main(int argc, char *argv[]) {
    int ret;
    if (argc > 1) {
        prime_sieve();
        exit(0);
    }

    int i = 2, pipeline_write_fd = add_to_pipeline();

    for (; i <= 35; i++) {
        if ((ret = write(pipeline_write_fd, &i, INT_SIZE)) != INT_SIZE) {
            printf("Failed to write %d, result %d\n", i, ret);
            break;
        }
    }

    close(pipeline_write_fd);

    wait(0);
    exit(0);
}