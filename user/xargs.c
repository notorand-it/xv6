#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAXARGS 10


int main(int argc, char* argv[]) {

    //for quick testing
    // mkdir("a");
    // int fd = open("./a/b", O_CREATE|O_WRONLY);
    // write(fd, "Hello world\n", 12);
    // write(fd, "Hey world\n", 10);
    // write(fd, "Hi there\n", 9);
    // write(fd, "Hi world\n", 9);
    // close(fd);

    char* cmd;
    char* c_args[MAXARGS];

    if(argc < 2){
        cmd = "echo";    
    } else {
        cmd = argv[1];        
    }

    c_args[0] = cmd;

    int i, k;
    for(i = 2, k = 1; i < argc && k < MAXARGS; i++, k++) {
        c_args[k] = argv[i];
    }

    int c_args_count = k;

    char buff[512];
    i = 0;
    while(read(0, &buff[i], 1) > 0 && i < 512 && k < MAXARGS) {
        if(buff[i] == ' ') {
            //add buffer to cargs
            //reset buffer

            buff[i] = '\0';
            c_args[k++] = malloc(i);
            strcpy(c_args[k-1], buff);
            
            memset(buff, 0, 512);
            i = 0;
        }

        else if(buff[i] == '\n') {
            //add buffer to cargs
            //reset buffer
            //fork process
            //exec commands with cargs

            buff[i] = '\0';
            c_args[k++] = malloc(i);
            strcpy(c_args[k-1], buff);
            c_args[k] = 0;

            int pid = fork(), ret;

            if (pid == 0) {
                if ((ret = exec(cmd, c_args))) {
                    fprintf(2, "EXEC FAILED result: %d\n", ret);
                    exit(1);
                }
            }

            wait(0);

            memset(buff, 0, 512);

            while(k > c_args_count) {
                free(c_args[--k]);
            }
            memset(c_args+c_args_count, 0, MAXARGS);
            i = 0;
            // k = c_args_count;
        }

        else {
            i++;
        }
    }

    if (i == 521) {
        printf("arg to large\n");
    }

    if (k == MAXARGS) {
        printf("args count too large\n");
    }

    exit(0);
}