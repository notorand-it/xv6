#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"
#include "user/check_helpers.c"

int main() {
    char *user_buff = malloc(DIAG_MSG_BUFF_SIZE + 1);
    if (dmesg((uint64) user_buff, DIAG_MSG_BUFF_SIZE + 1) < 0) {
        raise_err("Somehow dmesg err occurred.");
    }
    printf("%s", user_buff);
    free(user_buff);
    return 0;
}