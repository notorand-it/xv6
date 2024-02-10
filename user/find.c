#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

void find(char* dir_path, char* file_name) {
    int fd = open(dir_path, 0);
    if (fd < 1) {
        printf("failed to open %s\n", dir_path);
        return;
    }

    char* buf = malloc(512);
    char* ptr;
    int buf_size = 512;

    struct dirent dir_entry;
    struct stat st;

    if (strlen(dir_path) > buf_size - 2) { //considering / and \0 that will be added to buffer
        printf("find: dir path is too long\n");
        goto exit;
    }

    strcpy(buf, dir_path);
    ptr = buf+strlen(buf); //=&buf[strlen(buf)]
    *ptr++ = '/'; //ptr will reference address after '/' in buf

    // printf("Searching in: %s\n", buf);

    while(read(fd, &dir_entry, sizeof(dir_entry)) == sizeof(dir_entry)) {
        if(dir_entry.inum == 0)
            continue;

        if(strcmp(dir_entry.name, ".") == 0 || strcmp(dir_entry.name, "..") == 0) {
            continue;
        }

        // dir_path/dir_entry_name
        if (strlen(dir_path) + 1 + strlen(dir_entry.name) > buf_size-1) {
            printf("find: generated path is too long\n");
            goto exit;
        }

        //copy dir_entry_name to buf after '/'
        strcpy(ptr, dir_entry.name);
        
        if(stat(buf, &st) < 0){
            printf("[%s]: cannot stat %s\n", dir_path, buf);
            continue;
        }

        switch (st.type) {
            case T_DIR:
                find(buf, file_name);
                break;

            case T_FILE:
            case T_DEVICE:
                if(strcmp(dir_entry.name, file_name) == 0) {
                    printf("%s\n", buf);
                }
                break;
            
            default:
                continue;
        }
    }

    exit:
        close(fd);
        free(buf);
}

int main(int argc, char* argv[]) {

    // for quick testing
    // mkdir("a");
    // int fd = open("./a/b", O_CREATE);
    // close(fd);

    if(argc < 3){
        find(".", argv[1]);
        exit(0);
    }

    find(argv[1], argv[2]);

    exit(0);
}