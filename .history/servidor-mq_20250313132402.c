#include "claves.h"
#include <stddef.h>
#include <pthread.h>
#include "proxy-mq.h"
#include <unistd.h>
#include "servidor-mq.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *tratar_peticion(void *arg) {
    Peticion *peticion = (Peticion *)arg;
    printf("Handling request: tipo_operacion=%d, key=%d\n", peticion->tipo_operacion, peticion->key);

    Respuesta respuesta;
    respuesta.status = 0; // Assume success

    // Handle the request based on tipo_operacion
    switch (peticion->tipo_operacion) {
        case 1: // SET
            respuesta.status = set_value(peticion->key, peticion->value1, peticion->N_value2, peticion->V_value2, peticion->value3);
            break;
        case 2: // GET
            respuesta.status = get_value(peticion->key, respuesta.value1, &respuesta.N_value2, respuesta.V_value2, &respuesta.value3);
            break;
        case 3: // MODIFY
            respuesta.status = modify_value(peticion->key, peticion->value1, peticion->N_value2, peticion->V_value2, peticion->value3);
            break;
        default:
            respuesta.status = -1; // Unknown operation
            break;
    }

    // Send the response back to the client
    char cola_respuesta[256];
    snprintf(cola_respuesta, sizeof(cola_respuesta), "/cola_respuesta_%d", peticion->cola_respuesta);
    mqd_t cliente = mq_open(cola_respuesta, O_WRONLY);
    if (cliente == (mqd_t)-1) {
        perror("Error en mq_open (client response)");
        return NULL;
    }

    if (mq_send(cliente, (char *)&respuesta, sizeof(Respuesta), 0) == -1) {
        perror("Error en mq_send (client response)");
    }

    mq_close(cliente);
    return NULL;
}

int main((int argc, char *argv[])) {
    mqd_t server;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Peticion);
    attr.mq_curmsgs = 0;

    printf("Starting server...\n");
    server = mq_open(PETICIONES, O_CREAT | O_RDONLY, 0666, &attr);
    if (server == (mqd_t)-1) {
        perror("Error en mq_open");
        exit(-1);
    }

    printf("Server started, waiting for requests...\n");
    while (1) {
        Peticion peticion;
        if (mq_receive(server, (char *)&peticion, sizeof(Peticion), NULL) == -1) {
            perror("Error en mq_receive");
            exit(-2);
        }
        printf("Received request: tipo_operacion=%d, key=%d\n", peticion.tipo_operacion, peticion.key);
        // tratamos las peticion concurrentemente 
        pthread_t esclavo;
        pthread_create(&esclavo, NULL, tratar_peticion, &peticion);
        pthread_detach(esclavo);         
    }

    mq_close(server);
    return 0;
}