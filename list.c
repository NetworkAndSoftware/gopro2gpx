#include "list.h"
#include <stdlib.h>

// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

node* list_node(const void* value)
{
  node *p = malloc(sizeof(node));
  p->value = value;
  p->next = 0;
  return p;
}

void list_free(node** head)
{
  if (head == NULL || *head == NULL)
    return;

  if ((*head)->next)
    list_free(&(*head)->next);
  free(*head);
  *head = 0;
}

node* list_tail(node* head)
{
  if (head == NULL)
    return NULL;

  while (head->next != NULL)
    head = head->next;

  return head;
}

node* list_previous(node* n, node* head)
{
  if (head == NULL || head == n)
    return NULL;

  for (; head->next != NULL; head = head->next)
    if (head->next == n)
      return head;

  return NULL;
}

void list_append(node* n, node* head)
{
  node* tail = list_tail(head);

  if (tail != NULL)
    tail->next = n;
}

void list_insert(node* n, node** head)
{
  if (head == NULL || *head == NULL || n == NULL)
    return;

  n->next = *head;
  *head = n;
}

int list_find(node* n, node* head)
{
  if (n == NULL)
    return 0;

  for (;head != NULL; head = head->next)
  {
    if (head == n)
      return 1;
  }

  return 0;
}

int list_remove_one(node* n, node** head)
{
  if (head == NULL || *head == NULL || n == NULL)
    return 0;

  if (*head == n)
  {
    *head = n->next;
    n->next = 0;
    return 1;
  }

  node* previous = list_previous(n, *head);

  if (previous == NULL)
    return 0;

  previous->next = previous->next->next;
  n->next = NULL;
  return 1;
}

int list_size(node *head)
{
  int i = 0;
  for (; head != NULL; head = head->next)
    i++;

  return i;
}

node* list_element_at_index(const int i, node* head)
{
  for (int j = 0; head != NULL; head = head->next)
  {
    if (j == i)
      return head;
    j++;
  }

  return NULL;
}


node* list_remove_at_index(const int i, node** head) // returns removed element, so that the caller may free it.
{
  if (head == NULL || *head == NULL)
    return NULL;

  if (i == 0)
  {
    node* n = *head;
    *head = (*head)->next;
    return n;
  }

  const node* previous = list_element_at_index(i - 1, *head);

  node* removed = previous->next;
  previous->next->next = previous->next;
  removed->next = NULL;

  return removed;
}

void list_insert_at_index(int i, node* n, node** head)
{
  if (head == NULL || *head == NULL)
    return;

  if (n == 0)
  {
    n->next = *head;
    *head = n;
    return;
  }

  node* previous = list_element_at_index(i - 1, *head);

  if (previous == NULL)
    return;

  n->next = previous->next;
  previous->next = n;
}

const void* list_popleft(node** head)
{
  if (head == NULL || *head == NULL)
    return NULL;

  node* node = *head;
  const void* value = node->value;
  *head = node->next;
  free(node);
  return value;
}