#include "proxy-sock.h"

Respuesta send_rcv(Peticion *peticion) {
    char *ip_tuplas, *puerto_tuplas;
    Respuesta respuesta;
    if ((ip_tuplas = getenv("IP_TUPLAS")) == NULL) {
        perror("Error en getenv");
        respuesta.status = -2;
        return respuesta;
    }
    if ((puerto_tuplas = getenv("PUERTO_TUPLAS")) == NULL) {
        perror("Error en getenv");
        respuesta.status = -2;
        return respuesta;
    }  
    int cliente_sock;
    struct sockaddr_in servidor;
    struct hostent *host;

    cliente_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cliente_sock == -1) {
        perror("Error en socket cliente");
        respuesta.status = -2;
        close(cliente_sock);
        return respuesta;
    }
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(atoi(puerto_tuplas));

    if ((host = gethostbyname(ip_tuplas)) == NULL) {
        perror("Error en gethostbyname");
        respuesta.status = -2;
        return respuesta;
    }
    memcpy(&servidor.sin_addr, host->h_addr_list[0], host->h_length);

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