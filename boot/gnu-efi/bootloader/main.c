#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>

#define DRAGONBOOT_VERBOSE
#define DEFAULT_CONFIG_PATH L"dragonboot.cfg"


// Config stuff.
UINT8 boot_panic = 0;


// Memory map structure.
struct MemoryMap {
    EFI_MEMORY_DESCRIPTOR* map;     // Actual memory map.
    UINTN size;                     // Size of memory map.
    UINTN descriptor_size;          // Size of each descriptor.
    UINTN map_key;
    UINT32 descriptor_version;
};


/*
 *  Dragon protocol:
 *
 *  Gives information & services to the kernel.
 *
 *
 */

struct DragonProtocol {
    struct MemoryMap* mmap;

    struct {
        void* base;
        UINT64 size;
        UINT64 horizontal_resolution;
        UINT64 vertical_resolution;
        UINT64 pixels_per_scanline;
        UINT64 pitch;
    } framebuffer;
} dproto;


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
 *                Storage.                *
 ******************************************
 */

EFI_FILE_HANDLE get_volume(EFI_HANDLE imageHandle) {
    EFI_LOADED_IMAGE* loaded_image = NULL;
    EFI_GUID lipGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_FILE_IO_INTERFACE* IOvolume;
    EFI_GUID fsguid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_FILE_HANDLE volume;

    // Get loaded image protocol interface.
    uefi_call_wrapper(BS->HandleProtocol, 3, imageHandle, &lipGUID, (void**)&loaded_image);
    
    // Get volume handle.
    uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &fsguid, (VOID*)&IOvolume);

    // Open volume.
    uefi_call_wrapper(IOvolume->OpenVolume, 2, IOvolume, &volume);
    return volume;
}


UINT64 get_file_sz(EFI_FILE_HANDLE fhandle) {
    UINT64 tmp;
    EFI_FILE_INFO* finfo = LibFileInfo(fhandle);
    tmp = finfo->FileSize;
    FreePool(finfo);
    return tmp;
}


/*
 ******************************************
 *               String tools.            *
 ******************************************
 */


UINT64 strlen(UINT8* str) {
    UINT64 i = 0;
    while (str[i++]);
    return i - 1;
}

UINT8 strcmp(UINT8* str1, UINT8* str2) {
    if (strlen(str1) != strlen(str2)) {
        return 1;
    }

    for (UINT64 i = 0; i < strlen(str1); ++i) {
        if (str1[i] != str2[i]) {
            return 1;
        }
    }

    return 0;
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
            break;
        }
    }

    if (EFI_ERROR(s)) {
        Print(L"FATAL: Failed to fetch memory map! %d\n", s);
        panic();
    }

#ifdef DRAGONBOOT_VERBOSE
    for (size_t i = 0; i < out->size / out->descriptor_size; ++i) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)out->map + (i * out->descriptor_size)); 
        Print(L"[phys - virt]: %x - %x (%d KB)\n", desc->PhysicalStart, desc->VirtualStart, desc->NumberOfPages * 4096 / 1024);
    }
#endif

    return s;
}

/*
 ******************************************
 *        Graphics Output Protocol        *
 ******************************************
 */


EFI_STATUS init_gop(void) {
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS s = uefi_call_wrapper(BS->LocateProtocol, 3, &gop_guid, NULL, (void**)&gop);

    // Verify everthing went alright while 
    // locating GOP.
    if (EFI_ERROR(s)) {
        // Sadly, no.
        Print(L"FATAL: Failed to locate Graphics Output Protocol!\n");
        panic();
    }

    // Now we want to fetch information from GOP.
    dproto.framebuffer.base = (void*)gop->Mode->FrameBufferBase;
    dproto.framebuffer.size = gop->Mode->FrameBufferSize;
    dproto.framebuffer.horizontal_resolution = gop->Mode->Info->HorizontalResolution;
    dproto.framebuffer.vertical_resolution = gop->Mode->Info->VerticalResolution;
    dproto.framebuffer.pixels_per_scanline = gop->Mode->Info->PixelsPerScanLine;
    return s;
}


// Entry point.
EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
    InitializeLib(imageHandle, sysTable);
    Print(L"Coming soon!\n");

    // Fetch memory map.
    struct MemoryMap mmap;
    init_mmap(&mmap);

    // Init GOP.
    init_gop();

    __asm__ __volatile__("cli; hlt");
    return EFI_SUCCESS;
}
