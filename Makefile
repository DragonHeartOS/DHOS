BOOTLOADER_FILES = $(shell find boot/src/ -name "*.c")
CFLAGS = -fpic  						\
		-ffreestanding 					\
		-fno-stack-protector 			\
		-fno-stack-check 				\
		-fshort-wchar 					\
		-mno-red-zone 					\
		-maccumulate-outgoing-args

LDBOOTFLAGS = -shared \
			  -Bsymbolic \
			  -Lboot/gnu-efi/x86_64/gnuefi \
			  -Tboot/gnu-efi/gnuefi/elf_x86_64_efi.lds


CC = x86_64-elf-gcc
LD = x86_64-elf-ld

main.efi: bootloader
	$(LD) $(LDBOOTFLAGS) boot/gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o obj/*.o -o main.so -lgnuefi
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 main.so main.efi
	rm -rf obj/ *.so

.PHONY: bootloader
bootloader: $(BOOTLOADER_FILES)
	mkdir -p obj/
	cd boot/gnu-efi; make
	$(CC) -I boot/gnu-efi/inc $(CFLAGS) -c $^ -o obj/$(notdir $^).o
