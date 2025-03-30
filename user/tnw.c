#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int run_one_second_work(){
    int start_time;
    int x,z,i,j;
    start_time = uptime();
    x = 0;
    z = 20;
    
    while((uptime() - start_time) < 100){
	for(i = 0; i < 1000000; i++){
	    for(j = 0; j < 10000; j++){
		x += z * 89;
	    }
	}
    }

    return x;
}


void test_nice_weight() {
    int parent = getpid();
    setnice(parent, 5); // Set parent's nice value to 5

    int child = fork();
    if (child == 0) {
        // Child process
        setnice(getpid(), 20); // Set child's nice value to 20
        for(int i = 0; i < 10; i++)
		run_one_second_work();
        exit(0);
    } else {
        // Parent process
        
        for(int i = 0; i < 10; i++){
		run_one_second_work();
		ps(0);
	}
        
        wait(0);
    }
}

int main(int argc, char **argv) {
    printf("=== TEST NICE WEIGHT START ===\n");
    test_nice_weight();
    printf("=== TEST NICE WEIGHT END ===\n");
    exit(0);
}

