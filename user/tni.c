#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_nice_inheritance() {
    int parent = getpid();
    setnice(parent, 30); // Set high nice value for parent

    int child = fork();
    if (child == 0) {
        // Child process
        printf("Child nice value: %d\n", getnice(getpid()));
        exit(0);
    } else {
        wait(0);
    }
}

int main(int argc, char **argv) {
    printf("=== TEST NICE INHERITANCE START ===\n");
    test_nice_inheritance();
    printf("=== TEST NICE INHERITANCE END ===\n");
    exit(0);
}
