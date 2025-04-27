#include "claves.h"
#include "claves_rpc.h"
#include <rpc/clnt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 256
#define MAX_ARRAY  32

/* Códigos de retorno adicionales */
#define RPC_ERROR -2  /* Error a nivel de librería RPC (timeout, servidor caído, etc.) */

static CLIENT *clnt = NULL;

/* Helper para conectar la primera vez */
static int init_client(void){
    if(clnt) return 0; /* ya conectado */

    char *ip = getenv("IP_TUPLAS");
    if(!ip){
        fprintf(stderr, "IP_TUPLAS no definida
");
        return RPC_ERROR;
    }

    clnt = clnt_create(ip, CLAVES_PROG, CLAVES_VERS, "tcp");
    if(!clnt){
        clnt_pcreateerror(ip);
        return RPC_ERROR;
    }
    /* Se podrían configurar timeouts aquí con clnt_control si se desea */
    return 0;
}

static int check_null(int *result){
    if(!result){
        /* fallo en la RPC: devolvemos -2 */
        return RPC_ERROR;
    }
    return *result; /* valor devuelto por el servidor (-1,0, etc.) */
}

int proxy_set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    if(init_client()!=0) return RPC_ERROR;

    SetArgs args;
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    args.V_value2.V_value2_len = N_value2;
    args.V_value2.V_value2_val = V_value2;
    args.value3 = value3;

    int *result = set_value_1(&args, clnt);
    return check_null(result);
}

int proxy_get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    if(init_client()!=0) return RPC_ERROR;

    KeyArg k = {.key = key};
    GetReply *reply = get_value_1(&k, clnt);
    if(!reply) return RPC_ERROR; /* fallo RPC */

    if(reply->status != 0){ /* error lógico devuelto por el servidor */
        return reply->status; // -1 o -2 provenientes del servidor
    }

    strncpy(value1, reply->value1, MAX_STRING);
    *N_value2 = reply->N_value2;
    for(int i=0;i<reply->N_value2 && i<MAX_ARRAY;i++){
        V_value2[i] = reply->V_value2.V_value2_val[i];
    }
    *value3 = reply->value3;

    xdr_free((xdrproc_t)xdr_GetReply, (char*)reply);
    return 0;
}

int proxy_modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    if(init_client()!=0) return RPC_ERROR;

    ModifyArgs args;
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    args.V_value2.V_value2_len = N_value2;
    args.V_value2.V_value2_val = V_value2;
    args.value3 = value3;

    int *result = modify_value_1(&args, clnt);
    return check_null(result);
}

int destroy(void){
    if(init_client()!=0) return RPC_ERROR;
    int *result = destroy_1(NULL, clnt);
    return check_null(result);
}

int delete_key(int key){
    if(init_client()!=0) return RPC_ERROR;
    KeyArg k={.key=key};
    int *result = delete_key_1(&k, clnt);
    return check_null(result);
}

int exist(int key){
    if(init_client()!=0) return RPC_ERROR;
    KeyArg k={.key=key};
    int *result = exist_1(&k, clnt);
    return check_null(result);
}