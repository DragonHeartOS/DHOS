#include <efi.h>
#include <efilib.h>
#include <stddef.h>

// Memory map structure.
struct MemoryMap {
    EFI_MEMORY_DESCRIPTOR* map;     // Actual memory map.
    UINTN size;                     // Size of memory map.
    UINTN descriptor_size;          // Size of each descriptor.
    UINTN map_key;
    UINT32 descriptor_version;
};


/*
 ******************************************
 *                 Panic.                 *
 ******************************************
 */

void panic(void) {
    Print(L"\n** DRAGONBOOT PANIC **\n");
    __asm__ __volatile__("cli; hlt");
}


/*
 ******************************************
 *          Memory map management.        *
 ******************************************
 */


EFI_STATUS init_mmap(struct MemoryMap* out) {
    out->size = 0;
    out->map = NULL;
    EFI_STATUS s = uefi_call_wrapper(BS->GetMemoryMap, 5, &out->size, out->map, &out->map_key, &out->descriptor_size, &out->descriptor_version);

    while (s != 0) {
        out->map = AllocatePool(out->size);
        s = uefi_call_wrapper(BS->GetMemoryMap, 5, &out->size, out->map, &out->map_key, &out->descriptor_size, &out->descriptor_version);

        if (s == EFI_BUFFER_TOO_SMALL) {
            FreePool(out->map);
            out->size *= 2;
        } else {
            Print(L"%d\n", s);
            break;
        }
    }

    if (EFI_ERROR(s)) {
        Print(L"FATAL: Failed to fetch memory map! %d\n", s);
        panic();
    }

    for (size_t i = 0; i < out->size / out->descriptor_size; ++i) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)out->map + (i * out->descriptor_size)); 
        Print(L"[phys - virt]: %x - %x (%d KB)\n", desc->PhysicalStart, desc->VirtualStart, desc->NumberOfPages * 4096 / 1024);
    }

    return s;
}


// Entry point.
EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
    InitializeLib(imageHandle, sysTable);
    Print(L"Coming soon!\n");

    // Fetch memory map.
    struct MemoryMap mmap;
    init_mmap(&mmap);

    __asm__ __volatile__("cli; hlt");
    return EFI_SUCCESS;
}
