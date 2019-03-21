#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

Node * parseInput(FILE * in)
{
    Node * head = NULL;

    while(!feof(in))
    {
        int key;
        char instr;
        int numRead = fread(&key, sizeof(int), 1, in);
        numRead = fread(&instr, sizeof(char), 1, in);
        numRead++;

        if (instr == 'i')
        {
            head = insertNode(head, key);
        }
        else if (instr == 'd')
        {
            head = deleteNode(head, key);
        }
        else
        {

        }
    }

    return head;
}

void preOrderTraversal(Node * node, FILE * out)
{
    if (node == NULL) return;
    preOrderTraversal(node->left, out);
    preOrderTraversal(node->right, out);

    fwrite(&(node->key), sizeof(int), 1, out);

    char val = 0;
    if(node->left != NULL) val |= 0x2;
    if(node->right != NULL) val |= 0x1;

    fwrite(&val, sizeof(char), 1, out);
}

Node * create_node(int key)
{
    Node * tn = malloc(sizeof(Node));
    if (tn == NULL)
    {
        return NULL;
    }
    tn->key = key;
    return tn;
}

Node * insertNode(Node * head, int key)
{
    Node * ya = head;
    Node * curr = head;
    Node * pya = NULL;
    Node * pcurr = NULL;
    Node * q;

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

    q = create_node(key);
    q->bal = 0;

    if (pcurr == NULL)
        head = q;
    else
    {
        if (key < pcurr->key)
            pcurr->left = q;
        else
            pcurr->right = q;
    }

    // update the balance
    curr = ya;
    while (curr != q)
    {
        if (key < curr->key)
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

    Node * nya;
    nya = rotate(ya, key);

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
    //Node * head = node;

    if (node->key == key)
    {
        if (node->left == NULL && node->right == NULL)
        {
            free(node);
            return NULL;
        }
        else if (node->left == NULL)
        {
            Node * n = node->right;
            free(node);
            return n;
        }
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

    if (key < node->key)
    {
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

Node * deleteNode(Node * head, int key)
{
    head = deleteNodeHelper(head, key);
    if (head->bal == 2 || head->bal == -2)
    {
        head = rotate(head, key);
    }
    return head;
}

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
    if ((node->bal == 2) && (node->right->bal == 1))
    {
        child = node->right;
        curr = child;
        newNode = rightRotation(node);
        node->bal = 0;
        child->bal = 0;
    }

    if ((node->bal == -2) && (node->left->bal == -1))
    {
        child = node->left;
        curr = child;
        newNode = leftRotation(node);
        node->bal = 0;
        child->bal = 0;
    }

    // node and child are unbalanced in opp. Directions
    //Node * newChild;
    if ((node->bal == 2) && (node->right->bal == -1))
    {
        child = node->right;
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
            if (child->bal == 1) // ori. left subtree of curr
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

    if ((node->bal == -2) && (node->left->bal == 1))
    {
        child = node->left;
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

    return newNode;
}

Node * leftRotation(Node * node)
{
    Node * newNode = node->right;
    Node * newNodeLeft = node->right->left;

    newNode->left = node;
    node->right = newNodeLeft;

    return newNode;
}

Node * rightRotation(Node * node)
{
    Node * newNode = node->left;
    Node * newNodeRight = node->left->right;

    newNode->right = node;
    node->left = newNodeRight;

    return newNode;
}
