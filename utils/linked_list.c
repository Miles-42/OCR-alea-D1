#include "linked_list.h"

// Code functions for initializing a linked List, inserting an element,
// removing an element, freeing the List, getting the length of the List.

List *initList()
{
  List *l = malloc(sizeof(List));
  if (l == NULL)
  {
    err(1, "malloc");
  }
  l->next = NULL;
  l->data = NULL;
  return l;
}

void listInsert(List *l, void *data)
{
  List *new = malloc(sizeof(List));
  if (new == NULL)
  {
    err(1, "malloc");
  }
  new->data = data;
  new->next = l->next;
  l->next   = new;
}

void listRemove(List *l, void *data)
{
  List *prev = l;
  List *curr = l->next;
  while (curr != NULL)
  {

    if (curr->data == data)
    {
      // printf("Removing %p\n", data);
      prev->next = curr->next;
      free(curr);
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}

void freeList(List *l)
{
  List *curr = l;
  List *next = l->next;
  while (next != NULL)
  {
    free(curr);
    curr = next;
    next = next->next;
  }
  free(curr);
}

int listLength(List *l)
{

  int   length = 0;
  List *curr   = l->next;
  while (curr != NULL)
  {
    length++;
    curr = curr->next;
  }
  return length;
}
