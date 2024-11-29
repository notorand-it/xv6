# Informe Oscar Lobo

## Funcionamiento del Sistema de Permisos
El sistema de permisos básicos implementado en xv6 utiliza una nueva función de sistema chmod para gestionar permisos de archivos. Este sistema permite definir restricciones de acceso para los archivos en los siguientes modos:

### Lectura/Escriptura: Permite operaciones tanto de lectura como escritura.
Solo Lectura: Restringe las operaciones de escritura en el archivo.
Inmutable: Marca el archivo como no modificable, tanto en contenido como en permisos.
Estas funcionalidades mejoran el control de acceso, garantizando la seguridad y consistencia de los archivos almacenados.

### chmod
La función chmod permite cambiar los permisos asociados a un archivo. Sus características principales incluyen:

### Actualizar los permisos de un archivo especificado a los valores definidos por el usuario.
Proteger archivos inmutables (perm = 5) contra cualquier tipo de modificación, tanto en contenido como en permisos.

## Modificaciones Realizadas

### Archivo sysfile.c
Se realizaron las siguientes modificaciones para respetar los permisos:

Función sys_open
Verifica los permisos del archivo antes de abrirlo:
Archivos de solo lectura (perm = 1) no pueden ser abiertos en modo escritura.
Archivos inmutables (perm = 5) solo pueden ser abiertos en modo lectura.
Función writei
Restringe las operaciones de escritura en archivos:
Si perm = 1 (solo lectura), la operación es denegada.
Si perm = 5 (inmutable), la operación también es denegada.
Archivo sysproc.c
Se implementó la función sys_chmod, que realiza las siguientes operaciones:

Cambia los permisos del archivo especificado a los valores definidos por el usuario.
Devuelve un error si el archivo es inmutable (perm = 5).

### Archivo syscall.h
Se añadió la definición de la nueva llamada al sistema


### Archivo usys.pl
Se agregó la entrada para la nueva función de sistema

### Archivo chmodetest.c
Se creó un programa de prueba para validar las funcionalidades implementadas. Este programa realiza las siguientes operaciones:

Creación del archivo:

Crea un archivo con permisos iniciales de lectura/escritura (O_CREATE | O_RDWR).
Escribe datos en el archivo y lo cierra.
Cambio a solo lectura:

Cambia los permisos del archivo a solo lectura (chmod(filename, 1)).
Intenta abrir el archivo en modo escritura, lo que debe fallar.
Restauración de permisos:

Cambia los permisos a lectura/escritura (chmod(filename, 3)).
Escribe nuevamente en el archivo para confirmar los permisos.
Prueba de inmutabilidad:

Cambia los permisos del archivo a inmutable (chmod(filename, 5)).
Intenta escribir en el archivo y modificar sus permisos, ambas operaciones deben fallar.
Manejo de Excepciones

### Cambios en sysfile.c:
Se añadieron validaciones para los permisos definidos en inode al manejar operaciones de apertura, lectura y escritura.
Manejo en chmod:
Si un archivo es inmutable, la función devuelve un error, bloqueando cambios en sus permisos.
Pruebas

## Descripción del Programa de Prueba
El programa chmodetest.c simula el uso de las nuevas funcionalidades de permisos en xv6. Sus pasos son:

Crear un archivo con permisos de lectura/escritura.
Cambiar los permisos a solo lectura y verificar que la escritura sea denegada.
Restaurar los permisos y confirmar que la escritura sea posible.
Cambiar los permisos a inmutable y verificar que no se pueda ni escribir ni modificar sus permisos.
Resultados de las Pruebas
Archivos con permisos de solo lectura restringen correctamente la escritura.
Archivos inmutables bloquean modificaciones tanto en contenido como en permisos.
El sistema genera mensajes de error apropiados cuando se violan las restricciones.
Dificultades Encontradas y Soluciones
Desalineación de la estructura inode
La inclusión del campo perm en el inode aumentó su tamaño, rompiendo la alineación requerida con BSIZE. Esto generó errores durante la inicialización del sistema de archivos. La solución fue añadir un campo de relleno para garantizar que la estructura sea múltiplo de BSIZE.

## Dificultades Encontradas y Soluciones

### Manejo de errores
surgieron muchos errores al intentar hacer la tarea, los cuales tuve que apoyarme de otros compañeros para soluconarlos, ya que no encontraba solución en internet.

### Validación de permisos
Inicialmente, las operaciones de escritura y apertura no verificaban correctamente los permisos. Este problema se resolvió añadiendo validaciones explícitas en las funciones sys_open y writei.

