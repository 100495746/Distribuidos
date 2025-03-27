#ifndef SERVIDOR_SOCK_H
#define SERVIDOR_SOCK_H

#include "claves.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// las colas de mensajes posix no reconocen punteros
typedef struct Peticion {
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = MODIFY
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
} Peticion;

typedef struct Respuesta {
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
    int status;
} Respuesta;


#endif // SERVIDOR_MQ_H