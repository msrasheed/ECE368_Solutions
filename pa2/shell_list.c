#include "shell_list.h"
#include "sequence.h"
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

typedef struct List {
  Node *node;
  struct List *next;
} List;

void printSubList(List * list)
{
  fprintf(stdout, "-------\n");
  while (list != NULL)
  {
    printLinkArr1(list->node);
    fprintf(stdout, "\n");
    list = list->next;
  }
  fprintf(stdout, "---------\n");
}

void goAhead(Node ** n, Node ** p, long num)
{
  int i = 0;
  while(*n != NULL)
  {
    if(i == num) return;
    (*p) = *n;
    (*n) = (*n)->next;
    i++;
  }
}

List * Create_Subarrays(Node * head, long k, int size)
{
  long origk = k;
  List * subHead = malloc(sizeof(List));
  List * nl = subHead;
  nl->node = head;
  head = head->next;
  Node * nnMain = head;
  Node * pnMain;
  Node * nList = nl->node;
  int i;
  int numElems = size/origk;
  if (size%origk == 0) numElems--;
  for(i = 0; i < numElems; i++)
  {
    goAhead(&nnMain, &pnMain, k - 1);
    nList->next = nnMain;
    nList = nList->next;
    pnMain->next = nnMain->next;
    nnMain = nnMain->next;
  }
  nList->next = NULL;
  k--;

  List * nl2;
  for (i = 0; i < origk - 2; i++)
  {
    nl2 = malloc(sizeof(List));
    nl->next = nl2;
    nl = nl->next;
    nl->node = head;
    head = head->next;
    nnMain = head;
    nList = nl->node;

    numElems = size/origk;
    if (i+origk*numElems >= size) numElems--;
    int j;
    for (j = 0; j < numElems; j++)
    {
      goAhead(&nnMain, &pnMain, k - 1);
      nList->next = nnMain;
      nList = nList->next;
      if (nnMain != NULL)
      {
        pnMain->next = nnMain->next;
        nnMain = nnMain->next;
      }
      else
      {
        pnMain->next = NULL;
        break;
      }
    }
    if (nList != NULL) nList->next = NULL;
    k--;
  }
  nl2 = malloc(sizeof(List));
  nl2->node = head;
  nl->next = nl2;
  return subHead;
}

Node * mergeSubarrays(List * list)
{
  Node * head = list->node;
  Node * n = head;
  list->node = list->node->next;
  List * nl = list->next;
  int i = 0;
  while (nl->node != NULL)
  {
    i++;
    n->next = nl->node;
    n = n->next;
    nl->node = nl->node->next;
    nl = nl->next;
    if (nl == NULL) nl = list;
    n->next = NULL;
    //printLinkArr1(head);
    //fprintf(stdout, "\n");
  }
  return head;
}

Node * insertionListSort(Node * head, double * comps)
{
  Node * nold = head->next;
  Node * pold;
  head->next = NULL;
  Node * newList = head;
  Node * nnew;
  Node * pnew;
  while(nold != NULL)
  {
    nnew = newList;
    pold = nold->next;
    *comps = *comps + 1;
    while( nnew != NULL && nold->value > nnew->value)
    {
      pnew = nnew;
      nnew = nnew->next;
    }
    if (nnew == newList)
    {
      nold->next = nnew;
      newList = nold;
    }
    else
    {
      pnew->next = nold;
      nold->next = nnew;
    }
    nold = pold;
  }
  return newList;
}

Node *List_Shellsort(Node *list, double *n_comp)
{
  int seq_size;
  int size = 0;
  Node * head = list;
  while (list != NULL) //figure out size of linked list
  {
    size++;
    list = list->next;
  }
  long * sortSeq = Generate_2p3q_Seq(size, &seq_size);
  double comps = 0;

  int i;
  for (i = seq_size - 1; i > 0; i--)
  {
    long nsort = sortSeq[i];

    //create subarrays
    List * subHead = Create_Subarrays(head, nsort, size);
    // fprintf(stdout, "before\n");
    // printSubList(subHead);
    List * sl = subHead;
    while (sl != NULL)
    {
      sl->node = insertionListSort(sl->node, &comps);
      sl = sl->next;
    }
    // fprintf(stdout, "%ld\n", nsort);
    // fprintf(stdout, "after\n");
    // printSubList(subHead);
    // fprintf(stdout, "\n\n");
    head = mergeSubarrays(subHead);
    // printLinkArr1(head);
  }
  head = insertionListSort(head, &comps);
  //printLinkArr1(head);
  *n_comp = comps;
  return head;
}
