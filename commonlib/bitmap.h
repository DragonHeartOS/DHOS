#ifndef BITMAP_H
#define BITMAP_H

#include <commonlib/types.h>

typedef struct {
  u64 size;
  u8 *buffer;
} bitmap_t;

void
bitmap_set(bitmap_t *bitmap, u64 index, u8 value)
{
  u64 byte_index = index / 8;
  u8 bit_index = index % 8,
     bit_indexer = 0b10000000 >> bit_index;

  bitmap->buffer[byte_index] &= ~bit_indexer;
  if (value == true)
    bitmap->buffer[byte_index] |= bit_indexer;
}

u8
bitmap_get(bitmap_t *bitmap, u64 index)
{
  u64 byte_index = index / 8;
  u8 bit_index = index % 8,
     bit_indexer = 0b10000000 >> bit_index;

  return (bitmap->buffer[byte_index] & bit_indexer) > 0;
}

#endif // BITMAP_H
