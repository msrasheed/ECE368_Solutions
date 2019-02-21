#include "delay.h"

Node * load_tree(FILE * infptr)
{
    double rd, r, c;
    fscanf(infptr, "%le %le %le\n", &rd, &r, &c);

    double len1, len2, cap;

    int firstChar = fgetc(infptr);
    fscanf(infptr, "(%le)\n", &cap);
    Node * leaf = create_leaf(firstChar - 48, cap);
    ListNode * head = create_ListNode(leaf);

    firstChar = fgetc(infptr);

    while (firstChar != -1)
    {
        if (firstChar == 40)
        {
            fscanf(infptr, "%le %le)\n", &len1, &len2);
            head = merge_nodes(head, len1, len2);
        }
        else
        {
            fscanf(infptr, "(%le)\n", &cap);
            leaf = create_leaf(firstChar - 48, cap);
            ListNode * ln = create_ListNode(leaf);
            head = insert_list(head, ln);
        }
        firstChar = fgetc(infptr);
    }

    leaf = head->node;
    free(head);
    return leaf;
}

Node * create_leaf(int id, double cap)
{
    Node * tn = malloc(sizeof(Node));
    tn->label = id;
    tn->sinkCap = cap;
    tn->left = NULL;
    tn->right = NULL;
    return tn;
}

void destroy_tree(Node * tree)
{
    if (tree == NULL) return;
    destroy_tree(tree->left);
    destroy_tree(tree->right);
    free(tree);
}

ListNode * create_ListNode(Node * tn)
{
    ListNode * ln = malloc(sizeof(ListNode));
    ln->node = tn;
    ln->next = NULL;
    return ln;
}

ListNode * insert_list(ListNode * head, ListNode * ins)
{
    ins->next = head;
    return ins;
}

ListNode * merge_nodes(ListNode * head, double leftLen, double rightLen)
{
    ListNode * right = head;
    ListNode * left = head->next;
    head = head->next->next;
    Node * tn = create_leaf(-1, -1);
    tn->left = left->node;
    tn->right = right->node;
    free(right);
    free(left);
    tn->left_len = leftLen;
    tn->right_len = rightLen;
    ListNode * ln = create_ListNode(tn);
    head = insert_list(head, ln);
    return head;
}

void preorder_traversal(FILE * out, Node * tree)
{
    if (tree->left == NULL)
    {
        fprintf(out, "%d(%le)\n", tree->label, tree->sinkCap);
        return;
    }

    fprintf(out, "(%le %le)\n", tree->left_len, tree->right_len);
    preorder_traversal(out, tree->left);
    preorder_traversal(out, tree->right);
}
