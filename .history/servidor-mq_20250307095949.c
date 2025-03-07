#include "claves.h"
#include <stddef.h>



struct Mensaje{
    int operacion; //SET=1; GET=2; 3=DELETE.
    int key;
    char val1;
    double *val2;
    int N_value; // size of value1
    struct Coord coord;
    



};