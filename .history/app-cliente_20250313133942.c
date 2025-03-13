#include "claves.h"
#include "proxy-mq.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *client_thread(void *arg) {
    int thread_id = *(int *)arg;
    int key = thread_id;
    char value1[256];
    snprintf(value1, sizeof(value1), "example_value_%d", thread_id);
    int N_value2 = 3;
    double V_value2[3] = {1.1, 2.2, 3.3};
    struct Coord value3 = {0.0, 0.0};

    // Set value
    printf("Thread %d: Setting value...\n", thread_id);
    int status = proxy_set_value(key, value1, N_value2, V_value2, value3);
    if (status == 0) {
        printf("Thread %d: Set value successful\n", thread_id);
    } else {
        printf("Thread %d: Set value failed\n", thread_id);
    }

    // Get value
    printf("Thread %d: Getting value...\n", thread_id);
    char get_value1[256];
    int get_N_value2;
    double get_V_value2[32];
    struct Coord get_value3;

    status = proxy_get_value(key, get_value1, &get_N_value2, get_V_value2, &get_value3);
    if (status == 0) {
        printf("Thread %d: Get value successful\n", thread_id);
        printf("Thread %d: Value1: %s\n", thread_id, get_value1);
        printf("Thread %d: N_value2: %d\n", thread_id, get_N_value2);
        for (int i = 0; i < get_N_value2; i++) {
            printf("Thread %d: V_value2[%d]: %f\n", thread_id, i, get_V_value2[i]);
        }
        printf("Thread %d: Coord: (%d, %d)\n", thread_id, get_value3.x, get_value3.y);
    } else {
        printf("Thread %d: Get value failed\n", thread_id);
    }

    // Modify value
    printf("Thread %d: Modifying value...\n", thread_id);
    char new_value1[256] = "new_example_value";
    double new_V_value2[3] = {4.4, 5.5, 6.6};
    struct Coord new_value3 = {1.0, 1.0};

    status = proxy_modify_value(key, new_value1, N_value2, new_V_value2, new_value3);
    if (status == 0) {
        printf("Thread %d: Modify value successful\n", thread_id);
    } else {
        printf("Thread %d: Modify value failed\n", thread_id);
    }

    return NULL;
}

int main() {
    const int num_threads = 5; // Number of concurrent clients
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, client_thread, &thread_ids[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}