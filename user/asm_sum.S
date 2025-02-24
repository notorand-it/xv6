.data

write_sum: .asciz "Sum: %d\n"
write_str: .asciz "|%s|\n"
write_err: .asciz "Error: %s\n"
err1: .asciz "First number is empty"
err2: .asciz "Second number is empty"
err3: .asciz "Buffered is overflowed"
err4: .asciz "More than 1 space"

#define MAX_LEN 50

buf:      .space  MAX_LEN

.text
.global main


main:
    la a0, buf
    li a1, MAX_LEN
    call gets

    li t0, '\n'
    la a1, buf
    li t2, 0
    li t3, MAX_LEN-1

find_endline:
    lbu t1, (a1)
    beq t1, zero, next_step
    beq t1, t0, del_n
    addi a1, a1, 1
    addi t2, t2, 1
    beq t2, t3, overflow_error
    j find_endline

del_n:
    sb zero, (a1)

next_step:
    la a1, buf
    lbu t1, (a1)
    li t0, ' '
    beq t1, t0, error1

write:
    la a0, write_str
    la a1, buf
    call printf

    la a1, buf
    li t0, ' '

split:
    lbu t1, (a1)
    beq t1, zero, error2
    beq t1, t0, del_space
    addi a1, a1, 1
    j split

del_space:
    sb zero, (a1)
    addi a1, a1, 1

    lbu t1, (a1)
    beq t1, zero, error2

    li t0, ' '
    beq t1, t0, error4

    la a0, buf
    call atoi
    mv s1, a0

    mv a0, a1
    call atoi
    mv s2, a0

    mv a1, zero
    add a1, s1, s2
    la a0, write_sum
    call printf

    li a7, 2
    li a0, 0
    ecall


error1:
    li a0, 2
    la a1, write_err
    la a2, err1
    call fprintf
    li a7, 2
    li a0, 1
    ecall

error2:
    li a0, 2
    la a1, write_err
    la a2, err2
    call fprintf
    li a7, 2
    li a0, 1
    ecall

overflow_error:
    li a0, 2
    la a1, write_err
    la a2, err3
    call fprintf
    li a7, 2
    li a0, 1
    ecall

error4:
    li a0, 2
    la a1, write_err
    la a2, err4
    call fprintf
    li a7, 2
    li a0, 1
    ecall
