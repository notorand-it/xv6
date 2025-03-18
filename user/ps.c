#include "kernel/types.h"
#include "user/user.h"

void print(struct procinfo *pi){
    printf("%s: pid = %d, parent_pid = %d, state = ", pi->name, pi->pid, pi->ppid);
    switch(pi->state){
        case PI_UNUSED: printf("UNUSED\n"); break;
        case PI_USED: printf("USED\n"); break;
        case PI_SLEEPING: printf("SLEEPING\n"); break;
        case PI_RUNNABLE: printf("RUNNABLE\n"); break;
        case PI_RUNNING: printf("RUNNING\n"); break;
        case PI_ZOMBIE: printf("ZOMBIE\n"); break;
        default: printf("UNKNOWN\n"); break;
    }
}

int main(int argc, char* argv[]){
    struct procinfo* pl;
    int size = 1, result;
    for( ; ; size <<= 1, free(pl)){
        pl = malloc(sizeof(struct procinfo) * size);
        result = ps_listinfo(pl, size);
        if(result != -2 || size > 1000000)
            break;
    }
    if(result < 0){
        free(pl);
        fprintf(2, "ps_listinfo error\n");
        exit(1);
    }
    printf("%d processes in total:\n", result);
    for (int i = 0; i < result; ++i) {
        print(pl + i);
    }
    free(pl);
    exit(0);
}