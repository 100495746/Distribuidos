#include "proxy_cliente.h"

Respuesta send_rcv(Peticion *peticion){
    mqd_t mp, mr;
    Respuesta respuesta;
    mp = mq_open(QUEUE_P, O_WRONLY, 0666, &atributos_cliente);
    if(mp == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-1);
    }
    if(mq_send(mp, (char *)peticion, sizeof(Peticion), 0) == -1){
        perror("Error en mq_send");
        exit(-1);
    }
    mq_close(mp);
    mr = mq_open(????, O_RDONLY, 0666, &atributos_servidor);
    if(mr == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-1);
    }
    if(mq_receive(mr, (char *)&respuesta, sizeof(Respuesta), NULL) == -1){
        perror("Error en mq_receive");
        exit(-1);
    }
    mq_close(mr);
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

    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}
