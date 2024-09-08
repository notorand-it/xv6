#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void thread_func(void *arg) {
    int tid = *(int *)arg;
    printf("Thread %d: started.\n", tid);

    for (int i = 0; i < 5; i++) {
        printf("Thread %d: counter %d\n", tid, i);
        sleep(10);
    }

    printf("Thread %d: finished.\n", tid);
    exit(0);
}

int main(int argc, char *argv[]) {
    const int num_threads = 3;
    int thread_ids[num_threads];
    int i;

    printf("Main thread: starting %d threads.\n", num_threads);

    for (i = 0; i < num_threads; i++) {
        thread_ids[i] = i;

        if (thread_create(&thread_func, (void*)&thread_ids[i]) < 0) {
            printf("Error: thread creation failed for thread %d\n", i);
            exit(1);
        }
    }

    printf("Main thread: all threads created, waiting for them to finish.\n");

    for (i = 0; i < num_threads; i++) {
        thread_wait(0, 0);
    }

    printf("Main thread: all threads finished.\n");

    exit(0);
}
