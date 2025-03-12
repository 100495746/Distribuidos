#include "proxy_cliente.h"

Respuesta send_rcv(Peticion *peticion){
    mqd_t server, cliente;
    Respuesta respuesta;
    server = mq_open(PETICIONES, O_WRONLY, 0666, &atributos_peticion);
    if(server == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-1);
    }
    if(mq_send(server, (char *)peticion, sizeof(Peticion), 0) == -1){
        perror("Error en mq_send");
        exit(-1);
    }
    mq_close(server);
    cliente = mq_open(peticion->cola_respuesta, O_CREAT|O_RDONLY, 0666, &atributos_respuesta);
    if(cliente == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-1);
    }
    // Esperar hasta que reciba la respuesta añadir un bucle o algo
    if(mq_receive(cliente, (char *)&respuesta, sizeof(Respuesta), NULL) == -1){
        perror("Error en mq_receive");
        exit(-1);
    }
    mq_close(cliente);
    return respuesta;


}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Peticion peticion;
    peticion.tipo_operacion = 1;
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, N_value2*sizeof(double));
    peticion.value3 = value3;
    peticion.cola_respuesta = getpid();

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    Peticion peticion;
    peticion.tipo_operacion = 2;
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, N_value2*sizeof(double));
    peticion.value3 = value3;
    peticion.cola_respuesta = getpid();

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Peticion peticion;
    peticion.tipo_operacion = 3;
    peticion.key = key;
    strcpy(peticion.value1, value1);
    peticion.N_value2 = N_value2;
    memcpy(peticion.V_value2, V_value2, N_value2*sizeof(double));
    peticion.value3 = value3;
    peticion.cola_respuesta = getpid();

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}
