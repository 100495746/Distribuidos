#include "claves.h"
#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>




typedef struct Node{
    int key;
    char *val1;
    double *val2;
    int N_value; // size of value1
    struct Coord coord;
    struct Node *next;
} Node;

// head es el primer nodo
static Node *head = NULL;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int destroy(){
    pthread_mutex_lock(&mutex);
    Node *current = head;
    if(head == NULL){
        pthread_mutex_unlock(&mutex);

        return 0;
    }
    else{
        while(current !=NULL){
            Node *temp = current->next;
            free(current->val1);
            free(current->val2);
            free(current);
            current = temp;
        }
        head = NULL;
    };
    pthread_mutex_unlock(&mutex);

    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    pthread_mutex_lock(&mutex);

    Node *current = head;

    //comprobar que no esta la Key en la lista
    while(current != NULL){
        if(current->key == key){
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        current = current->next;
    }

    //la longitud de la cadena no debe ser menor a uno ni mayor a 32
    if(N_value2>32 || N_value2<1){
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    if (strlen(value1)>255){
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    Node *inserted = (Node *)malloc(sizeof(Node));
    if (inserted==NULL){
        free(inserted);
        pthread_mutex_unlock(&mutex);
        return(-1);
    }
    
    inserted->val1 = (char *)malloc(strlen(value1));
    if (inserted->val1== NULL){
        free(inserted);
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    strcpy(inserted->val1, value1);


    inserted->val2 = (double *)malloc(N_value2*sizeof(double));
    if (inserted->val2== NULL){
        free(inserted->val1);
        free(inserted);
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    memcpy(inserted->val2, V_value2, N_value2*sizeof(double));


    inserted->key = key;
    inserted->coord = value3;
    inserted->next = head; //el nuevo nodo será el primero
    inserted-> N_value = N_value2;

    // el nuevo nodo es el primero (el nuevo head)
    head = inserted;
    pthread_mutex_unlock(&mutex);

    return 0;
    
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    pthread_mutex_lock(&mutex);
    Node *current = head;
    while(current->key!=key){
        current=current->next;
        if (current == NULL){
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
    *N_value2 = current->N_value;
    strcpy(value1, current->val1);
    memcpy(V_value2, current->val2, (*N_value2)*sizeof(double));
    *value3 = current->coord;
   
    pthread_mutex_unlock(&mutex);
    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    pthread_mutex_lock(&mutex);
    Node *current = head;
    while(current!=NULL && current->key!=key){current=current->next;}
    if (current==NULL){
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    if (N_value2 < 1 || N_value2 > 32){
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    free(current->val1);
    current->val1 = (char *)malloc(strlen(value1)+1);
    if (current->val2 == NULL){
        free(current->val1);
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    free(current->val2);
    current->val2 = (char *)malloc(N_value2*sizeof(double));
    if (current->val2 == NULL){
        free(current->val1);
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    memcpy(V_value2, current->val2, N_value2 * sizeof(double));
    current->N_value = N_value2;
    current->coord = value3;
    pthread_mutex_unlock(&mutex);

    return 0;

}
/* int delete_key(int key). Este servicio permite borrar el elemento cuya 
clave es key. La función devuelve 0 en caso de éxito y -1 en caso de error.
En caso de que la clave no exista también se devuelve -1*/
int delete_key(int key){
    pthread_mutex_lock(&mutex);
    Node *current = head;
    Node *prev = NULL;

    if (current!=NULL && current->key == key){
        head = current->next;
        free(current->val1);
        free(current->val2);
        free(current);
    }
    while(current!=NULL && current->key!=key){
        prev = current;
        current=current->next;
        }
    if (current ==NULL){
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    prev->next = current->next;
    free(current->val1);
    free(current->val2);
    free(current);
    pthread_mutex_unlock(&mutex);

    
    return 0;   
}