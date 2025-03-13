#include "claves.h"
#include "proxy-mq.h"
#include <stdio.h>

int main() {
    int key = 1;
    char value1[256] = "example_value";
    int N_value2 = 3;
    double V_value2[3] = {1.1, 2.2, 3.3};
    struct Coord value3 = {0.0, 0.0};

    // Set value
    int status = proxy_set_value(key, value1, N_value2, V_value2, value3);
    if (status == 0) {
        printf("Set value successful\n");
    } else {
        printf("Set value failed\n");
    }

    // Get value
    char get_value1[256];
    int get_N_value2;
    double get_V_value2[32];
    struct Coord get_value3;

    status = proxy_get_value(key, get_value1, &get_N_value2, get_V_value2, &get_value3);
    if (status == 0) {
        printf("Get value successful\n");
        printf("Value1: %s\n", get_value1);
        printf("N_value2: %d\n", get_N_value2);
        for (int i = 0; i < get_N_value2; i++) {
            printf("V_value2[%d]: %f\n", i, get_V_value2[i]);
        }
        printf("Coord: (%d, %f)\n", get_value3.x, get_value3.y);
    } else {
        printf("Get value failed\n");
    }

    // Modify value
    char new_value1[256] = "new_example_value";
    double new_V_value2[3] = {4.4, 5.5, 6.6};
    struct Coord new_value3 = {1.0, 1.0};

    status = proxy_modify_value(key, new_value1, N_value2, new_V_value2, new_value3);
    if (status == 0) {
        printf("Modify value successful\n");
    } else {
        printf("Modify value failed\n");
    }

    return 0;
}