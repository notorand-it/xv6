#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main ()
{
    printf("=== TEST START ===\n");
    int pid1 = fork();
    int a = 1;
    int range = 100000;


    // parent process
    if(pid1>0){
        setnice(getpid(),0);
        for(int i = 0; i< range; i++){
            for(int j = 0; j< range; j++){
	    a = a * 10 + 1;
            }
        }
	ps(0);

        wait(0);
    }
    else if(pid1 == 0){
        setnice(getpid(),10);
        for(int i = 0; i< range; i++){
            for(int j = 0; j< range; j++){
            //a = 9.9+9.9*a;
	    a = a * 10 + 1;
            }
        }
	exit(0);
    }
    exit(0);
}
