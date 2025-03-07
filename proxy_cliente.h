#define proxy_cliente_H
#include "claves.h"
#include <stddef.h>
#include <mqueue.h>  // Biblioteca POSIX de colas de mensajes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // Para O_CREAT, O_WRONLY, etc.
#include <sys/stat.h>   // Para permisos

// las colas de mensajes posix no reconocen punteros
typedef struct peticion{
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = MODIFY
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
} Peticion;

typedef struct respuesta{
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = MODIFY
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
    int status;
} Respuesta;
