#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    int fd;

    // Crear un archivo
    fd = open("testfile", O_CREATE | O_RDWR);
    if (fd < 0) {
        printf("Error: No se pudo crear el archivo\n");
        exit(1);
    }
    write(fd, "hola", 4);
    close(fd);

    // Cambiar a solo lectura
    if (chmod("testfile", 1) < 0) {
        printf("Error: No se pudo cambiar a solo lectura\n");
        exit(1);
    }

    // Intentar abrir en modo escritura (debe fallar)
    fd = open("testfile", O_WRONLY);
    if (fd >= 0) {
        printf("Error: Permiso de escritura no debería estar permitido\n");
        close(fd);
        exit(1);
    }

    // Cambiar a inmutable
    if (chmod("testfile", 5) < 0) {
        printf("Error: No se pudo cambiar a inmutable\n");
        exit(1);
    }

    // Intentar modificar permisos (debe fallar)
    if (chmod("testfile", 3) == 0) {
        printf("Error: Cambió permisos de archivo inmutable\n");
        exit(1);
    }

    printf("Pruebas completadas con éxito\n");
    exit(0);
}