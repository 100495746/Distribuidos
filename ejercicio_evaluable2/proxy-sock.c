#include "proxy-mq.h"
#include <unistd.h>  // For getpid()

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

Respuesta send_rcv(Peticion *peticion) {
    // cambiar los perror por respuesta.status = -2 q es cuando falla el servidor
    mqd_t server, cliente;
    Respuesta respuesta;
    char cola_respuesta[256];

    // Convert pid_t to string for the message queue name
    snprintf(cola_respuesta, sizeof(cola_respuesta), "/cola_respuesta_%d", peticion->cola_respuesta);

    printf("Opening server queue...\n");
    server = mq_open(PETICIONES, O_WRONLY, 0666, &atributos_peticion);
    if (server == (mqd_t)-1) {
        perror("Error en mq_open (server)");
        respuesta.status = -2;
        return respuesta;
    }

    printf("Sending request to server...\n");
    if (mq_send(server, (char *)peticion, sizeof(Peticion), 0) == -1) {
        perror("Error en mq_send");
        respuesta.status = -2;
        mq_close(server);
        return respuesta;
    }
    mq_close(server);

    printf("Opening client queue: %s...\n", cola_respuesta);
    cliente = mq_open(cola_respuesta, O_CREAT | O_RDONLY, 0666, &atributos_respuesta);
    if (cliente == (mqd_t)-1) {
        perror("Error en mq_open (client)");
        respuesta.status = -2;
        return respuesta;
    }

    printf("Waiting for response from server...\n");
    if (mq_receive(cliente, (char *)&respuesta, sizeof(Respuesta), NULL) == -1) {
        perror("Error en mq_receive");
        respuesta.status = -2;
        mq_close(cliente);
        return respuesta;
    }
    mq_close(cliente);

    printf("Received response from server: status=%d\n", respuesta.status);
    return respuesta;
}

int proxy_set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    Peticion peticion;
    peticion.tipo_operacion = 1;
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, N_value2 * sizeof(double));
    peticion.value3 = value3;
    peticion.cola_respuesta = getpid();

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}

int proxy_get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    Peticion peticion;
    peticion.tipo_operacion = 2;
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = *N_value2;
    memcpy(peticion.V_value2, V_value2, (*N_value2) * sizeof(double));
    peticion.value3 = *value3;
    peticion.cola_respuesta = getpid();

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}

int proxy_modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    Peticion peticion;
    peticion.tipo_operacion = 3;
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, N_value2 * sizeof(double));
    peticion.value3 = value3;
    peticion.cola_respuesta = getpid();

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}