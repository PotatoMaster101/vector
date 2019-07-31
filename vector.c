///////////////////////////////////////////////////////////////////////////////
// vector.c
// Vector implementation in C99. 
//
// Date:   11/04/2019
// Author: PotatoMaster101
///////////////////////////////////////////////////////////////////////////////

#include "vector.h"
#define DEF_MAX 10

static _Bool vec_fix(vec_t *v);
static inline _Bool vec_check(vec_t *v);
static void *vec_new_elem(const void *d, size_t n);

// Initialises the specified vector. 
//
// PARAMS: 
// v - the vector to initialise
//
// RET: 
// Zero on success, non-zero on error. 
int vec_init(vec_t *v) {
    if (v == NULL)
        return VEC_NULL_ERR;

    int ret = VEC_GOOD;
    v->len = 0;
    v->max = DEF_MAX;
    v->data = malloc(DEF_MAX * sizeof(void *));
    if (v->data == NULL)
        ret = VEC_ALLOC_ERR;
    return ret;
}

// Reserves n elements in the specified vector. 
//
// PARAMS: 
// v - the vector to reserve
// n - number of elements
//
// RET: 
// Zero on success, non-zero on error. 
int vec_reserve(vec_t *v, size_t n) {
    if (v == NULL)
        return VEC_NULL_ERR;
    if (n <= v->max)
        return VEC_RANGE_ERR;

    int ret = VEC_ALLOC_ERR;
    void **res = realloc(v->data, n * v->max * (sizeof *res));
    if (res != NULL) {
        ret = VEC_GOOD;
        v->max = n;
        v->data = res;
    }
    return ret;
}

// Sorts the specified vector using the comparison function. 
//
// PARAMS: 
// v   - the vector to sort
// cmp - the comparison function
//
// RET: 
// Zero on success, non-zero on error. 
int vec_sort(vec_t *v, int (*cmp)(const void *, const void *)) {
    if (!vec_check(v) || cmp == NULL)
        return VEC_NULL_ERR;
    if (v->len != 0)
        qsort(v->data, v->len, sizeof(void *), cmp);
    return VEC_GOOD;
}

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
int vec_add(vec_t *v, const void *d, size_t n) {
    if (!vec_check(v))
        return VEC_NULL_ERR;
    if (!vec_fix(v))
        return VEC_ALLOC_ERR;

    int ret = VEC_ALLOC_ERR;
    void *elem = vec_new_elem(d, n);
    if (elem != NULL) {
        v->data[v->len++] = elem;
        ret = VEC_GOOD;
    }
    return ret;
}

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
int vec_ins(vec_t *v, const void *d, size_t n, size_t i) {
    if (!vec_check(v))
        return VEC_NULL_ERR;
    if (i >= v->len)
        return vec_add(v, d, n);    // add to last if out of range
    if (!vec_fix(v))
        return VEC_ALLOC_ERR;

    int ret = VEC_ALLOC_ERR;
    void *elem = vec_new_elem(d, n);
    if (elem != NULL) {
        size_t sh = v->len - i;
        memmove(v->data + i + 1, v->data + i, sh * sizeof(void *));
        v->data[i] = elem;
        v->len++;
        ret = VEC_GOOD;
    }
    return ret;
}

// Deletes an element in the vector specified by the index. 
//
// PARAMS: 
// v - the vector to delete the element
// i - the index of the element
//
// RET: 
// The deleted element, or NULL on error. 
void *vec_del(vec_t *v, size_t i) {
    if (!vec_check(v))
        return NULL;

    void *ret = NULL;
    if (i >= v->len - 1) {  // remove last if out of range
        ret = v->data[v->len - 1];
    } else {
        ret = v->data[i];
        size_t shift = v->len - i;
        memmove(v->data + i, v->data + i + 1, shift * sizeof(void *));
    }
    v->data[--v->len] = NULL;
    return ret;
}

// Deletes a range of elements in the specified vector. 
//
// PARAMS: 
// v - the vector to delete the elements
// i - the starting index to delete
// n - the number of elements to delete
void vec_delrange(vec_t *v, size_t i, size_t n) {
    if (!vec_check(v) || n == 0)
        return;

    // fix i and n if they are out of range
    i = (i >= v->len) ? (v->len - 1) : i;
    n = (n > v->len - i) ? (v->len - i) : n;

    size_t left = v->len - (i + n);
    for (size_t j = 0; j < n; j++)
        free(v->data[i + j]);
    if (left != 0)
        memmove(v->data + i, v->data + i + n, left * sizeof(void *));
    v->len -= n;
}

// Reverts every element in the vector. 
//
// PARAMS: 
// v - the vector to reverse. 
void vec_reverse(vec_t *v) {
    if (vec_check(v)) {
        for (size_t i = 0; i < (v->len / 2); i++) {
            void *swap = v->data[i];
            v->data[i] = v->data[v->len - i - 1];
            v->data[v->len - i - 1] = swap;
        }
    }
}

// Clears the specified vector, freeing all elements. 
//
// PARAMS: 
// v - the vector to clear
void vec_clear(vec_t *v) {
    if (vec_check(v)) {
        for (size_t i = 0; i < v->len; i++)
            free(v->data[i]);
        v->len = 0;
    }
}

// Frees the internal buffer in the specified vector. 
//
// PARAMS: 
// v - the vector to free
void vec_free(vec_t *v) {
    if (vec_check(v)) {
        for (size_t i = 0; i < v->len; i++)
            free(v->data[i]);
        free(v->data);
        v->data = NULL;
    }
}

// Fixes the buffer of the vector, reallocates if needed. 
//
// PARAMS: 
// v - the vector to fix
//
// RET: 
// True if the vector has enough storage for one more element, false 
// otherwise. 
static _Bool vec_fix(vec_t *v) {
    if (v->len < v->max)
        return true;    // space enough

    printf("fixing...\n");
    _Bool ret = false;
    void **temp = realloc(v->data, 2 * v->max * (sizeof *temp));
    if (temp != NULL) {
        ret = true;
        v->max *= 2;
        v->data = temp;
    }
    return ret;
}

// Checks whether the vector is valid. 
//
// PARAMS: 
// v - the vector to check
//
// RET: 
// True if the vector is in valid state, false otherwise. 
static inline _Bool vec_check(vec_t *v) {
    return (v != NULL && v->data != NULL);
}

// Returns a copy of the specified element. 
//
// PARAMS: 
// d - the element to copy
// n - the size of the element
static void *vec_new_elem(const void *d, size_t n) {
    if (d == NULL || n == 0)
        return NULL;

    void *ret = malloc(n);
    if (ret != NULL)
        memcpy(ret, d, n);
    return ret;
}

