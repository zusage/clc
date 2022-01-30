/*******************************************************************************
** Copyright (c) 2012 - 2022 v.arteev

** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:

** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.

** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*******************************************************************************/
#include <stdarg.h>  // for 'vargs'
#include <stdlib.h>
#include <string.h>  // for 'memset'

#include "container/list.h"

typedef int func_ptr8_t(void*, void*, void*, void*, void*, void*, void*);
////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////
int call(int (*fn)(void* data), int count, void* data, char* buf[]);
// Call function pointer with parameters
int call(int (*fn)(void* data), int count, void* data, char* buf[]) {
  if (!fn || !data || !buf) return -1;
  // Call function
  int (*function)(void* data1, void* data2, void* data3, void* data4, void* data5, void* data6, void* data7) =
      ((func_ptr8_t*)fn);
  switch (count) {
    case 0:
      return function(data, NULL, NULL, NULL, NULL, NULL, NULL);
      break;
    case 1:
      return function(data, buf[0], NULL, NULL, NULL, NULL, NULL);
      break;
    case 2:
      return function(data, buf[0], buf[1], NULL, NULL, NULL, NULL);
      break;
    case 3:
      return function(data, buf[0], buf[1], buf[2], NULL, NULL, NULL);
      break;
    case 4:
      return function(data, buf[0], buf[1], buf[2], buf[3], NULL, NULL);
      break;
    case 5:
      return function(data, buf[0], buf[1], buf[2], buf[3], buf[4], NULL);
      break;
    case 6:
      return function(data, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
      break;
  }
  return -1;
}
////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////
dlist_t* list_push_back(dlist_t** lst, void* data) {
  if (!lst || !data) return NULL;

  dlist_t* cur = list_front(*lst);
  if (*lst == NULL) {
    dlist_t* node = (dlist_t*)calloc(1, sizeof(dlist_t));
    if (!node) {
      return NULL;
    }

    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    *lst = node;
    return *lst;
  } else {
    while (cur && (cur->next != NULL)) cur = cur->next;
  }

  dlist_t* node = (dlist_t*)calloc(1, sizeof(dlist_t));
  if (!node) return NULL;
  node->data = data;
  node->next = NULL;
  node->prev = cur;
  if (cur) cur->next = node;

  return node;
}
int list_push_back_unique(dlist_t** lst, const void* value, void* src,
                          int (*predicate)(const void* data1, const void* data2),
                          void (*copy)(const void* src_data, void* dst_data),
                          void* (*ctor_copy)(const void* src_data)) {
  if (!lst || !value || !src) return -1;

  ////////////////////////////////////////////////////////////////////////////
  // Find element
  ////////////////////////////////////////////////////////////////////////////
  const dlist_t* node = list_find(*lst, value, predicate);

  // Refresh element
  if (node) {
    if (copy) {
      copy(src, node->data);
      return 0;
    }
  }
  // Create element
  else {
    if (ctor_copy) {
      void* node_new = ctor_copy(src);
      if (!node_new) return -1;
      // Add to list
      list_push_back(lst, node_new);
      return 0;
    }
  }
  return -1;
}
void list_remove_if(dlist_t** lst, const void* value, int (*predicate)(const void* data1, const void* data2),
                    void (*deleter)(void* data)) {
  if (!lst || !predicate) return;

  dlist_t* lst_ = list_front(*lst);
  while (lst_) {
    if (0 == predicate(lst_->data, value)) {
      if (!lst_->prev && !lst_->next) {
        // Delete element
        if (deleter) {
          deleter(lst_->data);
          lst_->data = NULL;
        } else {
          // Free memory
          free(lst_->data);
          lst_->data = NULL;
        }
        free(lst_);
        *lst = lst_ = NULL;

        break;
      } else if (!lst_->prev && lst_->next) {
        // Reassign address node
        (lst_->next)->prev = NULL;
        *lst = lst_->next;
      } else if (lst_->prev && !lst_->next) {
        // Reassign address node
        (lst_->prev)->next = NULL;
        *lst = lst_->prev;
      } else {
        // Reassign address node
        if (lst_->prev) (lst_->prev)->next = lst_->next;
        if (lst_->next) (lst_->next)->prev = lst_->prev;
        *lst = lst_->prev;
      }
      // Delete element
      if (deleter) {
        deleter(lst_->data);
        lst_->data = NULL;
      } else {
        // Free memory
        free(lst_->data);
        lst_->data = NULL;
      }
      free(lst_);
      lst_ = NULL;

      break;
    }
    // Set next
    lst_ = lst_->next;
  }
  return;
}
void list_clear(dlist_t** lst, void (*deleter)(void* data)) {
  if (!lst || !(*lst)) return;
  dlist_t* lst_ = list_front(*lst);
  while (lst_) {
    dlist_t* tmp = lst_;
    // Set next
    lst_ = lst_->next;
    // Delete element
    if (deleter) {
      deleter(tmp->data);
      tmp->data = NULL;
    } else {
      // Free memory
      free(tmp->data);
      tmp->data = NULL;
    }
    free(tmp);
    tmp = NULL;
  }
  *lst = NULL;
}
const dlist_t* list_cfront(const dlist_t* lst) {
  if (!lst) return NULL;

  const dlist_t* cur = lst;
  while (cur && (cur->prev)) cur = cur->prev;
  return cur;
}
dlist_t* list_front(dlist_t* lst) {
  if (!lst) return NULL;

  dlist_t* cur = lst;
  while (cur && (cur->prev)) cur = cur->prev;
  return cur;
}
dlist_t* list_back(dlist_t* lst) {
  if (!lst) return NULL;

  dlist_t* cur = lst;
  while (cur && (cur->next != NULL)) cur = cur->next;
  return cur;
}
void list_pop_front(dlist_t** lst, void (*deleter)(void* data)) {
  if (!lst || !(*lst)) return;

  dlist_t* cur = list_front(*lst);
  if (!cur) return;
  // Next node
  dlist_t* tmp = NULL;
  // First node cannot have the previous node
  if (cur->prev) return;
  // Reassign address node
  if (cur->next) {
    (cur->next)->prev = NULL;
    tmp = cur->next;
  }
  // Delete element
  if (deleter) {
    deleter(cur->data);
    cur->data = NULL;
  } else {
    // Free memory
    free(cur->data);
    cur->data = NULL;
  }
  free(cur);
  cur = NULL;
  // Reassign address to the next node
  *lst = tmp;
  return;
}
void list_pop_back(dlist_t** lst, void (*deleter)(void* data)) {
  if (!lst || !(*lst)) return;

  dlist_t* cur = list_back(*lst);
  if (!cur) return;
  // Reassign address node
  if (cur->prev) (cur->prev)->next = NULL;
  // Last node cannot have the next node
  if (cur->next) return;
  // Delete node
  if (deleter) {
    deleter(cur->data);
    cur->data = NULL;
  } else {
    // Free memory
    free(cur->data);
    cur->data = NULL;
  }
  free(cur);
  cur = NULL;

  return;
}
int list_empty(const dlist_t* lst) {
  if (!lst) return 1;
  return 0;
}
int list_size(const dlist_t* lst) {
  if (!lst) return 0;

  int count = 0;
  // Get first
  const dlist_t* cur = list_cfront(lst);
  while (cur) {
    count++;
    // Set next
    cur = cur->next;
  }
  return count;
}
////////////////////////////////////////////////////////////////////////////
// Algorithms
////////////////////////////////////////////////////////////////////////////
// Applies function fn to each of the elements in the range [first,last).
int list_for_each(dlist_t* lst, int (*fn)(void* data)) {
  if (!lst) return 0;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    if (fn) fn(tmp->data);
  }
  return 0;
}
int list_for_each_extra(dlist_t* lst, int (*fn)(void* data), int count, ...) {
  if (!lst) return 0;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    // Call function
    if (fn) {
      char* buf[count];
      memset(buf, 0, sizeof(buf));
      va_list args;
      va_start(args, count);
      int i = 0;
      for (i = 0; i < count; ++i) buf[i] = va_arg(args, char*);
      va_end(args);
      // Call function
      call(fn, count, tmp->data, buf);
    }
  }
  return 0;
}
int list_any_of(dlist_t* lst, int (*fn)(void* data)) {
  if (!lst) return 0;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    if (fn)
      if (1 == fn(tmp->data)) return 1;
  }
  return 0;
}
int list_any_of_extra(dlist_t* lst, int (*fn)(void* data), int count, ...) {
  if (!lst) return 0;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    // Call function
    if (fn) {
      char* buf[count];
      memset(buf, 0, sizeof(buf));
      va_list args;
      va_start(args, count);
      int i = 0;
      for (i = 0; i < count; ++i) buf[i] = va_arg(args, char*);
      va_end(args);
      // Call function
      if (1 == call(fn, count, tmp->data, buf)) return 1;
    }
  }
  return 0;
}
int list_cany_of_extra(const dlist_t* lst, int (*fn)(void* data), int count, ...) {
  if (!lst) return 0;

  const dlist_t* cur = lst;
  while (cur) {
    const dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    // Call function
    if (fn) {
      char* buf[count];
      memset(buf, 0, sizeof(buf));
      va_list args;
      va_start(args, count);
      int i = 0;
      for (i = 0; i < count; ++i) buf[i] = va_arg(args, char*);
      va_end(args);
      // Call function
      if (1 == call(fn, count, tmp->data, buf)) return 1;
    }
  }
  return 0;
}
int list_all_of(dlist_t* lst, int (*fn)(void* data)) {
  if (!lst) return 1;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (fn)
      if (!fn(tmp->data)) return 0;
  }
  return 1;
}
int list_all_of_extra(dlist_t* lst, int (*fn)(void* data), int count, ...) {
  if (!lst) return 1;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    // Call function
    if (fn) {
      char* buf[count];
      memset(buf, 0, sizeof(buf));
      va_list args;
      va_start(args, count);
      int i = 0;
      for (i = 0; i < count; ++i) buf[i] = va_arg(args, char*);
      va_end(args);
      // Call function
      if (!call(fn, count, tmp->data, buf)) return 0;
    }
  }
  return 1;
}
int list_none_of(dlist_t* lst, int (*fn)(void* data)) {
  if (!lst) return 1;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (fn)
      if (1 == fn(tmp->data)) return 0;
  }
  return 1;
}
int list_none_of_extra(dlist_t* lst, int (*fn)(void* data), int count, ...) {
  if (!lst) return 1;

  dlist_t* cur = lst;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;
    // Call function
    if (fn) {
      char* buf[count];
      memset(buf, 0, sizeof(buf));
      va_list args;
      va_start(args, count);
      int i = 0;
      for (i = 0; i < count; ++i) buf[i] = va_arg(args, char*);
      va_end(args);
      // Call function
      if (1 == call(fn, count, tmp->data, buf)) return 0;
    }
  }
  return 1;
}
int list_distance(const dlist_t* first1, const dlist_t* last1) {
  if (!first1 || !last1) return -1;

  int count = 0;
  const dlist_t* cur = first1;
  while (cur && (cur < last1)) {
    count++;
    // Set next
    cur = cur->next;
  }
  return count;
}
int list_is_permutation(const dlist_t* first1, const dlist_t* first2,
                        int (*predicate)(const void* data1, const void* data2)) {
  if (!first1 != !first2) return 0;  // Have NOT a same elements

  int sz1 = list_size(first1);
  int sz2 = list_size(first2);

  if (sz1 != sz2) return 0;  // Have NOT a same elements

  if (!sz1 && !sz2) return 1;  // Have a same elements, because they are zero sizes.

  if (sz1) {
    const dlist_t* cur = first1;
    while (cur) {
      const dlist_t* tmp = cur;
      // Set next
      cur = cur->next;

      if (!list_cfind(first2, tmp->data, predicate)) return 0;  // Have NOT a same elements
    }
    return 1;  // Have a same elements
  }
  return 0;  // Have NOT a same elements
}
int list_copy(const dlist_t* src_lst, dlist_t** dst_lst, void* (*ctor)(size_t count), void (*dtor)(void* data),
              void (*copy)(const void* src_data, void* dst_data)) {
  if (!ctor || !copy) return -1;

  // Free destination list
  if (dtor) list_clear(dst_lst, dtor);

  const dlist_t* cur = src_lst;
  while (cur) {
    const dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    void* src_data = tmp->data;
    void* dst_data = ctor(1);
    if (!dst_data) {
      list_clear(dst_lst, dtor);
      return -1;
    }
    copy(src_data, dst_data);
    // Add to list
    list_push_back(dst_lst, dst_data);
  }
  return 0;
}
// Returns the number of elements in the range [first, last) satisfying specific criteria.
int list_count(const dlist_t* lst, const void* value, int (*predicate)(const void* data1, const void* data2)) {
  if (!predicate || !lst) return -1;

  int count = 0;
  // Get first
  const dlist_t* cur = list_cfront(lst);
  while (cur) {
    const dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (0 == predicate(tmp->data, value)) count++;
  }
  return count;
}
dlist_t* list_find(dlist_t* lst, const void* value, int (*predicate)(const void* data1, const void* data2)) {
  if (!predicate || !lst) return NULL;

  dlist_t* cur = list_front(lst);
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (0 == predicate(tmp->data, value)) return tmp;
  }
  return NULL;
}
const dlist_t* list_cfind(const dlist_t* lst, const void* value,
                          int (*predicate)(const void* data1, const void* data2)) {
  if (!predicate || !lst) return NULL;

  const dlist_t* cur = list_cfront(lst);
  while (cur) {
    const dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (0 == predicate(tmp->data, value)) return tmp;
  }
  return NULL;
}
// Searches the list for the last occurrence of the sequence specified by its arguments, and returns an pointer to
// element, or null if no occurrences are found.
dlist_t* list_find_end(dlist_t* lst, const void* value, int (*predicate)(const void* data1, const void* data2)) {
  if (!predicate) return NULL;

  dlist_t* cur = list_front(lst);
  dlist_t* last = NULL;
  while (cur) {
    dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (0 == predicate(tmp->data, value)) last = tmp;
  }
  return last;
}
// Searches the list for the last occurrence of the sequence specified by its arguments, and returns an pointer to
// element, or null if no occurrences are found.
const dlist_t* list_cfind_end(const dlist_t* lst, const void* value,
                              int (*predicate)(const void* data1, const void* data2)) {
  if (!predicate) return NULL;

  const dlist_t* cur = list_cfront(lst);
  const dlist_t* last = NULL;
  while (cur) {
    const dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (0 == predicate(tmp->data, value)) last = tmp;
  }
  return last;
}
void list_traverse(const dlist_t* lst, void (*fn)(const dlist_t* lst, void** data), void** data) {
  const dlist_t* cur = list_cfront(lst);
  while (cur) {
    const dlist_t* tmp = cur;
    // Set next
    cur = cur->next;

    if (fn) fn(tmp, data);
  }
}
// Transforms
uint8_t* transform_lst_to_data_x(const dlist_t* lst, uint8_t** data, size_t* data_sz,
                                 uint8_t* (*functor)(uint8_t** data, size_t* data_sz, size_t number_elements, ...),
                                 size_t number_elements, ...) {
  if (!data || !data_sz) return NULL;

  va_list vl;
  va_start(vl, number_elements);
  typedef uint8_t* (*adapter)(void* data_);
  adapter adapters[number_elements];
  memset(adapters, 0, sizeof(adapters));
  // CAUTION: GCC have a Bug 83584 - "ISO C forbids conversion of object pointer to function pointer type" -- no, not
  // really See link: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83584 See standard (ISO/IEC 9899:201x):
  // http://port70.net/~nsz/c/c11/n1570.html#J.5.7p1 J.5.7 [Function pointer casts] 1 A pointer to an object or to void
  // may be cast to a pointer to a function, allowing data to be invoked as a function (6.5.4). 2 A pointer to a
  // function may be cast to a pointer to an object or to void, allowing a function to be inspected or modified (for
  // example, by a debugger) (6.5.4).
  size_t i = 0;
  for (i = 0; i < number_elements; i++) adapters[i] = (adapter)va_arg(vl, int*);

  while (lst) {
    void* val = (void*)lst->data;
    if (functor) {
      for (i = 0; i < number_elements; i++) {
        if (adapters[i]) {
          if (!functor(data, data_sz, 1, adapters[i](val))) {
            va_end(vl);
            return NULL;
          }
        }
      }
    }
    // Set next
    lst = lst->next;
  }
  va_end(vl);

  return *data;
}

