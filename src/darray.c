#include "darray.h"
#include <stdlib.h>
#include <string.h>

void* _darray_create(u64_t item_size, u64_t capacity) {
  void* darray = malloc(sizeof(darray_header_t) + item_size * capacity);
  memset(darray, 0, sizeof(darray_header_t) + item_size * capacity);
  darray_header_t* header = darray;
  header->item_size = item_size;
  header->capacity = capacity;
  header->length = 0;

  return (u8_t*)darray + sizeof(darray_header_t);
}

void darray_destroy(void* array) {
  if (array) {
    free((void*)((u8_t*)array - sizeof(darray_header_t)));
  }
}

u64_t darray_length(void* array) {
  darray_header_t* header = (darray_header_t*)((u8_t*)array - sizeof(darray_header_t));
  return header->length;
}

void darray_length_set(void* array, u64_t length) {
  darray_header_t* header = (darray_header_t*)((u8_t*)array - sizeof(darray_header_t));
  header->length = length;
}

u64_t darray_isize(void* array) {
  darray_header_t* header = (darray_header_t*)((u8_t*)array - sizeof(darray_header_t));
  return header->item_size;
}

void* darray_resize(void* array) {
  darray_header_t* header = (darray_header_t*)((u8_t*)array - sizeof(darray_header_t));

  void* new_array = _darray_create(header->item_size, header->capacity * DARRAY_RESIZE_FACTOR);
  memcpy(new_array, array, header->item_size * header->length);
  darray_length_set(new_array, header->length);
  darray_destroy(array);
  return new_array;
}

void darray_clear(void* array) { darray_length_set(array, 0); }

void* _darray_push(void* array, void* item) {
  darray_header_t* header = (darray_header_t*)((u8_t*)array - sizeof(darray_header_t));

  if (header->length >= header->capacity) {
    array = darray_resize(array);
    header = (darray_header_t*)((u8_t*)array - sizeof(darray_header_t));
  }

  memcpy((u8_t*)array + header->item_size * header->length, item, header->item_size);
  darray_length_set(array, header->length + 1);
  return array;
}

void* darray_pop(void* array) {
  u64_t len = darray_length(array);
  if (len == 0) {
    return array;
  }

  darray_length_set(array, len - 1);
  return array;
}

void* darray_pop_at(void* array, u64_t idx) {
  u64_t len = darray_length(array);
  u64_t isize = darray_isize(array);
  if (len == 0 || idx >= len) {
    return array;
  }

  void* ptr = array + idx * isize;
  memmove(ptr, ptr + isize, (len - idx - 1) * isize);

  darray_length_set(array, len - 1);
  return array;
}
