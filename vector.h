///////////////////////////////////////////////////////////////////////////////
// vector.h
// Vector implementation in C99. 
//
// Date:   11/04/2019
// Author: PotatoMaster101
///////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H
#define VECTOR_H
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define VEC_GOOD 0
#define VEC_ALLOC_ERR 1
#define VEC_NULL_ERR 2
#define VEC_RANGE_ERR 3

// The vector type. 
typedef struct vec_t {
    void **data;            // internal data
    size_t len;             // current length
    size_t max;             // maximum length
} vec;

// Initialises the specified vector. 
//
// PARAMS: 
// v - the vector to initialise
//
// RET: 
// Zero on success, non-zero on error. 
int vec_init(vec *v);

// Sorts the specified vector using the comparison function. 
//
// PARAMS: 
// v   - the vector to sort
// cmp - the comparison function
//
// RET: 
// Zero on success, non-zero on error. 
int vec_sort(vec *v, int (*cmp)(const void *, const void *));

// Adds a new element into the specified vector. The new element will be 
// copied. 
//
// PARAMS: 
// v - the vector to add the element
// d - the element to add
// n - the size of the element
//
// RET: 
// Zero on success, non-zero on error. 
int vec_add(vec *v, const void *d, size_t n);

// Inserts a new element into the specified vector. The new element will be 
// copied. 
//
// PARAMS: 
// v - the vector to insert the element
// d - the element to insert
// n - the size of the element
// i - the index to insert to
//
// RET: 
// Zero on success, non-zero on error. 
int vec_ins(vec *v, const void *d, size_t n, size_t i);

// Deletes an element in the vector specified by the index. 
//
// PARAMS: 
// v - the vector to delete the element
// i - the index of the element
//
// RET: 
// The deleted element, or NULL on error. 
void *vec_del(vec *v, size_t i);

// Deletes a range of elements in the specified vector. 
//
// PARAMS: 
// v - the vector to delete the elements
// i - the starting index to delete
// n - the number of elements to delete
void vec_delrange(vec *v, size_t i, size_t n);

// Reverts every element in the vector. 
//
// PARAMS: 
// v - the vector to reverse. 
void vec_reverse(vec *v);

// Clears the specified vector, freeing all elements. 
//
// PARAMS: 
// v - the vector to clear
void vec_clear(vec *v);

// Frees the internal buffer in the specified vector. 
//
// PARAMS: 
// v - the vector to free
void vec_free(vec *v);

#endif

