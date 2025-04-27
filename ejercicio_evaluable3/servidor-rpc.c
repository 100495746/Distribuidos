#include "claves_rpc.h"
#include "claves.h"
#include <rpc/rpc.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING 256
#define MAX_ARRAY  32

/* The RPC library expects static space for return values */
int *destroy_1_svc(void *argp, struct svc_req *rqstp){
    static int  result;
    result = destroy();
    return &result;
}

int *set_value_1_svc(SetArgs *argp, struct svc_req *rqstp){
    static int result;
    result = set_value(argp->key, argp->value1, argp->N_value2,
                       argp->V_value2.V_value2_val, argp->value3);
    return &result;
}

GetReply *get_value_1_svc(KeyArg *argp, struct svc_req *rqstp){
    static GetReply reply;
    static char value1_buf[MAX_STRING];
    static double value2_buf[MAX_ARRAY];

    int N_value2 = 0;
    struct Coord coord;

    int status = get_value(argp->key, value1_buf, &N_value2, value2_buf, &coord);

    /* Prepare reply */
    reply.status = status;
    reply.value1 = value1_buf;
    reply.N_value2 = N_value2;
    reply.V_value2.V_value2_len = N_value2;
    reply.V_value2.V_value2_val = value2_buf;
    reply.value3 = coord;
    return &reply;
}

int *delete_key_1_svc(KeyArg *argp, struct svc_req *rqstp){
    static int result;
    result = delete_key(argp->key);
    return &result;
}

int *modify_value_1_svc(ModifyArgs *argp, struct svc_req *rqstp){
    static int result;
    result = modify_value(argp->key, argp->value1, argp->N_value2,
                          argp->V_value2.V_value2_val, argp->value3);
    return &result;
}

int *exist_1_svc(KeyArg *argp, struct svc_req *rqstp){
    static int result;
    result = exist(argp->key);
    return &result;
}