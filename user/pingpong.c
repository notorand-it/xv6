#include "kernel/types.h"
#include "user/user.h"

#define EXIT_FAILURE 1
#define ONE_SECOND 10 // for some reason a second is 10 ticks in the qemu we run xv6 in

void perror(char* message) {
    fprintf(2, message);
}

int main() {
    int p1[2], p2[2], n_read = 0;
    char packet = 255;

    // Create a pipe
    if (pipe(p1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Create a pipe
    if (pipe(p2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    int start_time = uptime(), end_time, current_time = 0, measure_start;
    int parent_pid = getpid();

    int complete_exchanges = 0, complete_exchanges_per_sec = 0, run_limit_seconds = ONE_SECOND * 1;
    
    if(fork() == 0) {
        // In child process
        int child_pid = getpid();
        close(p1[1]);
        while(1) {
            n_read = read(p1[0], &packet, 1);
            if(n_read == 0) {
                close(p1[0]); //close read in pipe
                close(p2[1]); //close write out pipe
                break;
            }
            printf("%d: received ping\n", child_pid);
            write(p2[1], &packet, 1);
        }
    } else {
        // In parent process
        close(p2[1]);
        start_time = uptime();
        measure_start = start_time;

        while(1) {
            write(p1[1], &packet, 1);
            n_read = read(p2[0], &packet, 1);
            complete_exchanges++;
            complete_exchanges_per_sec++;

            printf("%d: received pong\n", parent_pid);
            // printf("%d: received pong. %d %d %d\n", parent_pid, start_time, measure_start, current_time);

            current_time = uptime();

            // Measure after every second of exchanges
            if (current_time - measure_start >= ONE_SECOND)
            {
                printf("bandwidth: %d B/s\n", complete_exchanges_per_sec);
                
                //reset measurement variables
                complete_exchanges_per_sec = 0;
                measure_start = uptime();
            }

            if (current_time - start_time >= run_limit_seconds || complete_exchanges > 10)
            {
                int runtime = current_time - start_time;
                printf("Exchange runtime: %d seconds\n", runtime/10);
                close(p1[1]); //Close write out pipe
                close(p2[0]); //Close read in pipe
                break;
            }
        }
    }
    
    wait(0);
    
    if(getpid() == parent_pid) {
        // In parent process
        end_time = uptime();

        int runtime = end_time - start_time;
        printf("Process runtime: %d seconds\n", runtime/10);
    }

    exit(0);
}
