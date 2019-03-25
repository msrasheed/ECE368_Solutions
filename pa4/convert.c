#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    FILE * inptr;
    FILE * outptr;

    if (argv[1][1] == 'f')
    {
        inptr = fopen(argv[2], "r");
        outptr = fopen(argv[3], "w");

        while (!feof(inptr))
        {
            int key;
            char instr;
            fscanf(inptr, "%d %c\n", &key, &instr);
            fprintf(stdout, "%d %c\n", key, instr);

            fwrite(&key, sizeof(int), 1, outptr);
            fwrite(&instr, sizeof(char), 1, outptr);
        }
    }
    else if (argv[1][1] == 'r')
    {
        inptr = fopen(argv[3], "r");
        outptr = fopen(argv[2], "w");

        while (!feof(inptr))
        {
            int key;
            char instr;
            fread(&key, sizeof(int), 1, inptr);
            fread(&instr, sizeof(char), 1, inptr);
            if (!feof(inptr))
            {
                fprintf(outptr, "%d %c\n", key, instr);
            }
        }
    }

    fclose(inptr);
    fclose(outptr);
    return EXIT_SUCCESS;
}
