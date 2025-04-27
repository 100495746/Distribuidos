#include "claves.h"
#include "claves_rpc.h"
#include <rpc/clnt.h>
#include <rpc/xdr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 256
#define MAX_ARRAY  32
#define RPC_ERROR -2  /* Error a nivel de librería RPC */

static CLIENT *clnt = NULL;

static int init_client(void) {
    if (clnt) return 0;
    char *ip = getenv("IP_TUPLAS");
    if (!ip) {
        fprintf(stderr, "IP_TUPLAS no definida\n");
        return RPC_ERROR;
    }
    clnt = clnt_create(ip, CLAVES_PROG, CLAVES_VERS, "tcp");
    if (!clnt) {
        clnt_pcreateerror(ip);
        return RPC_ERROR;
    }
    return 0;
}

int proxy_set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (init_client() != 0) return RPC_ERROR;
    SetArgs args = {0};
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    args.V_value2.V_value2_len = N_value2;
    args.V_value2.V_value2_val = V_value2;
    memcpy(args.value3, &value3, sizeof(struct Coord));


    int result = -1;
    enum clnt_stat st = set_value_1(&args, &result, clnt);
    if (st != RPC_SUCCESS) return RPC_ERROR;
    return result;
}

int proxy_get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    if (init_client() != 0) return RPC_ERROR;
    KeyArg arg = {.key = key};
    GetReply reply;
    memset(&reply, 0, sizeof(reply));

    enum clnt_stat st = get_value_1(&arg, &reply, clnt);
    if (st != RPC_SUCCESS) return RPC_ERROR;
    if (reply.status != 0) return reply.status;

    strncpy(value1, reply.value1, MAX_STRING);
    *N_value2 = reply.N_value2;
    for (int i = 0; i < reply.N_value2 && i < MAX_ARRAY; i++) {
        V_value2[i] = reply.V_value2.V_value2_val[i];
    }
    memcpy(value3, reply.value3, sizeof(struct Coord));
    xdr_free((xdrproc_t)xdr_GetReply, (char *)&reply);
    return 0;
}

int proxy_modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (init_client() != 0) return RPC_ERROR;
    ModifyArgs args = {0};
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    args.V_value2.V_value2_len = N_value2;
    args.V_value2.V_value2_val = V_value2;
    memcpy(args.value3, &value3, sizeof(struct Coord));


    int result = -1;
    enum clnt_stat st = modify_value_1(&args, &result, clnt);
    if (st != RPC_SUCCESS) return RPC_ERROR;
    return result;
}

int destroy(void) {
    if (init_client() != 0) return RPC_ERROR;
    int result = -1;
    enum clnt_stat st = destroy_1(NULL, &result, clnt);
    if (st != RPC_SUCCESS) return RPC_ERROR;
    return result;
}

int delete_key(int key) {
    if (init_client() != 0) return RPC_ERROR;
    KeyArg arg = {.key = key};
    int result = -1;
    enum clnt_stat st = delete_key_1(&arg, &result, clnt);
    if (st != RPC_SUCCESS) return RPC_ERROR;
    return result;
}

int exist(int key) {
    if (init_client() != 0) return RPC_ERROR;
    KeyArg arg = {.key = key};
    int result = -1;
    enum clnt_stat st = exist_1(&arg, &result, clnt);
    if (st != RPC_SUCCESS) return RPC_ERROR;
    return result;
}

