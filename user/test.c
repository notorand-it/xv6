#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void check_err(int t, char* msg) {
    if (t < 0) {
        fprintf(2, msg);
        exit(1);
    }
}

void create_file(char* name, char data) {
    int fd = open(name, O_CREATE | O_WRONLY);
    check_err(fd, "cannot open file\n");
    check_err(write(fd, &data, 1), "cannot write in file\n");
    close(fd);
}

void verify(char* buf, char* inf, int ok) {
    int fd = open(inf, O_RDONLY);
    if (fd < 0 && ok < 0) return;

    check_err(ok, "file should not be opened\n");
    check_err(fd, "file should be opened\n");

    char read1;
    read(fd, &read1, 1);
    close(fd);

    fd = open(buf, O_RDONLY);
    check_err(fd, "can't open file\n");
    char read2;
    read(fd, &read2, 1);
    close(fd);

    if (read1 != read2) {
        fprintf(2, "wrong file\n");
        exit(1);
    }
}

void check_mkdir() {
    check_err(mkdir("m"), "fs error\n");
    check_err(mkdir("m/b"), "fs error\n");
    check_err(mkdir("m/b/r"), "fs error\n");
    check_err(mkdir("m/b/r/x"), "fs error\n");
    check_err(mkdir("m/b/r/x/x2"), "fs error\n");
}

void check_links() {
    create_file("/m/b/r/1", '1');
    create_file("/m/2", '2');
    create_file("/m/b/r/x/x2/3", '3');
    check_err(symlink("/m/b/r/1", "/m/b/r/b1"), "symlink error\n");
    check_err(symlink("/m/b/r/b1", "/m/b/r/bb1"), "symlink error\n");
    check_err(symlink("/m/b/r/bb1", "/m/b/r/bb2"), "symlink error\n");
    check_err(symlink("/m/b/r/o1", "/m/b/r/ob1"), "symlink error\n");
    check_err(symlink("/m/b/r/ob1", "/m/b/r/ob2"), "symlink error\n");
    check_err(chdir("m/b/r"), "error in dir\n");
    check_err(symlink("./1", "o1"), "symlink error\n");
    check_err(symlink("o1", "oo1"), "symlink error\n");
    check_err(symlink("oo1", "oo2"), "symlink error\n");
    check_err(symlink("../../2", "o2"), "symlink error\n");
    check_err(symlink("x/x2/3", "o3"), "symlink error\n");
    check_err(symlink("zz", "zz"), "symlink error\n");
    check_err(symlink("g1", "g2"), "symlink error\n");
    check_err(symlink("g2", "g3"), "symlink error\n");
    check_err(symlink("g3", "g1"), "symlink error\n");
    check_err(symlink("10", "nxt"), "symlink error\n");
    check_err(symlink("2", "cr"), "symlink error\n");
    check_err(symlink("../../1", "up"), "symlink error\n");
    check_err(symlink("x/x2/1", "dwn"), "symlink error\n");
    char* links[] = {"/m/b/r/b1", "/m/b/r/bb1", "/m/b/r/bb2", "/m/b/r/ob1", "/m/b/r/ob2"};
    char* sublinks[] = {"o1", "oo1", "oo2"};
    char* incorrect[] = {"zz", "g2", "g3", "g1", "nxt", "cr"};
    verify("../../2", "o2", 0);
    verify("x/x2/3", "o3", 0);
    check_err(chdir("../../.."), "error in dir\n");
    for (int i = 0; i < 5; ++i) verify("/m/b/r/1", links[i], 0);
    check_err(chdir("m/b/r"), "error in dir\n");
    for (int i = 0; i < 3; ++i) verify("1", sublinks[i], 0);
    for (int i = 0; i < 6; ++i) verify("", incorrect[i], -1);
    printf("Success\n");
}

void check_pids() {
    char* dirs[] = {"/", "/m", "/m/b", "/m/b/r", "/m/b/r/x", "/m/b/r/x/x2"};
    char* commands[] = {"/ls", "../ls", "../../ls", "../../../ls", "../../../../ls", "../../../../../ls"};
    for (int i = 0; i < 6; ++i) {
        int pid = fork();
        if (pid == 0) {
            check_err(chdir(dirs[i]), "error id dir\n");
            char* arr[] = {0};
            exec(commands[i], arr);
            fprintf(2, "exec fail\n");
            exit(1);
        }
        else {
            int status;
            wait(&status);
        }
    }
}

int main() {
    check_mkdir();
    check_links();
    check_pids();
    exit(0);
}
