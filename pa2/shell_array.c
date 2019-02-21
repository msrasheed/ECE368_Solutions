#include "shell_array.h"
#include "sequence.h"
#include <stdlib.h>
#include <stdio.h>

// load long integers into array from file
long *Array_Load_From_File(char *filename, int *size)
{
  FILE * infptr = fopen(filename, "r");
  if (infptr == NULL)
  {
    return NULL;
  }
  fseek(infptr, 0, SEEK_END);
  *size = ftell(infptr) / sizeof(long);
  fseek(infptr, 0, SEEK_SET);
  long * nums = malloc(sizeof(long) * (*size));
  if (nums == NULL)
  {
    return NULL;
  }
  fread(nums,  sizeof(long), *size, infptr);
  fclose(infptr);
  return nums;
}

// save long integer array to file
int Array_Save_To_File(char *filename, long *array, int size)
{
  FILE * outfptr = fopen(filename, "w");
  int numWrit = fwrite(array, size, sizeof(long), outfptr);
  fclose(outfptr);
  return numWrit;
}

//print array helper function
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

// shell sort list
void Array_Shellsort(long *array, int size, double *n_comp)
{
  if (size == 0)
  {
    *n_comp = 0;
    return;
  }
  int seq_size;
  //get sequence to shell sort by
  long * sortSeq = Generate_2p3q_Seq(size, &seq_size);
  if (sortSeq == NULL)
  {
    *n_comp = 0;
    return;
  }
  double comps = 0;
  //printArray1(array, size, size);

  int i;
  //iterate through k= sort numbers from sortSeq
  for (i = seq_size - 1; i >= 0; i--)
  {
    long nsort = sortSeq[i];
    int j;
    //iterate through sub arrays
    for (j = 0; j < nsort; j++)
    {
      int endInd = size/nsort;
      if (j + nsort*endInd >= size) endInd--;
      int k;

      // get smallest item in subarray to front
      for (k = j+nsort*endInd; k > j; k -= nsort)
      {
        comps++;
        if (array[k] < array[k - nsort])
        {
          long temp = array[k];
          array[k] = array[k - nsort];
          array[k - nsort] = temp;
        }
      }

      // insertion sort on rest of subarray
      for (k = j+nsort*2; k <= j+nsort*endInd; k+=nsort)
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
  free(sortSeq);
  *n_comp = comps;
}
