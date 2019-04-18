#include <stdio.h>

typedef struct LISTNODE
{
    struct LISTNODE * next;
    int node;
} listnode;

int parseInputData(FILE * infile, listnode *** hcg_ret, listnode *** vcg_ret, double ** widths_ret, double ** heights_ret);
double * calculate_x (listnode ** hcg, double * widths, int size);
double * calculate_y (listnode ** vcg, double * heights, int size);
void writeOuput(FILE * outfile, double * x, double * y, int size);
void destroyGraph(listnode ** graph, int size);
