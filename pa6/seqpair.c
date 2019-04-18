#include "seqpair.h"
#include <stdlib.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2

void printIntArray(int * arr, int size, int vert)
{
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(stdout, "%d ", arr[i]);
        if (vert) fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

void printDoubleArray(double * arr, int size, int vert)
{
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(stdout, "%lf ", arr[i]);
        if (vert) fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

void printStack(listnode * stack)
{
    listnode * n = stack;
    listnode * p = NULL;

    if (n == NULL)
    {
        fprintf(stdout, "stack is empty\n");
        return;
    }

    fprintf(stdout, "%d", n->node);
    p = n;
    n = n->next;

    while (n != NULL)
    {
        p = n;
        n = n->next;
        fprintf(stdout, ", %d", p->node);
    }
    fprintf(stdout, "\n\n");
}

void printGraph(listnode ** graph, int size)
{
    int i;
    for (i = 1; i < size + 1; i++)
    {
        fprintf(stdout, "%d: ", i);
        if (graph[i] != NULL)
        {
            listnode * n = graph[i];
            fprintf(stdout, "%d", n->node);
            n = n->next;
            while (n != NULL)
            {
                fprintf(stdout, ", %d", n->node);
                n = n->next;
            }
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

listnode * create_listnode(int val)
{
    listnode * ln = malloc(sizeof(listnode));
    if (ln == NULL)
    {
        fprintf(stderr, "failed to allocate listnode\n");
        return NULL;
    }
    ln->node = val;
    ln->next = NULL;
    return ln;
}

void destroyLinkedList(listnode * ln)
{
    listnode * n = ln;
    listnode * p = NULL;
    while(n != NULL)
    {
        p = n;
        n = n->next;
        free(p);
    }
}

void push(listnode ** stack, int val)
{
    listnode * ln = create_listnode(val);
    ln->next = *stack;
    *stack = ln;
}

int pop(listnode ** stack)
{
    if (*stack != NULL)
    {
        listnode * ln = (*stack)->next;
        int val = (*stack)->node;
        free(*stack);
        *stack = ln;
        return val;
    }
    return 0;
}

int parseInputData(FILE * infile, listnode *** hcg_ret, listnode *** vcg_ret, double ** widths_ret, double ** heights_ret)
{
    int numNodes;
    fscanf(infile, "%d\n", &numNodes);
    numNodes++;

    listnode ** hcg = malloc(numNodes * sizeof(listnode *));
    if (hcg == NULL)
    {
        fprintf(stderr, "failed to allocate hcg\n");
        return -1;
    }
    int i;
    for (i = 0; i < numNodes; i++)
    {
        hcg[i] = NULL;
    }

    listnode ** vcg = malloc(numNodes * sizeof(listnode *));
    if (vcg == NULL)
    {
        fprintf(stderr, "failed to allocate vcg\n");
        return -1;
    }
    for (i = 0; i < numNodes; i++)
    {
        vcg[i] = NULL;
    }

    double * widths = malloc(numNodes * sizeof(double));
    if (widths == NULL)
    {
        fprintf(stderr, "failed to allocate width\n");
        return -1;
    }

    double * heights = malloc(numNodes * sizeof(double));
    if (heights == NULL)
    {
        fprintf(stderr, "failed to allocate width\n");
        return -1;
    }

    for (i = 1; i < numNodes; i++)
    {
        int num;
        double height;
        double width;
        fscanf(infile, "%d(%le,%le)\n", &num, &width, &height);
        widths[num] = width;
        heights[num] = height;
    }

    numNodes--;
    int * seq1 = malloc(numNodes * sizeof(int));
    if (seq1 == NULL)
    {
        fprintf(stdout, "failed to allocate seq1\n");
        return -1;
    }

    int * seq2 = malloc(numNodes * sizeof(int));
    if (seq2 == NULL)
    {
        fprintf(stdout, "failed to allocate seq2\n");
        return -1;
    }

    for (i = 0; i < numNodes; i++)
    {
        fscanf(infile, "%d ", &seq1[i]);
    }

    for (i = 0; i < numNodes; i++)
    {
        fscanf(infile, "%d ", &seq2[i]);
    }

    //printIntArray(seq1, numNodes, 0);
    //printIntArray(seq2, numNodes, 0);
    //printDoubleArray(widths, numNodes + 1, 1);
    //printDoubleArray(heights, numNodes + 1, 1);

    for (i = 0; i < numNodes; i++)
    {
        int ind2 = 0;
        while (seq1[i] != seq2[ind2]) ind2++;
        int j;
        for (j = i + 1; j < numNodes; j++)
        {
            int k;
            for (k = ind2 + 1; k < numNodes; k++)
            {
                if (seq1[j] == seq2[k])
                {
                    listnode * ln = create_listnode(seq1[j]);
                    if (hcg[seq1[i]] == NULL) hcg[seq1[i]] = ln;
                    else
                    {
                        ln->next = hcg[seq1[i]];
                        hcg[seq1[i]] = ln;
                    }
                    break;
                }
            }
        }
    }

    for (i = 0; i < numNodes; i++)
    {
        int ind2 = 0;
        while (seq1[i] != seq2[ind2]) ind2++;
        int j;
        for (j = i - 1; j >= 0; j--)
        {
            int k;
            for (k = ind2 + 1; k < numNodes; k++)
            {
                if (seq1[j] == seq2[k])
                {
                    listnode * ln = create_listnode(seq1[j]);
                    if (vcg[seq1[i]] == NULL) vcg[seq1[i]] = ln;
                    else
                    {
                        ln->next = vcg[seq1[i]];
                        vcg[seq1[i]] = ln;
                    }
                    break;
                }
            }
        }
    }

    //printGraph(hcg, numNodes);
    //printGraph(vcg, numNodes);

    free(seq1);
    free(seq2);
    *hcg_ret = hcg;
    *vcg_ret = vcg;
    *widths_ret = widths;
    *heights_ret = heights;
    return numNodes;
}

void dfs(int node, listnode ** graph, int size, int * color, listnode ** stack)
{
    if (graph[node] == NULL)
    {
        push(stack, node);
        return;
    }

    listnode * n = graph[node];
    listnode * p = NULL;

    while (n != NULL)
    {
        p = n;
        n = n->next;
        if (color[p->node] == WHITE)
        {
            color[p->node] = BLACK;
            dfs(p->node, graph, size, color, stack);
        }
    }

    push(stack, node);
}

listnode * topological_sort(listnode ** graph, int size)
{
    listnode * stack = NULL;
    int * color = malloc((size + 1) * sizeof(int));
    int i;
    for (i = 0; i < size + 1; i++)
    {
        color[i] = WHITE;
    }

    for(i = 1; i < size + 1; i++)
    {
        if (color[i] == WHITE)
        {
            color[i] = BLACK;
            dfs(i, graph, size, color, &stack);
        }
    }

    free(color);
    return stack;
}

double * calculate_x (listnode ** hcg, double * widths, int size)
{
    double * x = malloc((size + 1) * sizeof(double));

    int i;
    for (i = 0; i < size + 1; i++)
    {
        x[i] = 0;
    }

    listnode * order = topological_sort(hcg, size);
    //printStack(order);

    while (order != NULL)
    {
        int node = pop(&order);
        if (hcg[node] == NULL) continue;

        listnode * n = hcg[node];

        while (n != NULL)
        {
            if (x[n->node] < x[node] + widths[node])
            {
                x[n->node] = x[node] + widths[node];
            }

            n = n->next;
        }
    }

    //printDoubleArray(x, size + 1, 1);
    return x;
}

double * calculate_y (listnode ** vcg, double * heights, int size)
{
    double * y = malloc((size + 1) * sizeof(double));

    int i;
    for (i = 0; i < size + 1; i++)
    {
        y[i] = 0;
    }

    listnode * order = topological_sort(vcg, size);
    //printStack(order);

    while (order != NULL)
    {
        int node = pop(&order);
        if (vcg[node] == NULL) continue;

        listnode * n = vcg[node];

        while (n != NULL)
        {
            if (y[n->node] < y[node] + heights[node])
            {
                y[n->node] = y[node] + heights[node];
            }

            n = n->next;
        }
    }

    //printDoubleArray(y, size + 1, 1);

    return y;
}

void writeOuput(FILE * outfile, double * x, double * y, int size)
{
    int i;
    for (i = 1; i < size + 1; i++)
    {
        fprintf(outfile, "%d(%le,%le)\n", i, x[i], y[i]);
    }
    return;
}

void destroyGraph(listnode ** graph, int size)
{
    int i;
    for (i = 0; i < size + 1; i++)
    {
        if (graph[i] != NULL)
        {
            destroyLinkedList(graph[i]);
        }
    }
    free(graph);
}
