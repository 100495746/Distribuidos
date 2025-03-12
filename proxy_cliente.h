#define proxy_cliente_H
#include "claves.h"
#include <stddef.h>
#include <mqueue.h>  // Biblioteca POSIX de colas de mensajes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // Para O_CREAT, O_WRONLY, etc.
#include <sys/stat.h>   // Para permisos
#define PETICIONES "/servidor"

// las colas de mensajes posix no reconocen punteros
typedef struct Peticion{
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = MODIFY
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
    pid_t cola_respuesta; // PID del cliente que indicara la cola donde el servidor tiene que responder
} Peticion;

typedef struct Respuesta{
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = MODIFY
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
    int status;
} Respuesta;

struct mq_attr atributos_peticion = {
    .mq_flags = 0,          
    .mq_maxmsg = 10,        // Máximo número de mensajes en la cola
    .mq_msgsize = sizeof(Peticion), // Se elige Respuesta como tamaño de mensaje porque es el mayor
    .mq_curmsgs = 0         
};

struct mq_attr atributos_respuesta = {
    .mq_flags = 0,          
    .mq_maxmsg = 10,        // Máximo número de mensajes en la cola
    .mq_msgsize = sizeof(Respuesta), // Se elige Respuesta como tamaño de mensaje porque es el mayor
    .mq_curmsgs = 0         
};

