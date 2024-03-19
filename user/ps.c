#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

const char *states[] = {
        [UNUSED]    "unused",
        [USED]      "used",
        [SLEEPING]  "sleep ",
        [RUNNABLE]  "runble",
        [RUNNING]   "run   ",
        [ZOMBIE]    "zombie"
};

int main() {
    int BUFF_SIZE = 8;

    while (1) {
        procinfo_t buf[BUFF_SIZE];
        int num_of_processes = ps_listinfo(buf, BUFF_SIZE);
        if (num_of_processes == -1) {
            fprintf(2, "Writing to address failed.");
            exit(1);
        }
        if (num_of_processes <= BUFF_SIZE) {
            printf("===== %d processes =====\n", num_of_processes);
            for (int i = 0; i < num_of_processes; i++) {
                printf("[%s] name: %s, parent PID: %d\n", states[buf[i].state], buf[i].name, buf[i].parent_id);
            }
            break;
        }
        BUFF_SIZE <<= 1;
    }

    exit(0);
}