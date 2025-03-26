#include "proxy-sock.h"

Respuesta send_rcv(Peticion *peticion) {
    int cliente_sock;
    struct sockaddr_in servidor;
    Respuesta respuesta;

    cliente_sock = socket(AF_INET, SOCK_STREAM, 0);
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PUERTO);
    inet_pton(AF_INET, , &servidor.sin_addr);

    if (connect(cliente_sock, (struct sockaddr *)&servidor, sizeof(servidor)) == -1) {
        perror("Error en connect");
        respuesta.status = -2;
        close(cliente_sock);
        return respuesta;
    }

    send(cliente_sock, peticion, sizeof(Peticion), 0);
    recv(cliente_sock, &respuesta, sizeof(Respuesta), 0);
    close(cliente_sock);

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
    Respuesta respuesta = send_rcv(&peticion);
    return respuesta.status;
}