#include <stdio.h>
#include <stdlib.h>

typedef struct tree_node
{
    struct tree_node * left;
    struct tree_node * right;
    double left_len;
    double right_len;
    double sinkCap;
    int label;
} Node;

typedef struct list_node
{
    struct list_node * next;
    Node * node;
} ListNode;

Node * load_tree(FILE * infptr);
Node * create_leaf(int id, double cap);
void destroy_tree(Node * tree);

ListNode * create_ListNode(Node * tn);
ListNode * insert_list(ListNode * head, ListNode * ins);
ListNode * merge_nodes(ListNode * head, double leftLen, double rightLen);

void preorder_traversal(FILE * out, Node * tree);
