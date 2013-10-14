#ifndef PP_LINKED_LIST
#define PP_LINKED_LIST


struct Node {
    struct Node* next;
    int value;
};

struct List {
    struct Node *head, *tail;
};

struct List* empty_list();
void free_list(struct List* l);
void push_back(struct List* l, int v);
int pop_front(struct List* l);
int is_empty(struct List* l);

#endif
