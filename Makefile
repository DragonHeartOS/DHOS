img:
	cp boot/gnu-efi/x86_64/bootloader/main.efi ./BOOTX64.EFI
	cd boot; make
	dd if=/dev/zero of=DHOS.iso bs=1k count=1440
	mformat -i DHOS.iso -f 1440 ::
	mmd -i DHOS.iso ::/EFI
	mmd -i DHOS.iso ::/EFI/BOOT
	mcopy -i DHOS.iso BOOTX64.EFI ::/EFI/BOOT
	mcopy -i DHOS.iso boot/startup.nsh ::
	rm *.EFI

run:
	qemu-system-x86_64 -cdrom DHOS.iso -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file=boot/OVMFbin/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=boot/OVMFbin/OVMF_VARS-pure-efi.fd
