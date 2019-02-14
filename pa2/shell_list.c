#include "shell_list.h"
#include <stdio.h>
#include <stdlib.h>

void printLinkArr1(Node * head)
{
  while(head != NULL)
  {
    fprintf(stdout, "%ld\n", head->value);
    head = head->next;
  }
}

Node *List_Load_From_File(char *filename)
{
  FILE * infptr = fopen(filename, "r");
  long read;
  int num = fread(&read, sizeof(long), 1, infptr);
  Node * head = malloc(sizeof(Node));
  head->value = read;
  Node * n;
  Node * n2;
  n = head;
  while (num > 0)
  {
    num = fread(&read, sizeof(long), 1, infptr);
    if (num > 0)
    {
      n2 = malloc(sizeof(Node));
      n2->value = read;
      n->next = n2;
      n = n2;
    }
  }
  n->next = NULL;
  return head;
}

int List_Save_To_File(char *filename, Node *list)
{
  FILE * outfptr = fopen(filename, "w");
  int count = 0;
  while (list != NULL)
  {
    fwrite(&(list->value), sizeof(long), 1, outfptr);
    list = list->next;
    count++;
  }
  return count;
}

Node *List_Shellsort(Node *list, double *n_comp)
{
  return NULL;
}
