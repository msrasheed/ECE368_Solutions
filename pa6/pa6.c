#include <stdlib.h>
#include <stdio.h>
#include "seqpair.h"

int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "not proper amount of inputs\n");
        return EXIT_FAILURE;
    }

    FILE * infptr = fopen(argv[1], "r");
    if (infptr == NULL)
    {
        fprintf(stderr, "Input file failed to open or doesn't exist\n");
        return EXIT_FAILURE;
    }

    listnode ** hcg = NULL;
    listnode ** vcg = NULL;
    double * widths = NULL;
    double * heights = NULL;

    int size = parseInputData(infptr, &hcg, &vcg, &widths, &heights);
    fclose(infptr);

    double * x = calculate_x(hcg, widths, size);
    double * y = calculate_y(vcg, heights, size);

    FILE * outfptr = fopen(argv[2], "w");
    if (outfptr == NULL)
    {
        fprintf(stderr, "Output file failed to open or doesn't exist\n");
        return EXIT_FAILURE;
    }
    writeOuput(outfptr, x, y, size);
    fclose(outfptr);

    destroyGraph(hcg, size);
    destroyGraph(vcg, size);
    free(widths);
    free(heights);
    free(x);
    free(y);

    return EXIT_SUCCESS;
}
