#ifndef DARRAY_H
#define DARRAY_H

#include "types.h"

#define DARRAY_BASE_CAPACITY 8
#define DARRAY_RESIZE_FACTOR 2

typedef struct darray_header_t {
  u64_t item_size;
  u64_t capacity;
  u64_t length;
} darray_header_t;

void* _darray_create(u64_t item_size, u64_t capacity);
void darray_destroy(void* array);
u64_t darray_length(void* array);
void darray_length_set(void* array, u64_t length);
void darray_clear(void* array);
void* _darray_push(void* array, void* item);
void* darray_pop(void* array);
void* darray_pop_at(void* array, u64_t idx);

#define darray_create(type) _darray_create(sizeof(type), DARRAY_BASE_CAPACITY)

#define darray_push(array, item) \
  { array = _darray_push(array, item); }

#endif
