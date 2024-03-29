#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "lab2_checkers.c"
#include "lab3_checkers.c"

// mutex_test [INPUT STRING] [CORRECTNESS ARG (default = false)]
int main(int argc, char **argv) {
    myassert(argc == 2 || argc == 3, "Not enough / invalid args.\n");
    int is_correct_version = (argc == 3 && !strcmp(argv[2], "-c")) ? 1 : 0;

    int child[2], parent[2];
    pipe_check(pipe(child));
    pipe_check(pipe(parent));

    int mutex = mutex_create(); // Пусть для красоты кода мьютекс всегда создается... А то иначе жесткая копипаста
    mutex_check(mutex);

    int pid = fork();
    fork_check(pid);

    if (pid == 0) {
        close(child[1]);
        close(parent[0]);

        char c;
        int read_status;

        while ((read_status = read(child[0], &c, 1)) != 0) {
            check_read_status(read_status);

            if (is_correct_version) mutex_check(mutex_lock(mutex));
            printf("%d: received %c\n", getpid(), c);
            if (is_correct_version) mutex_check(mutex_unlock(mutex));

            write(parent[1], &c, 1);
        }
        close(child[0]);
        close(parent[1]);

        exit(0);
    }

    close(child[0]);
    close(parent[1]);

    write(child[1], argv[1], strlen(argv[1]));
    close(child[1]);

    char c;
    int read_status;
    while ((read_status = read(parent[0], &c, 1)) != 0) {
        check_read_status(read_status);

        if (is_correct_version) mutex_check(mutex_lock(mutex));
        printf("%d: received %c\n", getpid(), c);
        if (is_correct_version) mutex_check(mutex_unlock(mutex));
    }

    close(parent[0]);
    wait((int *) 0);
    mutex_destroy(mutex);

    exit(0);
}