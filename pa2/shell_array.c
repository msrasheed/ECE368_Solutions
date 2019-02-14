#include "shell_array.h"
#include "sequence.h"
#include <stdlib.h>
#include <stdio.h>

long *Array_Load_From_File(char *filename, int *size)
{
  FILE * infptr = fopen(filename, "r");
  fseek(infptr, 0, SEEK_END);
  *size = ftell(infptr) / sizeof(long);
  fseek(infptr, 0, SEEK_SET);
  long * nums = malloc(sizeof(long) * (*size));
  fread(nums,  sizeof(long), *size, infptr);
  fclose(infptr);
  return nums;
}

int Array_Save_To_File(char *filename, long *array, int size)
{
  FILE * outfptr = fopen(filename, "w");
  int numWrit = fwrite(array, size, sizeof(long), outfptr);
  fclose(outfptr);
  return numWrit;
}

void printArray1(long * arr, int size, int nsort)
{
  int i;
  for (i = 0; i < size; i++)
  {
    if (i%nsort == 0)
    {
      fprintf(stdout, "------\n");
    }
    fprintf(stdout, "%ld\n", arr[i]);
  }
  fprintf(stdout, "\n");
}

void Array_Shellsort(long *array, int size, double *n_comp)
{
  int seq_size;
  long * sortSeq = Generate_2p3q_Seq(size, &seq_size);
  double comps = 0;
  //printArray1(array, size, size);

  int i;
  for (i = seq_size - 1; i >= 0; i--) //iterate through k= sort numbers from sortSeq
  {
    long nsort = sortSeq[i];
    int j;
    for (j = 0; j < nsort; j++) //iterate through sub arrays
    {
      int endInd = size/nsort;
      if (j + nsort*endInd >= size) endInd--;
      int k;

      for (k = j+nsort*endInd; k > j; k -= nsort) // get smallest item in subarray to front
      {
        comps++;
        if (array[k] < array[k - nsort])
        {
          long temp = array[k];
          array[k] = array[k - nsort];
          array[k - nsort] = temp;
        }
      }

      for (k = j+nsort*2; k <= j+nsort*endInd; k+=nsort) // sort rest of subarray
      {
        int d;
        for (d = k; d > j; d-=nsort)
        {
          comps++;
          if(array[d] < array[d - nsort])
          {
            long temp = array[d];
            array[d] = array[d - nsort];
            array[d - nsort] = temp;
          }
          else break;
        }
      }
    }
    //fprintf(stdout, "%ld\n", nsort);
    //printArray1(array, size, nsort);
  }
  *n_comp = comps;
}
