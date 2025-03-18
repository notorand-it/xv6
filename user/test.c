#include "kernel/types.h"
#include "user/user.h"

#define check(x) if(x){printf("ok\n");return 1;}printf("wrong\n");return 0;

int test_null() {
    printf("Null: ");
    int result = ps_listinfo((struct procinfo *) 0, 0);
    check(result > 0)
}

int test_small_limit() {
    printf("Small limit: ");
    struct procinfo pl[1];
    int result = ps_listinfo(pl, 0);
    check(result == -2)
}

int test_wrong_address() {
    printf("Wrong address: ");
    int result = ps_listinfo((struct procinfo *) 1, 1000000000);
    check(result == -1)
}

int test_casual() {
    printf("Casual: ");
    struct procinfo* pl;
    int size = 1, result;
    for( ; ; size <<= 1){
        pl = malloc(sizeof(struct procinfo) * size);
        result = ps_listinfo(pl, size);
        free(pl);
        if(result != -2 || size > 1000000)
            break;
    }
    check(result > 0 && result < size)
}

int main(int argc, char *argv[]) {
    int flag[4];
    for (int i = 0; i < 4; ++i) {
        flag[i] = argc <= 1 ? 1 : 0;
    }
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-null") == 0)
            flag[0] = 1;
        else if (strcmp(argv[i], "-sml") == 0)
            flag[1] = 1;
        else if (strcmp(argv[i], "-wrad") == 0)
            flag[2] = 1;
        else if (strcmp(argv[i], "-cas") == 0)
            flag[3] = 1;
    }
    int passed = 0, total = 0;
    if (flag[0] == 1)
        total++, passed += test_null();
    if (flag[1] == 1)
        total++, passed += test_small_limit();
    if (flag[2] == 1)
        total++, passed += test_wrong_address();
    if (flag[3] == 1)
        total++, passed += test_casual();
    printf("passed %d/%d\n", passed, total);
}