#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"
#include "check_helpers.c"

void test_correct_ps(void) {
    printf("[test_correct_ps] ");
    int BUFF_SIZE = 8;
    procinfo_t buf[BUFF_SIZE];
    int num_of_processes = ps_listinfo(buf, BUFF_SIZE);

    printf("(Num of processes: %d) ", num_of_processes);
    myassert(num_of_processes > 0 && num_of_processes <= BUFF_SIZE, "Correct test failed.\n");
    printf("OK\n");
}

void test_small_buffer_err(void) {
    printf("[test_small_buffer_err] ");
    int BUFF_SIZE = 1;
    procinfo_t buf[BUFF_SIZE];
    int num_of_processes = ps_listinfo(buf, BUFF_SIZE);

    myassert(num_of_processes > BUFF_SIZE, "Small buffer test failed.\n");
    printf("OK\n");
}

void test_nullptr_buffer_err(void) { // Если даже указан NULLPTR в качестве буфера, возвращать количество процессов надо
    printf("[test_nullptr_buffer_err] ");
    int BUFF_SIZE = 64;
    int num_of_processes = ps_listinfo(0, BUFF_SIZE);

    printf("(Num of processes: %d) ", num_of_processes);
    myassert(num_of_processes > 0 && num_of_processes <= BUFF_SIZE, "NULLPTR buffer test failed.\n");
    printf("OK\n");
}

void test_wrong_address(void) {
    printf("[test_wrong_address] ");
    int num_of_processes = ps_listinfo(0x0, 64);

    myassert(num_of_processes > 0, "Wrong address test failed.\n");
    printf("OK\n");
}

int main() {
    printf("===== PS tests started =====\n");
    test_correct_ps();
    test_small_buffer_err();
    test_nullptr_buffer_err();
    test_wrong_address();
    printf("===== All tests passed successfully! =====\n");
    exit(0);
}