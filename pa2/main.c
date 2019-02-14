#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"
#include "sequence.h"
#include "shell_list.h"

void printArray(long * arr, int size);
void printLinkArr(Node * head);

int main(int argc, char ** argv)
{
  if (argc < 4)
  {
    fprintf(stderr, "not enough input arguments\n");
    return EXIT_FAILURE;
  }

  int size;
  if (argv[1][1] == 'a')
  {
    long * arr = Array_Load_From_File(argv[2], &size);
    //printArray(arr, size);
    //fprintf(stdout, "%d\n\n", size);
    double comps;
    Array_Shellsort(arr, size, &comps);
    //printArray(arr, size);
    fprintf(stdout, "\n%E\n\n", comps);
    Array_Save_To_File(argv[3], arr, size);
  }
  else if (argv[1][1] == 'l')
  {
    Node * head = List_Load_From_File(argv[2]);
    printLinkArr(head);
  }
}

void printArray(long * arr, int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    fprintf(stdout, "%ld\n", arr[i]);
  }
  //fprintf(stdout, "\n");
}

void printLinkArr(Node * head)
{
  while(head != NULL)
  {
    fprintf(stdout, "%ld\n", head->value);
    head = head->next;
  }
}
