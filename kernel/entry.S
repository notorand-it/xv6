#include "param.h"
        # qemu -kernel loads the kernel at 0x80000000
        # and causes each hart (i.e. CPU) to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.
.section .text
.global _entry
_entry:
        # set up a stack for C.
        # stack0 is declared in start.c,
        # with a CPUSTACKSIZE-kilobytes stack per CPU as definied in param.h.
        # sp = stack0 + ((hartid + 1) * CPUSTACKSIZE)
        la sp, stack0
        li a0, 1024 * CPUSTACKSIZE
        csrr a1, mhartid
        addi a1, a1, 1
        mul a0, a0, a1
        add sp, sp, a0
        # jump to start() in start.c
        call start
spin:
        j spin
