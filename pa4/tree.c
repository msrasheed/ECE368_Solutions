#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

void preOrderTraversal2(Node * node)
{
    if (node == NULL) return;

    char val = 0;
    if(node->left != NULL) val |= 0x2;
    if(node->right != NULL) val |= 0x1;

    fprintf(stdout, "%d %d %d\n", node->key, val, node->bal);

    preOrderTraversal2(node->left);
    preOrderTraversal2(node->right);
}

//build tree given ops file
Node * buildTree(FILE * in, int * form)
{
    Node * head = NULL;
    int i = 0;

    while(!feof(in))
    {
        int key;
        char instr;
        int num1 = fread(&key, sizeof(int), 1, in);
        int num2 = fread(&instr, sizeof(char), 1, in);

        if (num1 != num2)
        {
            *form = 0;
            return head;
        }

        if (!feof(in))
        {
            if (instr == 'i')
            {
                head = insertNode(head, key, form);
                if (*form == 0)
                    return head;
            }
            else if (instr == 'd')
            {
                head = deleteNode(head, key);
            }
            else
            {
                *form = 0;
                return head;
            }
        }
        i++;
        //fprintf(stdout, "%d\n", i);
        //preOrderTraversal2(head);
        //fprintf(stdout, "\n");
    }

    return head;
}

//prints out to write file
void preOrderTraversal(Node * node, FILE * out)
{
    if (node == NULL) return;

    fwrite(&(node->key), sizeof(int), 1, out);

    char val = 0;
    if(node->left != NULL) val |= 0x2;
    if(node->right != NULL) val |= 0x1;

    fwrite(&val, sizeof(char), 1, out);
    //fprintf(stdout, "%d %d\n", node->key, val);

    preOrderTraversal(node->left, out);
    preOrderTraversal(node->right, out);
}

// creates and instantiates tree node
Node * create_node(int key)
{
    Node * tn = malloc(sizeof(Node));
    if (tn == NULL)
    {
        return NULL;
    }
    tn->key = key;
    tn->left = NULL;
    tn->right = NULL;
    return tn;
}

// insert node into tree and maintain height balance
Node * insertNode(Node * head, int key, int * form)
{
    Node * ya = head;
    Node * curr = head;
    Node * pya = NULL;
    Node * pcurr = NULL;
    Node * q;

    //find where to insert node
    while (curr != NULL)
    {
        if (key <= curr->key)
            q = curr->left;
        else
            q = curr->right;

        if (q != NULL && q->bal != 0)
        {
            pya = curr;
            ya = q;
        }

        pcurr = curr;
        curr = q;
    }

    //craete node
    q = create_node(key);
    if (q == NULL)
    {
        *form = 0;
        return head;
    }
    q->bal = 0;

    //insert node
    if (pcurr == NULL)
    {
        head = q;
        return head;
    }
    else
    {
        if (key <= pcurr->key)
            pcurr->left = q;
        else
            pcurr->right = q;
    }

    // update the balance
    curr = ya;
    while (curr != q)
    {
        if (key <= curr->key)
        {
            curr->bal += 1;
            curr = curr->left;
        }
        else
        {
            curr->bal -= 1;
            curr = curr->right;
        }
    }

    // check if balancing is required
    if ((ya->bal < 2) && (ya->bal > -2))
        return head;

    //rotate youngest ancestor
    Node * nya;
    nya = rotate(ya, key);

    //add rotated subtree back into tree
    if (pya == NULL)
    {
        head = nya;
    }
    else if (pya->left == ya)
    {
        pya->left = nya;
    }
    else {
        pya->right = nya;
    }

    return head;
}

Node * deleteNodeHelper(Node * node, int key)
{
    //found key and must delete node
    if (node->key == key)
    {
        //if leaf
        if (node->left == NULL && node->right == NULL)
        {
            free(node);
            return NULL;
        }
        //if only one child
        else if (node->left == NULL)
        {
            Node * n = node->right;
            free(node);
            return n;
        }
        //has left and right child
        else
        {
            int nkey;
            int pbal = node->left->bal;
            node->left = getImmPredecessor(node->left, &nkey);
            node->key = nkey;
            if (node->left == NULL)
            {
                node->bal -= 1;
            }
            else
            {
                if (node->left->bal == 2 || node->left->bal == -2)
                {
                    node->left = rotate(node->left, key);
                }
                if (pbal != node->left->bal && node->left->bal == 0)
                {
                    node->bal -= 1;
                }
            }
            return node;
        }
    }

    //move down tree - if balance of child changes to zero, edit balance
    if (key < node->key)
    {
        if (node->left == NULL)
            return node;

        int pbal = node->left->bal;
        node->left = deleteNodeHelper(node->left, key);
        if (node->left == NULL)
        {
            node->bal -= 1;
        }
        else
        {
            if (node->left->bal == 2 || node->left->bal == -2)
            {
                node->left = rotate(node->left, key);
            }
            if (pbal != node->left->bal && node->left->bal == 0)
            {
                node->bal -= 1;
            }
        }
        return node;
    }
    else
    {
        if (node->right == NULL)
            return node;

        int pbal = node->right->bal;
        node->right = deleteNodeHelper(node->right, key);
        if (node->right == NULL)
        {
            node->bal += 1;
        }
        else
        {
            if (node->right->bal == 2 || node->right->bal == -2)
            {
                node->right = rotate(node->right, key);
            }
            if (pbal != node->right->bal && node->right->bal == 0)
            {
                node->bal += 1;
            }
        }
        return node;
    }
}

//do one last rotate on head node if necessary
Node * deleteNode(Node * head, int key)
{
    head = deleteNodeHelper(head, key);
    if (head->bal == 2 || head->bal == -2)
    {
        head = rotate(head, key);
    }
    return head;
}

