#ifndef SERVIDOR_MQ_H
#define SERVIDOR_MQ_H

#include "claves.h"
#include "proxy-mq.h"
#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function declarations
void *tratar_peticion(void *arg);

#endif // SERVIDOR_MQ_H