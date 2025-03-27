#include "servidor-sock.h"

void *tratar_peticion(void *arg) {
    int cliente_sock = *(int *)arg;
    free(arg);
    Peticion peticion;
    Respuesta respuesta;
    if (recv(cliente_sock, &peticion, sizeof(Peticion), 0) == -1) {
        perror("Error en recv");
        close(cliente_sock);
        exit(-2);
    }
    printf("Handling request: tipo_operacion=%d, key=%d\n", peticion.tipo_operacion, peticion.key);

    respuesta.status = 0; // Assume success
    // Handle the request based on tipo_operacion
    switch (peticion.tipo_operacion) {
        case 1: // SET
            printf("Handling SET operation\n");
            respuesta.status = set_value(peticion.key, peticion.value1, peticion.N_value2, peticion.V_value2, peticion.value3);
            break;
        case 2: // GET
            printf("Handling GET operation\n");
            respuesta.status = get_value(peticion.key, respuesta.value1, &respuesta.N_value2, respuesta.V_value2, &respuesta.value3);
            break;
        case 3: // MODIFY
            printf("Handling MODIFY operation\n");
            respuesta.status = modify_value(peticion.key, peticion.value1, peticion.N_value2, peticion.V_value2, peticion.value3);
            break;
        default:
            printf("Unknown operation\n");
            respuesta.status = -1; // Unknown operation
            break;
    }

    // Send the response back to the client
    if (send(cliente_sock, &respuesta, sizeof(Respuesta), 0) == -1) {
        perror("Error en send");
        close(cliente_sock);
        exit(-2);
    }
    close(cliente_sock);
    return NULL;    

}

int main(int argc, char *argv[]) {
    printf("Starting server...\n");
    int server_sock;
    struct sockaddr_in servidor_config, cliente;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Error en socket");
        exit(-2);
    }
    servidor_config.sin_family = AF_INET;
    servidor_config.sin_port = htons(atoi(argv[1]));
    servidor_config.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&servidor_config, sizeof(servidor_config)) == -1) {
        perror("Error en bind");
        exit(-2);
    }
    if (listen(server_sock, SOMAXCONN) == -1) {
        perror("Error en listen");
        exit(-2);
    }

    printf("Server started, waiting for requests...\n");
    while (1) {
        struct sockaddr_in cliente_config;
        socklen_t cliente_tam = sizeof(cliente_config);
        int *cliente_sock = malloc(sizeof(int));
        *cliente_sock = accept(server_sock, (struct sockaddr *)&cliente_config, &cliente_tam);
        if (*cliente_sock == -1) {
            perror("Error en accept");
            free(cliente_sock);
            exit(-2);
        }
        printf("Conectado con %s:%d\n", inet_ntoa(cliente_config.sin_addr), ntohs(cliente_config.sin_port));
        // tratamos las peticion concurrentemente 
        pthread_t esclavo;
        pthread_create(&esclavo, NULL, tratar_peticion, cliente_sock);
        pthread_detach(esclavo);         
    }

    mq_close(server_sock);
    return 0;
}