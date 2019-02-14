#include "sequence.h"
#include <stdlib.h>
#include <stdio.h>

long my_pow(int base, int exp)
{
  if (exp == 0)
  {
    return 1;
  }
  if(exp == 1)
  {
    return base;
  }
  int temp = my_pow(base, exp/2);
  temp *= temp;
  if (exp%2 == 1)
  {
    temp *= base;
  }
  return temp;
}

void sortSeq(long * seq, int size)
{
  int i;
  int j;
  for (i = size - 1; i > 0; i--)
  {
    if (seq[i] < seq[i - 1])
    {
      long temp = seq[i];
      seq[i] = seq[i - 1];
      seq[i - 1] = temp;
    }
  }

  for (i = 2; i < size; i++)
  {
    for (j = i; j > 0; j--)
    {
      if (seq[j] < seq[j - 1])
      {
        long temp = seq[j];
        seq[j] = seq[j - 1];
        seq[j - 1] = temp;
      }
    }
  }
}

long * Generate_2p3q_Seq(int length, int *seq_size)
{
  int numRows = 1;
  int dummyLen = length;
  while (dummyLen > 1)
  {
    dummyLen /= 2;
    numRows++;
  }
  int i;
  int countGuess = 0;
  for (i = 1; i <= numRows; i++)
  {
    countGuess += i;
  }
  //fprintf(stdout, "%d\n", countGuess);
  long * seq = malloc(sizeof(long) * countGuess);
  int index = 0;
  int j;
  for (i = 1; i <= numRows; i++)
  {
    for (j = 0; j < i; j++)
    {
      long num = my_pow(2, i - 1 - j) * my_pow(3, j);
      if (num < length)
      {
        seq[index] = num;
        index++;
      }
    }
  }
  *seq_size = index;
  seq = realloc(seq, sizeof(long) * (*seq_size));
  sortSeq(seq, *seq_size);
  return seq;
}
