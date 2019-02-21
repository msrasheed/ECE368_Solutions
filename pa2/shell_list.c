#include "shell_list.h"
#include "sequence.h"
#include <stdio.h>
#include <stdlib.h>

//print linked list helper function
void printLinkArr1(Node * head)
{
  while(head != NULL)
  {
    fprintf(stdout, "%ld\n", head->value);
    head = head->next;
  }
}

//load long integers from file as linked list
Node *List_Load_From_File(char *filename)
{
  FILE * infptr = fopen(filename, "r");
  if (infptr == NULL)
  {
    return NULL;
  }
  long read;
  int num = fread(&read, sizeof(long), 1, infptr);
  if (num == 0) return NULL;
  Node * head = malloc(sizeof(Node));
  if (head == NULL) return NULL;
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
      if (n2 == NULL) return NULL;
      n2->value = read;
      n->next = n2;
      n = n2;
    }
  }
  n->next = NULL;
  fclose(infptr);
  return head;
}

// save linked list of long integers to file
int List_Save_To_File(char *filename, Node *list)
{
  FILE * outfptr = fopen(filename, "w");
  if (list == NULL)
  {
    fclose(outfptr);
    return 0;
  }
  int count = 0;
  while (list != NULL)
  {
    fwrite(&(list->value), sizeof(long), 1, outfptr);
    list = list->next;
    count++;
  }
  fclose(outfptr);
  return count;
}

//definition of list for a linked list of linked lists
typedef struct List {
  Node *node;
  struct List *next;
} List;

//print out the entire linked list of linked lists
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

//move ahead a certain number of nodes in the linked lists
//not used anymore
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

//used to free lists
void destroyLists(List * list)
{
  List * l = list->next;
  while (l != NULL)
  {
    free(list);
    list = l;
    l = list->next;
  }
  free(list);
}

//creates k numbe ro fsubarrays from linked list
List * Create_Subarrays2(Node * head, long k)
{
  //fprintf(stdout, "\n\n");
  //printLinkArr1(head);
  List * newList = malloc(sizeof(List));
  //node tracker is used to move along the subarrays so that the actual
  //linked list of linked lists can stay pointing at just the head of the subarrays
  List * nodeTracker = malloc(sizeof(List));
  if (newList == NULL || nodeTracker == NULL)
  {
    return NULL;
  }
  nodeTracker->node = head;
  newList->node = head;
  head = head->next;
  List * nl = newList;
  List * ntl = nodeTracker;
  int i;
  //initialize subarrays
  for (i = 0; i < k - 1; i++)
  {
    List * nl2 = malloc(sizeof(List));
    List * ntl2 = malloc(sizeof(List));
    if (nl2 == NULL || ntl2 == NULL)
    {
      return NULL;
    }
    nl2->node = head;
    ntl2->node = head;
    head = head->next;
    nl->next = nl2;
    nl = nl->next;
    ntl->next = ntl2;
    ntl = ntl->next;
  }
  nl->next = NULL;
  ntl->next = NULL;

  // fill in the rest of the subarrays
  nl = newList;
  ntl = nodeTracker;
  while(head != NULL)
  {
    ntl->node->next = head;
    head = head->next;
    ntl->node = ntl->node->next;
    ntl = ntl->next;
    if (ntl == NULL) ntl = nodeTracker;
  }
  ntl = nodeTracker;

  //add null termination to all subarrays
  while (ntl != NULL)
  {
    ntl->node->next = NULL;
    ntl = ntl->next;
  }
  destroyLists(nodeTracker);
  return newList;
}

//previous version of Create_Subarrays, not used anymore
//much slower
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

//merge linked list of linked lists into one large linked list again
//merge subarrays into linked list again
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

//preform insertion sort on a linked list
//used iteratively on each subarray
Node * insertionListSort(Node * head, double * comps)
{
  Node * nold;
  Node * pold;
  Node * newList;
  Node * nnew;
  Node * pnew;

  newList = head;
  nold = head->next;
  newList->next = NULL;
  Node * front = malloc(sizeof(Node));
  front->next = newList;
  while(nold != NULL)
  {
    nnew = front->next;
    pnew = front;
    pold = nold->next;
    *comps = *comps + 1;
    while (nnew != NULL && nold->value < nnew->value)
    {
      pnew = nnew;
      nnew = nnew->next;
    }
    if (nnew == NULL) {

    }
    pnew->next = nold;
    nold->next = nnew;
    nold = pold;
  }
  Node * nf = front->next;
  free(front);

  Node * placeH;
  newList = nf;
  nf = nf->next;
  newList->next = NULL;
  while (nf != NULL)
  {
      placeH = nf->next;
      nf->next = newList;
      newList = nf;
      nf = placeH;
  }
  return newList;
}

//sort linked list using shell sort
Node *List_Shellsort(Node *list, double *n_comp)
{
  if (list == NULL)
  {
    *n_comp = 0;
    return NULL;
  }
  int seq_size;
  int size = 0;
  Node * head = list;

  //figure out size of linked list
  while (list != NULL)
  {
    size++;
    list = list->next;
  }

  //get sequence to shell sort with
  long * sortSeq = Generate_2p3q_Seq(size, &seq_size);
  double comps = 0;
  if (sortSeq == NULL)
  {
    *n_comp = comps;
    return NULL;
  }

  //iterate through numbers to shell sort with in sortSeq
  int i;
  for (i = seq_size - 1; i > 0; i--)
  {
    long nsort = sortSeq[i];

    //create subarrays
    List * subHead = Create_Subarrays2(head, nsort);//, size);
    if (subHead == NULL) return NULL;
    //printSubList(subHead);
    //sort each subarray
    List * sl = subHead;
    while (sl != NULL)
    {
      sl->node = insertionListSort(sl->node, &comps);
      sl = sl->next;
    }
    //merge subarrays after sorting them
    //printSubList(subHead);
    head = mergeSubarrays(subHead);
    destroyLists(subHead);
    //fprintf(stdout, "yo %ld\n", nsort);
  }
  //one last insertion sort, basically k = 1 since was skipped above
  head = insertionListSort(head, &comps);
  *n_comp = comps;
  free(sortSeq);
  return head;
}
