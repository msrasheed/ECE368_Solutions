#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        return EXIT_FAILURE;
    }

    if (argv[1][1] == 'b')
    {
        if (argc < 4)
        {
            return EXIT_FAILURE;
        }

        FILE * infptr = fopen(argv[2], "r");
        if (infptr == NULL)
        {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }
        int form = 1;
        Node * tree = buildTree(infptr, &form);
        fclose(infptr);

        FILE * outfptr = fopen(argv[3], "w");
        if (outfptr == NULL)
        {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }
        preOrderTraversal(tree, outfptr);
        fprintf(stdout, "%d\n", form);
        fclose(outfptr);
        destroyTree(tree);
    }
    else if (argv[1][1] == 'e')
    {
        int form = -1;
        int bst = 0;
        int baltree = 0;

        FILE * infptr = fopen(argv[2], "r");
        if (infptr == NULL)
        {
            fprintf(stdout, "%d,%d,%d\n", form, bst, baltree);
            return EXIT_FAILURE;
        }
        evaluateTree(infptr, &form, &bst, &baltree);
        fprintf(stdout, "%d,%d,%d\n", form, bst, baltree);
        fclose(infptr);
    }

    return EXIT_SUCCESS;
}
