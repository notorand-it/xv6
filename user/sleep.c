#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "usage: sleep number_of_seconds\n");
        exit(1);
    }

    int seconds = atoi(argv[1]);
    printf("sleep for %d seconds\n", seconds);
    if(sleep(seconds * 10) != 0) {
        fprintf(2, "sleep error %d\n", seconds);
        exit(1);
    }
    exit(0);
}