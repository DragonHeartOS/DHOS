#!/bin/sh

if [ "$(basename $PWD)" != "build" ] ; then
    echo 'Not in build directory. Please go into Build/'
    exit 1
fi

if [ ! -f "kernel/kernel" ] ; then
    echo 'Kernel not built. Please build the kernel.'
    exit 1
fi

if ! command -v parted &>/dev/null; then
    echo 'Parted not found. Please install.'
    exit 1
fi

set -e

echo 'Building limine-install'
make -C ../limine

echo 'Creating disk image'
dd if=/dev/zero bs=1M count=0 seek=64 of=_disk_image
 
echo 'Creating GPT partition table'
parted -s _disk_image mklabel gpt
 
echo 'Creating ESP partition'
parted -s _disk_image mkpart ESP fat32 2048s 100%
parted -s _disk_image set 1 esp on
 
echo 'Installing Limine BIOS stages onto the image'
../limine/limine-deploy _disk_image
 
echo 'Mounting loopback device'
USED_LOOPBACK=$(sudo losetup -Pf --show _disk_image)
 
echo 'Formatting the ESP partition as FAT32'
sudo mkfs.fat -F 32 ${USED_LOOPBACK}p1
 
echo 'Mounting the partition'
mkdir -p img_mount
sudo mount ${USED_LOOPBACK}p1 img_mount
 
echo 'Copying files'
sudo mkdir -p img_mount/EFI/BOOT
sudo cp -v kernel/kernel ../base/limine.cfg ../limine/limine.sys img_mount/
sudo cp -v ../limine/BOOTX64.EFI img_mount/EFI/BOOT/
 
echo 'Syncing system cache and unmounting'
sync
sudo umount img_mount
sudo losetup -d ${USED_LOOPBACK}

rm -rf img_mount
