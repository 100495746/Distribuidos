#include "claves.h"
#include <stddef.h>

struct Coord{
    int x;
    int y;
};

typedef struct Node{
    int key;
    char *val1;
    double *val2;
    struct Coord coord;
    struct Node *next;
    

} Node;

// head es el primer nodo
static Node *head = NULL;

int destroy(){
    Node *current = head;
    if(head == NULL){
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

    return 0;
}

int proxy_set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Node *current = head;
    while(current != NULL){
        if(current->key == key){
            return -1;
        }
        if(N_value2>32 || N_value2<1){
            return -1;
        }
        if (sizeof(*value1)/sizeof(char)>255){
            return -1;
        }

        current = current->next;
    }
    free(current);

    Node *inserted = (Node *)malloc(sizeof(Node));
    if (inserted==NULL){
        free(inserted);
        return(-1);
    }
    
    inserted->val1 = (char *)malloc(strlen(value1));

    if (inserted->val1== NULL){
        free(inserted);
        return -1;
    }

    inserted->val2 = (double *)malloc(N_value2*sizeof(double));

    if (inserted->val2== NULL){
        free(inserted);
        return -1;
    }
    inserted->key = key;
    inserted->coord = value3;
    inserted->next = head;

    strcpy(inserted->val1,value1);
    memcpy(inserted->val2, V_value2, N_value2*sizeof(double));
    



}