const uint8_t* transform_data_to_lst_x(const uint8_t* data, size_t* data_sz, dlist_t** lst, size_t elem_sz,
                                       const uint8_t* (*functor)(const uint8_t* data, size_t* data_sz,
                                                                 size_t number_elements, ...),
                                       size_t number_elements, ...) {
  if (!data || !data_sz) return NULL;

  va_list vl;
  va_start(vl, number_elements);
  typedef uint8_t* (*adapter)(void* data_);
  adapter adapters[number_elements];
  memset(adapters, 0, sizeof(adapters));
  // CAUTION: GCC have a Bug 83584 - "ISO C forbids conversion of object pointer to function pointer type" -- no, not
  // really See link: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83584 See standard (ISO/IEC 9899:201x):
  // http://port70.net/~nsz/c/c11/n1570.html#J.5.7p1 J.5.7 [Function pointer casts] 1 A pointer to an object or to void
  // may be cast to a pointer to a function, allowing data to be invoked as a function (6.5.4). 2 A pointer to a
  // function may be cast to a pointer to an object or to void, allowing a function to be inspected or modified (for
  // example, by a debugger) (6.5.4).
  size_t i = 0;
  for (i = 0; i < number_elements; i++) adapters[i] = (adapter)va_arg(vl, int*);

  while (*data_sz > 0) {
    void* val = calloc(elem_sz, sizeof(uint8_t));
    if (!val) {
      va_end(vl);
      return NULL;
    }

    if (functor) {
      for (i = 0; i < number_elements; i++) {
        if (adapters[i]) {
          data = functor(data, data_sz, 1, adapters[i](val));
          if (!data) {
            // Free memory
            free(val);
            va_end(vl);
            return NULL;
          }
        }
      }
    }
    // Set data
    if (!list_push_back(lst, val)) {
      // Free memory
      free(val);
      va_end(vl);
      return NULL;
    }
  }
  va_end(vl);

  return data;
}
