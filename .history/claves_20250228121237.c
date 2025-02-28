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
    int N_value; // size of value1
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

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Node *current = head;

    //comprobar que no esta la Key en la lista
    while(current != NULL){
        if(current->key == key){
            return -1;
        }

        current = current->next;
    }

    //la longitud de la cadena no debe ser menor a uno ni mayor a 32
    if(N_value2>32 || N_value2<1){
        return -1;
    }
    if (strlen(value1)>255){
        return -1;
    }

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
    strcpy(inserted->val1, value1);


    inserted->val2 = (double *)malloc(N_value2*sizeof(double));
    if (inserted->val2== NULL){
        free(inserted->val1);
        free(inserted);
        return -1;
    }
    memcpy(inserted->val2, V_value2, N_value2*sizeof(double));


    inserted->key = key;
    inserted->coord = value3;
    inserted->next = head; //el nuevo nodo será el primero
    inserted-> N_value = N_value2;

    // el nuevo nodo es el primero (el nuevo head)
    head = inserted;
    return 0;
    
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    Node *current = head;
    while(current->key!=key){
        current=current->next;
        if (current == NULL){
            return -1;
        }
    
    }
    *N_value2 = current->N_value;
    strcpy(value1, current->val1);
    memcpy(V_value2, current->val2, (*N_value2)*sizeof(double));
    *value3 = current->coord;
   

    return 0;

}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    Node *current = head;
    while(current!=NULL ->key!=key){
        if(current->next == NULL){
            return -1;
        }
        current=current->next;
    }
    if (N_value2<1 || N_value2 >31){
        return -1;
    }
    free(current->val1);
    free(current->val2);
    current->val2 = (char *)malloc(N_value2*sizeof(double));
    memcpy(V_value2, current->val2, )

}