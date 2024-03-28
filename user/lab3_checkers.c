#ifndef XV6_RISCV_OS_LAB3_CHECKERS_C
#define XV6_RISCV_OS_LAB3_CHECKERS_C

#include "check_helpers.c"

void mutex_check(int ret_code) {
    myassert(ret_code >= 0, "Mutex error.\n");
}

#endif