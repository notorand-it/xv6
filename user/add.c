#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"

int all_digits_and_space_and_sign(char* str) {
    char digits_and_space[12] = "-0123456789 ";
    for (int i = 0; i < strlen(str); ++i) {
        for (int j = 0; j < strlen(digits_and_space); ++j) {
            if (str[i] == digits_and_space[j]) {
                break;
            }
            if (j == strlen(digits_and_space) - 1) {
                return 0;
            }
        }
    }
    return 1;
}

int number_space_number(char* str) {
    int count_spaces = 0, space_pos = 0, count_minus = 0;
    for (int i = 0; i < strlen(str); ++i)
    {
        if (str[i] == ' ') {
            space_pos = i;
            count_spaces++;
        }
        else if (str[i] == '-') {
            count_minus++;
        }
    }

    if (count_spaces != 1 || str[0] == ' ' || str[strlen(str) - 1] == ' ') {
        return 0;
    }

    if (count_minus > 2) return 0;

    if (count_minus == 2 && str[0] == '-' && str[space_pos + 1] == '-') return 1;
    if (count_minus == 1 && (str[0] == '-' || str[space_pos + 1] == '-')) return 1;
    if (count_minus == 0) return 1;

    return 0;
}

int main() {
    char buffer[24]; // 1 (знак числа) + 10 (максимальная длина int) + 1 (пробел) + 1 (знак числа) + 10 (максмальная длина int) + 1 (для хранения '\0')
    char c;
    int i = 0, count;

    while ((count = read(0, &c, 1)) != 0) {
        if (c == '\n') {
            break;
        }
        if (i == 128) {
            printf("Ошибка: буфер переполнен!\n");
            exit(1);
        }
        if (count < 0) {
            printf("Ошибка: не удалось считать строку!\n");
            exit(1);
        }
        buffer[i] = c;
        ++i;
    }

    buffer[i] = '\0';

    printf("|%s|\n", buffer);

    if (!all_digits_and_space_and_sign(buffer)) {
        printf("Ошибка: строка состоит не из цифр, пробелов и минусов!\n");
        exit(1);
    }
    if (!number_space_number(buffer)) {
        printf("Ошибка: строка не имеет вид |число число|\n");
        exit(1);
    }

    int minus = 0;
    if (buffer[0] == '-') {
        buffer[0] = '0';
        minus = 1;
    }
    int first_num = atoi(buffer);
    if (minus) first_num = -first_num;

    char curr[128];
    int flag = 0, k = 0;

    for (int j = 0; j < i; ++j) {
        if (flag) {
            curr[k] = buffer[j];
            ++k;
        }
        if (buffer[j] == ' ') {
            flag = 1;
            continue;
        }
    }

    minus = 0;
    if (curr[0] == '-') {
        curr[0] = '0';
        minus = 1;
    }
    int second_num = atoi(curr);
    if (minus) second_num = -second_num;

    printf("%d\n", add(first_num, second_num));
    exit(0);
}