//find immediate predecessor to replace deletion and preform rotations as necessary
Node * getImmPredecessor(Node * node, int * nkey)
{
    if (node->right == NULL && node->left == NULL)
    {
        *nkey = node->key;
        free(node);
        return NULL;
    }
    else if (node->right == NULL)
    {
        *nkey = node->key;
        Node * n = node->left;
        free(node);
        return n;
    }
    else
    {
        int pbal = node->right->bal;
        node->right = getImmPredecessor(node->right, nkey);
        if (node->right == NULL)
        {
            node->bal += 1;
        }
        else
        {
            if (node->right->bal == 2 || node->right->bal == -2)
            {
                node->right = rotate(node->right, *nkey);
            }
            if (pbal != node->right->bal && node->right->bal == 0)
            {
                node->bal += 1;
            }
        }
        return node;
    }
}

Node * rotate(Node * node, int key)
{
    Node * curr;
    // find the child into which q is inserted
    Node * child;
    if (key < node->key)
        child = node->left;
    else
        child = node->right;

    // the subtree rooted at node needs balancing
    // curr points to the new root of the subtree
    // pnode has to point to curr after rebalancing
    // both node and child are unbalanced in the same
    // direction
    Node * newNode;
    if ((node->bal == 2) && (node->left->bal == 1))
    {
        child = node->left;
        curr = child;
        newNode = rightRotation(node);
        node->bal = 0;
        child->bal = 0;
    }

    if ((node->bal == -2) && (node->right->bal == -1))
    {
        child = node->right;
        curr = child;
        newNode = leftRotation(node);
        node->bal = 0;
        child->bal = 0;
    }

    // node and child are unbalanced in opp. Directions
    //Node * newChild;
    if ((node->bal == 2) && (node->left->bal == -1))
    {
        child = node->left;
        curr = child->right;
        node->left = leftRotation(child);
        //node->left = curr;
        newNode = rightRotation(node);
        if (curr->bal == 0) // child is inserted node q
        {
            node->bal = 0;
            child->bal = 0;
        }
        else
        {
            if (curr->bal == 1) // ori. left subtree of curr
            {
                node->bal = -1; // contains q
                child->bal = 0;
            }
            else // ori. right subtree of curr
            {
                node->bal = 0; // contains q
                child->bal = 1;
            }
            curr->bal = 0; // new root is balanced
        }
    }

    if ((node->bal == -2) && (node->right->bal == 1))
    {
        child = node->right;
        curr = child->left;
        node->right = rightRotation(child);
        //node->right = curr;
        newNode = leftRotation(node);
        if (curr->bal == 0) // curr is inserted node q
        {
            node->bal = 0;
            child->bal = 0;
        }
        else
        {
            if (curr->bal == -1) // ori. right subtree of curr
            {
                node->bal = 1; // contains q
                child->bal = 0;
            }
            else // ori. left subtree of curr
            {
                node->bal = 0; // contains q
                child->bal = -1;
            }
            curr->bal = 0; // new root is balanced
        }
    }

    //if node to child to check has balance of zero
    if (node->bal == 2 && node->left->bal == 0)
    {
        child = node->left;
        curr = child;
        newNode = rightRotation(node);
        node->bal = 1;
        child->bal = -1;
    }

    if (node->bal == -2 && node->right->bal == 0)
    {
        child = node->right;
        curr = child;
        newNode = leftRotation(node);
        node->bal = -1;
        child->bal = 1;
    }

    return newNode;
}

// rotate node to left
Node * leftRotation(Node * node)
{
    Node * newNode = node->right;
    Node * newNodeLeft = node->right->left;

    newNode->left = node;
    node->right = newNodeLeft;

    return newNode;
}

// rotate node to right
Node * rightRotation(Node * node)
{
    Node * newNode = node->left;
    Node * newNodeRight = node->left->right;

    newNode->right = node;
    node->left = newNodeRight;

    return newNode;
}

void destroyTree(Node * node)
{
    if (node == NULL)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    free(node);
}

int evaluateTreeHelper(Node * node, int * bst, int * baltree)
{
    if (node == NULL)
        return -1;

    if (node->right != NULL && node->right->key < node->key)
        *bst = 0;
    if (node->left != NULL && node->left->key > node->key)
        *bst = 0;

    int lh = evaluateTreeHelper(node->left, bst, baltree);
    int rh = evaluateTreeHelper(node->right, bst, baltree);

    node->bal = lh - rh;
    if (node->bal > 2 || node->bal < -2)
        *baltree = 0;

    if (lh > rh)
        return lh + 1;
    else
        return rh + 1;
}

Node * evalBuildTree(FILE * inptr, int * form)
{
    if (*form == 0)
        return NULL;

    if (!feof(inptr))
    {
        int key;
        char val;
        int num1 = fread(&key, sizeof(int), 1, inptr);
        int num2 = fread(&val, sizeof(char), 1, inptr);

        if (feof(inptr)) return NULL;
        if (num1 != num2)
        {
            *form = 0;
            return NULL;
        }
        if (val < 0 || val > 3)
        {
            *form = 0;
            return NULL;
        }

        Node * tn = create_node(key);

        if ((val & 0x2) == 2)
            tn->left = evalBuildTree(inptr, form);
        if ((val & 0x1) == 1)
            tn->right = evalBuildTree(inptr, form);

        return tn;
    }

    return NULL;
}

void evaluateTree(FILE * inptr, int * form, int * bst, int * baltree)
{
    *form = 1;
    *bst = 1;
    *baltree = 1;

    Node * tree = evalBuildTree(inptr, form);
    if (*form == 0)
        return;

    evaluateTreeHelper(tree, bst, baltree);
    //preOrderTraversal2(tree);
    destroyTree(tree);
}
