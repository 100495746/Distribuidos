#ifndef PROXY_MQ_H
#define PROXY_MQ_H

#include "claves.h"
#include <stddef.h>
#include <mqueue.h>  // Biblioteca POSIX de colas de mensajes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // Para O_CREAT, O_WRONLY, etc.
#include <sys/stat.h>   // Para permisos
#include <sys/types.h>  // Para pid_t

#define PETICIONES "/servidor"

// las colas de mensajes posix no reconocen punteros
typedef struct Peticion {
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = MODIFY
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
    pid_t cola_respuesta; // PID del cliente que indicara la cola donde el servidor tiene que responder
} Peticion;

typedef struct Respuesta {
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
    int status;
} Respuesta;

extern struct mq_attr atributos_peticion;
extern struct mq_attr atributos_respuesta;

int proxy_set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int proxy_get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3);
int proxy_modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);

#endif // PROXY_MQ_H
