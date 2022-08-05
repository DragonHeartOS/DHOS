#include <efi.h>
#include <efilib.h>

// Entry point.
EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
    InitializeLib(imageHandle, sysTable);
	Print(L"Coming soon!");
    __asm__ __volatile__("cli; hlt");
    return 0;
}
