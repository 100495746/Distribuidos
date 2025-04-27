#include "claves_rpc.h"
#include "claves.h"
#include <rpc/rpc.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING 256
#define MAX_ARRAY  32

/* destroy_1_svc */
bool_t destroy_1_svc(void *argp, int *result, struct svc_req *rqstp) {
    (void)argp; (void)rqstp;  // Evitar warnings por no usar los parámetros
    *result = destroy();
    return TRUE;
}

/* set_value_1_svc */
bool_t set_value_1_svc(SetArgs *argp, int *result, struct svc_req *rqstp) {
    (void)rqstp;
    struct Coord value3;
    memcpy(&value3, argp->value3, sizeof(struct Coord));

    *result = set_value(argp->key, argp->value1, argp->N_value2,
                        argp->V_value2.V_value2_val, value3);
    return TRUE;
}

/* get_value_1_svc */
bool_t get_value_1_svc(KeyArg *argp, GetReply *reply, struct svc_req *rqstp) {
    (void)rqstp;
    static char value1_buf[MAX_STRING];
    static double value2_buf[MAX_ARRAY];
    int N_value2 = 0;
    struct Coord coord;

    int status = get_value(argp->key, value1_buf, &N_value2, value2_buf, &coord);

    reply->status = status;
    reply->value1 = value1_buf;
    reply->N_value2 = N_value2;
    reply->V_value2.V_value2_len = N_value2;
    reply->V_value2.V_value2_val = value2_buf;
    memcpy(reply->value3, &coord, sizeof(struct Coord));

    return TRUE;
}

/* delete_key_1_svc */
bool_t delete_key_1_svc(KeyArg *argp, int *result, struct svc_req *rqstp) {
    (void)rqstp;
    *result = delete_key(argp->key);
    return TRUE;
}

/* modify_value_1_svc */
bool_t modify_value_1_svc(ModifyArgs *argp, int *result, struct svc_req *rqstp) {
    (void)rqstp;
    struct Coord value3;
    memcpy(&value3, argp->value3, sizeof(struct Coord));

    *result = modify_value(argp->key, argp->value1, argp->N_value2,
                           argp->V_value2.V_value2_val, value3);
    return TRUE;
}

/* exist_1_svc */
bool_t exist_1_svc(KeyArg *argp, int *result, struct svc_req *rqstp) {
    (void)rqstp;
    *result = exist(argp->key);
    return TRUE;
}

bool_t claves_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result) {
    (void) transp; // Para evitar warnings
    (void) xdr_result;
    (void) result;
    return TRUE; // No hay nada que liberar manualmente
}
