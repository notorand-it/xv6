#include <kernel/syscall.h>
#include "test.h"
#include <stdio.h>

EXPORT_SYMBOL uint64_t syscall_test(void) {
    printf("syscall_test\n");
    return 0;
}