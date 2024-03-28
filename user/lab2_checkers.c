#ifndef XV6_RISCV_OS_LAB2_CHECKERS_C
#define XV6_RISCV_OS_LAB2_CHECKERS_C

#include "check_helpers.c"

void fork_check(int pid) {
    if (pid < 0) raise_err("Fork error.\n");
}

void pipe_check(int pipe_code) {
    if (pipe_code < 0) raise_err("Pipe error.\n");
}

void kill_check(int kill_status) {
    if (kill_status < 0) {
        raise_err("THIS CHILD IS IMMORTAL!!!\n");
    }
}

void check_read_status(int read_status) {
    if (read_status == -1) {
        raise_err("Error occurred while reading input.\n");
    }
}

void check_write_status(int write_status, int bytes_to_write) {
    if (write_status != bytes_to_write) {
        raise_err("Write error.\n");
    }
}

#endif