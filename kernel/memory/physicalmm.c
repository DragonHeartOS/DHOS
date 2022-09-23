#include <kernel/memory/physicalmm.h>

#include <kernel/kernel.h>
#include <kernel/kpanic.h>

#include <kernel/limine.h>

static struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

static struct limine_memmap_entry *entry = NULL;

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
}
