#include <stdio.h>
#include <stdlib.h>

#include "container/list.h"

int main() {
  dlist_t* lst = NULL;
  // Add element
  {
    int* data = calloc(1, sizeof(int));
    *data = 5;
    list_push_back(&lst, data);
    printf("add element: %d front: %d back: %d\n", *data, *(int*)(list_front(lst))->data,
           *(int*)(list_back(lst))->data);
  }
  // Add element
  {
    int* data = calloc(1, sizeof(int));
    *data = 7;
    list_push_back(&lst, data);
    printf("add element: %d front: %d back: %d\n", *data, *(int*)(list_front(lst))->data,
           *(int*)(list_back(lst))->data);
  }
  // Add element
  {
    int* data = calloc(1, sizeof(int));
    *data = 3;
    list_push_back(&lst, data);
    printf("add element: %d front: %d back: %d\n", *data, *(int*)(list_front(lst))->data,
           *(int*)(list_back(lst))->data);
  }
  printf("list size %d\n", list_size(lst));
  // Print list
  while (lst) {
    int cur = *(int*)lst->data;
    printf("element: %d\n", cur);
    lst = lst->next;
  }
  // Clear list
  list_clear(&lst, NULL);
  printf("list size %d\n", list_size(lst));
  return 0;
}
