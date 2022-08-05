ARCH ?= x86_64
KERN_FILE = kernel.sys

# Check that the architecture is supported and set relevant variables.
ifeq ($(ARCH),x86_64)
    override EFI_ARCH := X64
else
    $(error Architecture $(ARCH) not supported)
endif

.PHONY: all
all: DHOS.iso

.PHONY: run
run: DHOS.iso
	qemu-system-$(ARCH) -M q35 -m 2G -drive file=DHOS.iso -boot d -monitor stdio --enable-kvm -serial telnet:localhost:4321,server,nowait

.PHONY: debug1
debug1:
	qemu-system-$(ARCH) -M q35 -m 2G -drive file=DHOS.iso -boot d -monitor stdio -d int -D logfile.txt -s -S

setup:
	git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1

.PHONY: kernel
kernel:
	cd kernel; bash buildall

DHOS.iso: kernel
	make -C limine
	mkdir -p base/internals/
	rm -rf iso_root
	mkdir -p iso_root
	mkdir -p iso_root/DHOS
	cp base/limine.cfg \
		limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	cp kernel/$(KERN_FILE) base/internals/* iso_root/DHOS/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o DHOS.iso
	limine/limine-deploy DHOS.iso
	@ rm -rf iso_root
	bash kernel/clean

.PHONY: clean
clean:
	rm -rf iso_root DHOS.iso DHOS.hdd
	bash kernel/clean

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-$(EFI_ARCH)
	$(MAKE) -C kernel distclean
