#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[512];

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_valid_number(const char *str) {
    if (*str == '-') {
        str++; 
    }
    while (*str) {
        if (!is_digit(*str) && *str != '\0') {
            return 0; 
        }
        str++;
    }
    return 1;  
}


int
main(int argc, char *argv[])
{
    int n;
    int num1, num2;
    int sign1 = 1, sign2 = 1;

    printf("Enter two numbers: ");

    n = read(0, buf, sizeof(buf) - 1);
    if (n < 0) {
        printf("Read error. Code: %d\n", n);
        exit(-2);
    }

    buf[n - 1] = '\0';

    char *space_ptr = buf;
    while (*space_ptr != ' ' && *space_ptr != '\0') {
        space_ptr++;
    }

    if (*space_ptr != ' ') {
        printf("Incorrect Input. Enter two numbers separated by a space.\n");
        exit(-1);
    }

    *space_ptr = '\0';  
    char *first_num = buf;
    char *second_num = space_ptr + 1;

    if (!is_valid_number(first_num) && !is_valid_number(second_num)) {
        printf("Incorrect Input. Enter two numbers separated by a space.\n");
        exit(-1);
    }

    if (*first_num == '-') {
        sign1 = -1;
        first_num++;
    }
    if (*second_num == '-') {
        sign2 = -1;
        second_num++;
    }

    num1 = sign1 * atoi(first_num);
    num2 = sign2 * atoi(second_num);

    int result = add(num1, num2);

    printf("%d\n", result);

    exit(0);
}