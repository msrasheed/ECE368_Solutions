#include <stdio.h>

typedef struct tree_node
{
    struct tree_node * left;
    struct tree_node * right;
    int key;
    int bal;
} Node;

Node * buildTree(FILE * in, int * form);
Node * create_node(int key);
Node * insertNode(Node * head, int key, int * form);
Node * deleteNode(Node * node, int key);
Node * getImmPredecessor(Node * node, int * nkey);

Node * rotate(Node * node, int key);
Node * leftRotation(Node * node);
Node * rightRotation(Node * node);

void preOrderTraversal(Node * node, FILE * out);

void destroyTree(Node * node);
void evaluateTree(FILE * inptr, int * form, int * bst, int * baltree);
