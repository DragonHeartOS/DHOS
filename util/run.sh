#!/usr/bin/env bash

SYSTEM='QEMU'

if [ "$1" = 'Bochs' ] ; then
    echo 'Bochs not yet implemented!'
    exit 1
fi

BOOT_MODE='DISK'
if [ -f 'dhos.iso' ] ; then
    BOOT_MODE='CDROM'
fi

if [ "$SYSTEM" = 'QEMU' ] ; then
    QEMU_BIN='qemu-system-x86_64'
    QEMU_ARGS="$QEMU_ARGS -m 1G -serial stdio"
    [ "${DEBUG}" == 1 ] && QEMU_ARGS="$QEMU_ARGS -s -S" || QEMU_ARGS="$QEMU_ARGS -enable-kvm"
    if [ "$BOOT_MODE" = 'DISK' ] ; then
        QEMU_ARGS="$QEMU_ARGS -drive format=raw,file=_disk_image"
    elif [ "$BOOT_MODE" = 'CDROM' ] ; then
        QEMU_ARGS="$QEMU_ARGS -drive format=raw,media=cdrom,readonly=on,file=dhos.iso"
    else
        echo "Unknown boot mode: $BOOT_MODE"
        exit 1
    fi

    $QEMU_BIN $QEMU_ARGS
fi
