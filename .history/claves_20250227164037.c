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

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
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
        
    }

}