#include "claves.h"
#include <stddef.h>


// las colas de mensajes posix no reconocen punteros
typedef struct {
    int tipo_operacion;  // 1 = SET, 2 = GET, 3 = DELETE, etc.
    int key;
    char value1[256];    // Arreglo en lugar de puntero
    int N_value2;
    double V_value2[32]; // Arreglo en lugar de puntero
    struct Coord value3;
} Mensaje;
