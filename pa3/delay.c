#include "delay.h"

Node * load_tree(FILE * infptr, double * r, double * rd)
{
    double c;
    fscanf(infptr, "%le %le %le\n", rd, r, &c);

    double len1, len2, cap;
    int firstChar;
    fscanf(infptr, "%d(%le)\n", &firstChar, &cap);
    Node * leaf = create_leaf(firstChar, cap);
    ListNode * head = create_ListNode(leaf);

    firstChar = fgetc(infptr);

    while (firstChar != -1)
    {
        if (firstChar == 40)
        {
            fscanf(infptr, "%le %le)\n", &len1, &len2);
            head = merge_nodes(head, len1, len2, c);
        }
        else
        {
            int nextChar = fgetc(infptr);
            int id = firstChar - 48;
            while (nextChar != 40)
            {
                id = id * 10 + (nextChar - 48);
                nextChar = fgetc(infptr);
            }
            fscanf(infptr, "%le)\n", &cap);
            leaf = create_leaf(id, cap);
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
    tn->captot = cap;
    tn->capleft = cap;
    tn->capright = 0;
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

ListNode * merge_nodes(ListNode * head, double leftLen, double rightLen, double c)
{
    ListNode * right = head;
    ListNode * left = head->next;
    head = head->next->next;

    Node * tn = create_leaf(-1, 0);
    tn->left = left->node;
    tn->right = right->node;
    free(right);
    free(left);

    tn->left_len = leftLen;
    tn->right_len = rightLen;

    tn->left->captot += leftLen * c / 2;
    tn->right->captot += rightLen * c / 2;
    tn->captot = leftLen * c / 2 + rightLen * c / 2;

    tn->capleft = left->capleft + left->capright + left->captot;
    tn->capright = right->capleft + right->capright + right->captot;

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

typedef struct cap_node
{
    struct cap_node * next;
    double cap;
} Cap;

void push_cap(Cap ** head, double cap)
{
    Cap * h = *head;
    Cap * c = malloc(sizeof(Cap));
    c->cap = cap;
    c->next = h;
    *head = c;
}

double pop_cap (Cap ** head)
{
    Cap * p = *head;
    Cap * h = p->next;
    double cap = p->cap;
    free(p);
    *head = h;
    return cap;
}

int calc_delay_helper(int label, Node * tree, double r, double rollRes, double * sum, int * foundLeaf, double * divRes, Cap ** caps)
{
    if (tree == NULL)
    {
        *foundLeaf = 0;
        return 0;
    }

    if (tree->label == label)
    {
        *sum += tree->captot * rollRes * rollRes;
        *divRes = rollRes;
        *foundLeaf = 1;
        return 1;
    }

    int fll, flr;
    double sendResLeft = rollRes + r * tree->left_len;
    double sendResRight = rollRes + r * tree->right_len;
    int numLeft = calc_delay_helper(label, tree->left, r, sendResLeft, sum, &fll, divRes, caps);
    int numRight = calc_delay_helper(label, tree->right, r, sendResRight, sum, &flr, divRes, caps);

    if (fll || flr)
    {
        int num;
        if (fll) num = numRight;
        else num = numLeft;
        double rollRes2 = rollRes * rollRes;

        *sum += tree->captot * rollRes2;

        while (num > 0)
        {
            *sum += pop_cap(caps) * rollRes2;
            num--;
        }
        *foundLeaf = 1;
    }
    else
    {
        push_cap(caps, tree->captot);
        *foundLeaf = 0;
    }
    return numLeft + numRight + 1;
}

double calc_delay(int label, Node * tree, double rd, double r)
{
    Cap * capHead;
    double sum = 0;
    double divRes;
    int isLeaf;

    calc_delay_helper(label, tree, r, rd, &sum, &isLeaf, &divRes, &capHead);

    return sum / divRes;
}

void preorder_delays_helper(FILE * out, Node * tree, Node * head, double rd, double r)
{
    if (tree->left == NULL)
    {
        double delay = calc_delay(tree->label, head, rd, r);
        fwrite(&tree->label, sizeof(int), 1, out);
        fwrite(&delay, sizeof(double), 1, out);
        //fprintf(stdout, "%d %le\n", tree->label, delay);
        return;
    }

    preorder_delays_helper(out, tree->left, head, rd, r);
    preorder_delays_helper(out, tree->right, head, rd, r);
    return;
}

void preorder_delays(FILE * out, Node * tree, double rd, double r)
{
    preorder_delays_helper(out, tree, tree, rd, r);
    return;
}
