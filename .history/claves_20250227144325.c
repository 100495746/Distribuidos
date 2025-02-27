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
        while(head->next !=NULL){
            Node *temp = current->next;
            free(current->val1);
            free(current->val2);
            free(current->next);
            head->next = temp;
        }
        head = NULL;
    };
    return 0;

}