#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

long *Load_Into_Array(char *Filename, int *Size);
int Save_From_Array(char *Filename, long *Array, int Size);
int checkSorted(long * arr, int size);
void printArray(long * arr, int size);

int main (int argc, char ** argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "not enough input arguments\n");
        return EXIT_FAILURE;
    }

    int size = 0;
    long * arr = Load_Into_Array(argv[2], &size);

    //long arr2[] = {-2010884815871183238,2557845556366131292,7018955703101582110,2670323672438268248,8793203910231321379,2262850779443812467,5070109031671207187,-782291229912307780,5996608892763635662,7287682272799027857};


    fprintf(stdout, "sorted?: %d\n", checkSorted(arr, size));
    printArray(arr, size);

    if (argv[1][1] == 'q')
    {
        Quick_Sort(arr, size);
    }
    else if (argv[1][1] == 'm')
    {
        Merge_Sort(arr, size);
    }

    printArray(arr, size);
    fprintf(stdout, "sorted?: %d\n", checkSorted(arr, size));

    Save_From_Array(argv[3], arr, size);
    free(arr);

    return EXIT_SUCCESS;
}

long *Load_Into_Array(char *Filename, int *Size)
{
    FILE * infptr = fopen(Filename, "r");
    if (infptr == NULL)
    {
        fprintf(stderr, "input file doesn't exist\n");
        return NULL;
    }
    fseek(infptr, 0, SEEK_END);
    *Size = ftell(infptr) / sizeof(long);
    //fprintf(stdout, "size: %ld\n", ftell(infptr));

    long * arr = malloc(sizeof(long) * (*Size));
    if (arr == NULL)
    {
        fprintf(stderr, "failed to allocate memory for array\n");
        return NULL;
    }

    fseek(infptr, 0, SEEK_SET);
    fread(arr, sizeof(long), *Size, infptr);
    fclose(infptr);

    return arr;
}

int Save_From_Array(char *Filename, long *Array, int Size)
{
    FILE * outfptr = fopen(Filename, "w");
    if (outfptr == NULL)
    {
        fprintf(stderr, "output file doesn't exist\n");
        return 0;
    }
    int n = fwrite(Array, sizeof(long), Size, outfptr);
    fclose(outfptr);
    return n;
}

int checkSorted(long * arr, int size)
{
    int i;
    for (i = 0; i < size - 1; i++)
    {
        if (arr[i] > arr[i+1])
        {
            return 0;
        }
    }
    return 1;
}

void printArray(long * arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(stdout, "%ld\n", arr[i]);
    }
    fprintf(stdout, "\n");
}
