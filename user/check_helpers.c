#ifndef XV6_RISCV_OS_CHECK_HELPERS_C
#define XV6_RISCV_OS_CHECK_HELPERS_C

const int STDERR_D = 2;

void raise_err(const char *err_msg) {
    write(STDERR_D, err_msg, strlen(err_msg));
    exit(1);
}

void myassert(const int true_statement, const char *err_msg) {
    if (!true_statement) raise_err(err_msg);
}

#endif