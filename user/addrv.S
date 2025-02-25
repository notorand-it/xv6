.data
msg: .asciz "Incorrect format\n"
okmsg:  .asciz "ok\n"
buffer: .space 30

.text
.global main
main:
    la a0, buffer
    call gets
    mv s0, a0
    la t0, buffer
    li t1, 0

    li t4, 32
    li t5, 32
    jal t6, read_before_t4_or_t5


    li t4, 0
    li t5, 10
    jal t6, read_before_t4_or_t5

read_second_num:
    la t0, buffer
    add t0, t0, t1
    addi t0, t0, 1

    jal ra, check_for_minus
    mv a0, t0


    call atoi
    mv s1, a0
    beqz t1, read_fist_num
    sub s1, zero, s1

read_fist_num:
    la t0, buffer
    jal ra, check_for_minus
    mv a0, t0
    call atoi
    mv s1, a0
    beqz t1, myadd
    sub s1, zero, s1

myadd:
    add s2, s1, s2

    mv a0, s2
    call printf



read_before_t4_or_t5:
    jal ra, ok
    jal ra, check_for_minus
    lb t2, 0(t0)
    beq t2, t4, return_to_t6
    beq t2, t5, return_to_t6
    li t3, 58
    bge t2, t3, format_error
    li t3, 48
    blt t2, t3, format_error
    addi t0, t0, 1
    bne t4, t5, read_before_t4_or_t5 #if we look not for ' ' we don't count symbols
    addi t1, t1, 1
    jal zero, read_before_t4_or_t5

return_to_t6:
    jalr zero, t6, 0

check_for_minus:
    jal ra, ok

    lb t2, 0(t0)
    li t3, 45

    bne t2, t3, return_to_ra
    addi t0, t0, 1
    addi t1, t1, 1
    jalr zero, ra, 0

return_to_ra:
    jalr zero, ra, 0

format_error:
    li a0, 1
    la a1, msg
    call fprintf
    li a7, 2
    li a0, 1
    ecall

ok:
    li a0, 1
    la a1, okmsg
    call fprintf
    jalr zero, ra, 0