#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

#define MAX_PATH 512

void get_path(char *path) {
    char buf[DIRSIZ + 1];
    struct dirent de;
    struct stat st;
    int fd;

    if ((fd = open(".", O_RDONLY)) < 0) {
        printf("pwd: cannot open .\n");
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        printf("pwd: cannot stat .\n");
        close(fd);
        exit(1);
    }

    if (st.ino == ROOTINO) {
        strcpy(path, "/");
        close(fd);
        return;
    }

    path[0] = '\0';
    while (st.ino != ROOTINO) {
        if (chdir("..") < 0) {
            printf("pwd: cannot change to ..\n");
            exit(1);
        }

        if ((fd = open(".", O_RDONLY)) < 0) {
            printf("pwd: cannot open ..\n");
            exit(1);
        }

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == st.ino) {
                memmove(buf, de.name, DIRSIZ);
                buf[DIRSIZ] = '\0';

                int len = strlen(path);
                if (len + strlen(buf) + 2 > MAX_PATH) {
                    printf("pwd: path too long\n");
                    close(fd);
                    exit(1);
                }

                // Shift current path to the right
                memmove(path + strlen(buf) + 1, path, len + 1);
                // Prepend the new directory
                path[0] = '/';
                memmove(path + 1, buf, strlen(buf));
                break;
            }
        }

        if (fstat(fd, &st) < 0) {
            printf("pwd: cannot stat ..\n");
            close(fd);
            exit(1);
        }
        close(fd);
    }

    if (path[0] == '\0') {
        strcpy(path, "/");
    }
}

int main(void) {
    char path[MAX_PATH];
    get_path(path);
    printf("%s\n", path);
    exit(0);
}
