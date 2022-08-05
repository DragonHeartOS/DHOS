iso: img
	mkdir -p iso
	cp DHOS.img iso/
	xorriso -as mkisofs -R -f -e DHOS.img -no-emul-boot -o DHOS.iso iso
	rm -f *.img
	rm -rf obj/
	

img:
	cd boot; make
	dd if=/dev/zero of=DHOS.img bs=1k count=1440
	mformat -i DHOS.img -f 1440 ::
	mmd -i DHOS.img ::/EFI
	mmd -i DHOS.img ::/EFI/BOOT
	mcopy -i DHOS.img boot/gnu-efi/x86_64/bootloader/main.efi ::/EFI/BOOT
	mcopy -i DHOS.img boot/startup.nsh ::

run:
	qemu-system-x86_64 -cdrom DHOS.iso -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file=boot/OVMFbin/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=boot/OVMFbin/OVMF_VARS-pure-efi.fd
