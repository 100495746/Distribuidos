#include "claves.h"
#include <stddef.h>
#include <pthread.h>
#include "proxy_cliente.h"

int _set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    if(set_value(key, value1, N_value2, V_value2, value3) == 0){
        return 0;
    }
    else{
        return -1;
    }

int tratar_peticion(Peticion *peticion){
    switch(peticion->tipo_operacion){
        case 1: 
            _set_value(peticion->key, peticion->value1, peticion->N_value2, peticion->V_value2, peticion->value3);
            break;

            
    }
}


int main(int argc, char *argv){
    mqd_t mq;
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &atributos_cliente);
    if(mq == (mqd_t)-1){
        perror("Error en mq_open");
        exit(-1);
    }
    while(1){
        Peticion peticion;
        if(mq_receive(mq, (char *)&peticion, sizeof(Peticion), NULL) == -1){
            perror("Error en mq_receive");
            exit(-1);
        }
        
                
                
    }
}
