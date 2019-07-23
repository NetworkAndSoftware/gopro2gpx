// Sun Jul 21 09:34:39 DST 2019
// Copyright Michiel van Wessem

#ifndef LIST_H
#define LIST_H

typedef struct node
{
  const void* value;
  struct node* next;
} node;

node* list_node(const void* value);
void list_free(node** head);

node* list_tail(node* head);
node* list_previous(node* n, node* head);

void list_append(node* n, node* head);
void list_insert(node* n, node** head);

int list_find(node* n, node* head);
int list_remove_one(node* n, node** head);

int list_size(node* head);
node* list_element_at_index(const int i, node* head);   
node* list_remove_at_index(const int i, node** head); // returns removed element, so that the caller may free it.
void list_insert_at_index(int i, node* n, node** head);

const void* list_popleft(node** head);

#endif // LIST_H
