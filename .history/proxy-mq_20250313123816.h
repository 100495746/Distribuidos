#define proxy_mq_H
#include "claves.h"
#include <stddef.h>
#include <mqueue.h>  // Biblioteca POSIX de colas de mensajes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // Para O_CREAT, O_WRONLY, etc.
#include <sys/stat.h>   // Para permisos
#define PETICIONES "/servidor"



