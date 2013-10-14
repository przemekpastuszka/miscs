#include <stdlib.h>
#include "list.h"

struct List* empty_list() {
    struct List* new = malloc(sizeof(struct List));
    new -> head = new -> tail = NULL;
    return new;
}

void free_node(struct Node* n) {
    if(n != NULL) {
        free(n);
    }
}

void free_list(struct List* l) {
    struct Node* prev = NULL;
    struct Node* curr = l -> head;
    while(curr) {
        prev = curr;
        curr = curr -> next;
        free_node(prev);
    }

    free_node(prev);
    free(l);
}

void push_back(struct List* l, int v) {
    struct Node* element = malloc(sizeof(struct Node));
    element -> next = NULL;
    element -> value = v;
    if(l -> head == NULL) {
        l -> head = l -> tail = element;
    }
    else {
        l -> tail -> next = element;
        l -> tail = element;
    }
}

int pop_front(struct List* l) {
    int value = l -> head -> value;
    struct Node* tmp = l -> head -> next;
    struct Node* old_head = l -> head;
    l -> head = tmp;
    if(tmp == NULL) {
        l -> tail = NULL;
    }
    free(old_head);
    return value;
}
int is_empty(struct List* l) {
    return l -> head == NULL;
}
