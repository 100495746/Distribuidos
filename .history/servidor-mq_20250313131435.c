#include "claves.h"
#include <stddef.h>
#include <pthread.h>
#include "proxy-mq.h"
#include <unistd.h>
#include "servidor-mq.h"

void *tratar_peticion(void *arg) {
    Peticion *peticion = (Peticion *)arg;
    printf("Handling request: tipo_operacion=%d, key=%d\n", peticion->tipo_operacion, peticion->key);
    // Your existing code to handle the request
    // ...
    return NULL;
}

int main(int argc, char *argv[]) {
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