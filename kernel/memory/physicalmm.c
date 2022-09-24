#include <kernel/memory/physicalmm.h>

#include <kernel/kernel.h>
#include <kernel/kpanic.h>

#include <kernel/limine.h>

#include <commonlib/bitmap.h>

#define HIGHER_HALF_DATA 0xFFFF800000000000UL

static struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

static struct limine_memmap_entry *entry = NULL;

static bitmap_t bitmap;

u64
physicalmm_alloc(void)
{
  u64 idx=0;
  while (bitmap_get(&bitmap, idx) == true) {
    if (idx >= bitmap.size)
      return 0;
    idx++;
  }

  return 0x1000*idx;
}

void
physicalmm_free(u64 addr)
{
  u64 idx = addr/0x1000;
  bitmap_set(&bitmap, idx, false);
}

void
physicalmm_init(void)
{
  for (u64 i = 0; i < memmap_request.response->entry_count; i++) {
    struct limine_memmap_entry *current = memmap_request.response->entries[i];

    if (current->type == LIMINE_MEMMAP_USABLE) {
      if (entry == NULL || entry->length < current->length) {
        entry = current;
      }
    }
  }

  if (entry == NULL) {
    kpanic("PHYSICAL MEMORY MANAGER: Not enough memory available!");
  }

  bitmap.buffer = (u8 *)HIGHER_HALF_DATA + entry->base;
  bitmap.size = entry->length;
}
