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
#ifndef _LIST_H
#define _LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct dlist_t {
  void* data;
  struct dlist_t* next;
  struct dlist_t* prev;
} dlist_t;

////////////////////////////////////////////////////////////////////////////
// List
////////////////////////////////////////////////////////////////////////////
// Add element at the end. Adds a new element at the end of the list container, after its current last element. The
// content of val is copied (or moved) to the new element.
dlist_t* list_push_back(dlist_t** lst, void* data);
// Adds a new element at the end of the list container, if the element already exists elsewhere in the list, it updates
// the element without adding new.
int list_push_back_unique(dlist_t** lst, const void* value, void* src,
                          int (*predicate)(const void* data1, const void* data2),
                          void (*copy)(const void* src_data, void* dst_data), void* (*ctor_copy)(const void* src_data));
// Removes from the container all the elements for which predicate 'pred' returns 0. This calls the destructor 'deleter'
// of these objects and reduces the container size by the number of elements removed.
void list_remove_if(dlist_t** lst, const void* value, int (*predicate)(const void* data1, const void* data2),
                    void (*deleter)(void* data));
// Removes all elements from the list container (which are destroyed), and leaving the container with a size of 0.
void list_clear(dlist_t** lst, void (*deleter)(void* data));
// Returns a const pointer to the first element in the list container.
const dlist_t* list_cfront(const dlist_t* lst);
// Access first element. Returns a pointer to the first element in the list container.
dlist_t* list_front(dlist_t* lst);
// Access last element. Returns a pointer to the last element in the list container.
dlist_t* list_back(dlist_t* lst);
// Delete first element. Removes the first element in the list container, effectively reducing its size by one.
void list_pop_front(dlist_t** lst, void (*deleter)(void* data));
// Delete last element. Removes the last element in the list container, effectively reducing the container size by one.
void list_pop_back(dlist_t** lst, void (*deleter)(void* data));
// Test whether container is empty. Returns whether the list container is empty (i.e. whether its size is 0).
int list_empty(const dlist_t* lst);
// Return size. Returns the number of elements in the list container.
int list_size(const dlist_t* lst);
////////////////////////////////////////////////////////////////////////////
// Algorithms
////////////////////////////////////////////////////////////////////////////
// Applies function fn to each of the elements in the range [first,last). Unary function that accepts an element in the
// range as argument. Return: Its return value, if any, is ignored.
int list_for_each(dlist_t* lst, int (*fn)(void* data));
int list_for_each_extra(dlist_t* lst, int (*fn)(void* data), int count, ...);
// Test if any element in range fulfills condition. Unary function that accepts an element in the range as argument and
// returns a value convertible to bool. Returns true if pred returns true for any of the elements in the range
// [first,last), and false otherwise. If [first,last) is an empty range, the function returns false.
int list_any_of(dlist_t* lst, int (*fn)(void* data));
int list_any_of_extra(dlist_t* lst, int (*fn)(void* data), int count, ...);
int list_cany_of_extra(const dlist_t* lst, int (*fn)(void* data), int count, ...);
// Test condition on all elements in range. Unary function that accepts an element in the range as argument and returns
// a value convertible to bool. Returns true if pred returns true for all the elements in the range [first,last) or if
// the range is empty, and false otherwise.
int list_all_of(dlist_t* lst, int (*fn)(void* data));
int list_all_of_extra(dlist_t* lst, int (*fn)(void* data), int count, ...);
// Test if no elements fulfill condition. Unary function that accepts an element in the range as argument and returns a
// value convertible to bool. Returns true if pred returns false for all the elements in the range [first,last) or if
// the range is empty, and false otherwise.
int list_none_of(dlist_t* lst, int (*fn)(void* data));
int list_none_of_extra(dlist_t* lst, int (*fn)(void* data), int count, ...);
// Returns the number of hops from first to last.
int list_distance(const dlist_t* first1, const dlist_t* last1);
// Compares the elements in the range [first1,last1) with those in the range beginning at first2, and returns true if
// all of the elements in both ranges match, even in a different order.
int list_is_permutation(const dlist_t* first1, const dlist_t* first2,
                        int (*predicate)(const void* data1, const void* data2));
// Assign content. Assigns new contents to the container, replacing its current contents, and modifying its size
// accordingly.
int list_copy(const dlist_t* src_lst, dlist_t** dst_lst, void* (*ctor)(size_t count), void (*dtor)(void* data),
              void (*copy)(const void* src_data, void* dst_data));
// Returns the number of elements in the range [first, last) satisfying specific criteria.
int list_count(const dlist_t* lst, const void* value, int (*predicate)(const void* data1, const void* data2));
// Searches the list for the first occurrence of the sequence specified by its arguments.
dlist_t* list_find(dlist_t* lst, const void* value, int (*predicate)(const void* data1, const void* data2));
// Searches the list for the first occurrence of the sequence specified by its arguments.
const dlist_t* list_cfind(const dlist_t* lst, const void* value,
                          int (*predicate)(const void* data1, const void* data2));
// Searches the list for the last occurrence of the sequence specified by its arguments, and returns an pointer to
// element, or null if no occurrences are found.
dlist_t* list_find_end(dlist_t* lst, const void* value, int (*predicate)(const void* data1, const void* data2));
// Searches the list for the last occurrence of the sequence specified by its arguments, and returns an pointer to
// element, or null if no occurrences are found.
const dlist_t* list_cfind_end(const dlist_t* lst, const void* value,
                              int (*predicate)(const void* data1, const void* data2));
void list_traverse(const dlist_t* lst, void (*fn)(const dlist_t* lst, void** data), void** data);
// list subtract
// void list_subtract(dlist_t** lst, dlist_t*, int (*predicate)(const void* data1, const void* data2), void
// (*deleter)(void* data));

// Transforms
uint8_t* transform_lst_to_data_x(const dlist_t* lst, uint8_t** data, size_t* data_sz,
                                 uint8_t* (*functor)(uint8_t** data, size_t* data_sz, size_t number_elements, ...),
                                 size_t number_elements, ...);
const uint8_t* transform_data_to_lst_x(const uint8_t* data, size_t* data_sz, dlist_t** lst, size_t elem_sz,
                                       const uint8_t* (*functor)(const uint8_t* data, size_t* data_sz,
                                                                 size_t number_elements, ...),
                                       size_t number_elements, ...);

#ifdef __cplusplus
}
#endif

#endif  //_LIST_H
