#include "proxy-mq.h"

Respuesta send_rcv(Peticion *peticion){
    // cambiar los perror por respuesta.status = -2 q es cuando falla el servidor
    mqd_t server, cliente;
    Respuesta respuesta;

    snprintf(cola_respuesta, sizeof(cola_respuesta), "/cola_respuesta_%d", peticion->cola_respuesta);


    server = mq_open(PETICIONES, O_WRONLY, 0666, &atributos_peticion);
    if(server == (mqd_t)-1){
        respuesta.status = -2;
        return respuesta;
    }
    if(mq_send(server, (char *)peticion, sizeof(Peticion), 0) == -1){
        respuesta.status = -2;
        return respuesta;
        exit(-2);
    }
    mq_close(server);

    cliente = mq_open(peticion->cola_respuesta, O_CREAT|O_RDONLY, 0666, &atributos_respuesta);
    if(cliente == (mqd_t)-1){
        respuesta.status = -2;
        return respuesta;
    }
    // Esperar hasta que reciba la respuesta añadir un bucle o algo
    if(mq_receive(cliente, (char *)&respuesta, sizeof(Respuesta), NULL) == -1){
        respuesta.status = -2;
        return respuesta;
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
    memcpy(peticion.V_value2, V_value2, (*N_value2)*sizeof(double));
    peticion.value3 = *value3;
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
