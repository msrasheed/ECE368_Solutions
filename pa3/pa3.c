#include <stdlib.h>
#include <stdio.h>
#include "delay.h"

int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "Not enough input arguments\n");
        return EXIT_FAILURE;
    }
    FILE * inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Input file doesn't exist\n");
        return EXIT_FAILURE;
    }
    double r, rd;
    Node * tree = load_tree(inptr, &r, &rd);
    fclose(inptr);

    FILE * preptr = fopen(argv[2], "w");
    if (preptr == NULL)
    {
        fprintf(stderr, "Output file 1 doesn't exist or could not be created\n");
        return EXIT_FAILURE;
    }
    preorder_traversal(preptr, tree);
    fclose(preptr);

    FILE * delptr = fopen(argv[3], "w");
    if (delptr == NULL)
    {
        fprintf(stderr, "Output file 2 doesn't exist or could not be created\n");
        return EXIT_FAILURE;
    }
    preorder_delays(delptr, tree, rd, r);
    fclose(delptr);

    destroy_tree(tree);
    return EXIT_SUCCESS;
}
