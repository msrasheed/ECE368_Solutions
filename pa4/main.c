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
        Node * tree = parseInput(infptr);
        fclose(infptr);

        FILE * outfptr = fopen(argv[2], "w");
        if (outfptr == NULL)
        {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }
        preOrderTraversal(tree, outfptr);
        fclose(outfptr);
    }
    else if (argv[1][1] == 'e')
    {

    }

    return EXIT_SUCCESS;
}
