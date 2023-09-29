#pragma once

#include <err.h>
#include "stdio.h"
#include "stdlib.h"

typedef struct list
{
  struct list *next;
  void        *data;
} List;

/**
 * @brief Initialize a linked list
 *
 * @return List*
 */
List *initList();
/**
 * @brief Insert an element in the list
 *
 * @param l
 * @param data
 */
void listInsert(List *l, void *data);

/**
 * @brief Remove a data element from the list
 *
 * @param l
 * @param data
 */
void listRemove(List *l, void *data);

/**
 * @brief Free the list
 *
 * @param l
 */
void freeList(List *l);

/**
 * @brief Get the length of the list
 *
 * @param l
 * @return int
 */
int listLength(List *l);
