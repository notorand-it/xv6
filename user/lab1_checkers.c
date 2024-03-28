#ifndef XV6_RISCV_OS_LAB1_CHECKERS_C
#define XV6_RISCV_OS_LAB1_CHECKERS_C

#include "check_helpers.c"

void check_read_status(int read_status) {
    if (read_status == -1) {
        raise_err("Error occurred while reading input.\n");
    }
}

void check_buffer_overflow(int bytes_read, int BUF_SIZE) {
    if (bytes_read >= BUF_SIZE) {
        raise_err("Buffer overflow.\n");
    }
}

// Останется ли место для второго аргумента
void check_for_space(int read_first_bytes, char *read_char, int BUF_SIZE) {
    if (read_first_bytes >= BUF_SIZE - 2 || *read_char == '\n' || *read_char == '\r' || *read_char == '\0') {
        raise_err("Not enough arguments.\n");
    }
}

#endif
