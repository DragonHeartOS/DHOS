#include <efi.h>
#include <efilib.h>


EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* sysTable) {
	InitializeLib(imageHandle, sysTable);
	Print(L"Hello, World!\n");
	return EFI_SUCCESS;
}
