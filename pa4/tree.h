#include <stdio.h>

typedef struct tree_node
{
    struct tree_node * left;
    struct tree_node * right;
    int key;
    int bal;
} Node;

Node * parseInput(FILE * in);
Node * create_node(int key);
Node * insertNode(Node * head, int key);
Node * deleteNode(Node * node, int key);
Node * getImmPredecessor(Node * node, int * nkey);

Node * rotate(Node * node, int key);
Node * leftRotation(Node * node);
Node * rightRotation(Node * node);

void preOrderTraversal(Node * node, FILE * out);
