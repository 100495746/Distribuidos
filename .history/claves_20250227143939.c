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
    Node *temp = NULL;
    if(head == NULL){
        return 0;
    }
    else{
        while(head->next !=NULL){
            temp = head->next->next;
            free(head->next->coord);
            free(head->next->val1);
            free(head->next->val2);
            free(head->next->next);
            free(head->next->key);
            head->next = temp;
        }

    };

}