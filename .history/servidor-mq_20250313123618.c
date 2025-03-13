#include "claves.h"
#include <stddef.h>
#include <pthread.h>
#include "proxy-mq.h"
#include <unistd.h>
#


Respuesta _set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Respuesta resp;
    if(proxy_set_value(key, value1, N_value2, V_value2, value3) == 0){
        resp.key = key;
        strcpy(resp.value1, value1);
        resp.N_value2 = N_value2;
        memcpy(resp.V_value2, V_value2, N_value2*sizeof(double));
        resp.value3 = value3;
        resp.status = 0;
        return resp;
    }
    else{
        resp.status = -1;
        return resp;
    }
}

Respuesta _get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    Respuesta resp;
    if(get_value(key, value1, N_value2, V_value2, value3) == 0){
        resp.key = key;
        strcpy(resp.value1, value1);
        resp.N_value2 = *N_value2;
        memcpy(resp.V_value2, V_value2, (*N_value2)*sizeof(double));
        resp.value3 = *value3;
        resp.status = 0;
        return resp;
    }
    else{
        resp.status = -1;
        return resp;
    }
}

Respuesta _modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Respuesta resp;
    if(modify_value(key, value1, N_value2, V_value2, value3) == 0){
        resp.key = key;
        strcpy(resp.value1, value1);
        resp.N_value2 = N_value2;
        memcpy(resp.V_value2, V_value2, N_value2*sizeof(double));
        resp.value3 = value3;
        resp.status = 0;
        return resp;
    }
    else{
        resp.status = -1;
        return resp;
    }
}

void *tratar_peticion(void *arg){
    Peticion *peticion = (Peticion *)arg;
    Respuesta respuesta;
    char cola_respuesta[256];
    snprintf(cola_respuesta, sizeof(cola_respuesta), "/cola_respuesta_%d", peticion->cola_respuesta);

    switch(peticion->tipo_operacion){
        case 1: 
            respuesta =_set_value(peticion->key, peticion->value1, peticion->N_value2, peticion->V_value2, peticion->value3);
            break;
        case 2:
            respuesta = _get_value(peticion->key, peticion->value1, &peticion->N_value2, peticion->V_value2, &peticion->value3);
            break;
        case 3:
            respuesta = _modify_value(peticion->key, peticion->value1, peticion->N_value2, peticion->V_value2, peticion->value3);
            break;        
    }
    mqd_t cliente;
    cliente = mq_open(cola_respuesta, O_WRONLY, 0666, &atributos_respuesta);
    if(cliente == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-2);
    }
    if(mq_send(cliente, (char *)&respuesta, sizeof(Respuesta), 0) == -1){
        perror("Error en mq_send");
        exit(-2);
    }
    mq_close(cliente);
    return NULL;
}


int main(){
    mqd_t server;
    server = mq_open(PETICIONES, O_CREAT | O_RDONLY, 0666, &atributos_peticion);
    if(server == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-2);
    }
    while(1){
        Peticion peticion;
        if(mq_receive(server, (char *)&peticion, sizeof(Peticion), NULL) == -1){
            perror("Error en mq_receive");
            exit(-2);
        }
        // tratamos las peticion concurrentemente 
        pthread_t esclavo;
        pthread_create(&esclavo, NULL, tratar_peticion, &peticion);
        pthread_detach(esclavo);         
                
    }
}
