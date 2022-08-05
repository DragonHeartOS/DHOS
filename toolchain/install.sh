#!/bin/bash

VERBOSE=0

if [ "$1" = "verbose" ]; then
	VERBOSE=1
fi

set -e

BINUTILS_VER="2.36"
BINUTILS_MD5="4009acf0f62bab6696bc87b3370953fa binutils-$BINUTILS_VER.tar.gz"
GCC_VER="11.1.0"
GCC_MD5="333068a65c119e74c9d7bfcc75a8eeba gcc-11.1.0.tar.gz"

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
mkdir -p $SCRIPT_DIR/build
PREFIX=$SCRIPT_DIR/build

export PATH="$PREFIX/bin:$PATH"

if [ "$(nproc)" -eq 1 ]; then
	echo "It seems you only have one core, please upgrade your CPU with a modern one and try again."
	exit 1
fi

CORES=$(($(nproc) - 1))

clean_binutils() {
	rm -rf build-binutils
	rm -rf build/{x86_64-elf,}bin/{ar,as,ld,ld.bfd,nm,objcopy,objdump,ranlib,strip}
	rm -rf build/{x86_64-elf/,}lib/bfd-plugins
	rm -rf build/lib/bfd-plugins
	rm -rf build/share/info/{as.info,bfd.info,binutils.info,dir,gprof.info,ld.info}
	rm -rf build/share/man/x86_64-elf-{addr2line,ar,as,c++filt,dlltool,elfedit,gprof,ld,nm,objcopy,objdump,ranlib,readelf,size,strings,strip,windmc,windres}.1
	rm -rf build/x86_64-elf/lib/ldscripts
}

binutils_tries=0
get_binutils() {
	clean_binutils

	binutils_tries=$(($binutils_tries + 1))
	if ! [ -e binutils-$BINUTILS_VER.tar.gz ] || [ "$1" = "redownload" ] ; then
		rm -rf binutils-$BINUTILS_VER.tar.gz
		echo " :: Downloading binutils source code"
		wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.gz
	fi
	echo " :: Verifying file integrity"
	if md5sum --status -c <(echo "${BINUTILS_MD5}"); then
		echo " :: Binutils source code integrity OK"
		echo " :: Extracting source code"
		[ "$VERBOSE" = "1" ] && tar xvf binutils-$BINUTILS_VER.tar.gz || tar xf binutils-$BINUTILS_VER.tar.gz
		mkdir -p $SCRIPT_DIR/build-binutils
		cd $SCRIPT_DIR/build-binutils
		echo " :: Configuring"
		[ "$VERBOSE" = "1" ] && ../binutils-$BINUTILS_VER/configure --target=x86_64-elf --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror || \
		../binutils-$BINUTILS_VER/configure --target=x86_64-elf --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror > /dev/null
		echo " :: Compiling"
		[ "$VERBOSE" = "1" ] && make -j $CORES ||
		make -j $CORES > /dev/null
		echo " :: Installing"
		[ "$VERBOSE" = "1" ] && make install || make install > /dev/null
		cd $SCRIPT_DIR
	else
		echo " :: Binutils source code integrity not OK"
		if [ "$binutils_tries" -lt "4" ]; then
			echo " :: Trying again... Tries: $binutils_tries"
			get_binutils redownload
		else
			echo " !! Failed to download binutils. Aborting!"
			exit 1
		fi

	fi
}

clean_gcc() {
	rm -rf build/lib/gcc
	rm -rf build/bin/x86_64-elf-{lto-dump,g++,c++,gcc,cpp,gcov,gcov-tool,gcov-dump}
	rm -rf build/bin/x86_64-elf-gcc*
	rm -rf build/share/info/{gcc,cppinternals,gccinstall,cpp,gccint}.info
	rm -rf build/share/info/dir
	rm -rf build/share/man/man1/x86_64-elf-{g++,gcc,cpp,gcov,gcov-tool,gcov-dump,lto-dump}.1
	rm -rf build/share/man/man7
}

gcc_tries=0
get_gcc() {
	gcc_tries=$(($gcc_tries + 1))
	if ! [ -e gcc-$GCC_VER.tar.gz ] || [ "$1" = "redownload" ] ; then
		rm -rf gcc-$GCC_VER.tar.gz
		echo " :: Downloading GCC source code"
		wget https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.gz
	fi
	echo " :: Verifying file integrity"
	if md5sum --status -c <(echo "${GCC_MD5}"); then
		echo " :: GCC source code integrity OK"
		echo " :: Extracting source code"
		[ "$VERBOSE" = "1" ] && tar xvf gcc-$GCC_VER.tar.gz || tar xf gcc-$GCC_VER.tar.gz
		echo " :: Configuring"
		mkdir -p $SCRIPT_DIR/build-gcc
		cd $SCRIPT_DIR/build-gcc
		[ "$VERBOSE" = "1" ] && ../gcc-$GCC_VER/configure --target=x86_64-elf --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers --with-newlib || \
		../gcc-$GCC_VER/configure --target=x86_64-elf --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers --with-newlib > /dev/null
		echo " :: Compiling"
		[ "$VERBOSE" = "1" ] && make all-gcc -j $CORES ||
		make all-gcc -j $CORES > /dev/null
		[ "$VERBOSE" = "1" ] && make all-target-libgcc -j $CORES ||
		make all-target-libgcc -j $CORES > /dev/null
		echo " :: Installing"
		[ "$VERBOSE" = "1" ] && make install-gcc -j $CORES ||
		make install-gcc > /dev/null
		[ "$VERBOSE" = "1" ] && make install-target-libgcc -j $CORES ||
		make install-target-libgcc > /dev/null
		cd $SCRIPT_DIR
	else
		echo " :: GCC source code integrity not OK"
		if [ "$gcc_tries" -lt "4" ]; then
			echo " :: Trying again... Tries: $gcc_tries"
			get_gcc redownload
		else
			echo " !! Failed to download GCC. Aborting!"
			exit 1
		fi

	fi
}

BINUTILS_REINSTALL=0
GCC_REINSTALL=0

CLEAN_BINUTILS=0
CLEAN_GCC=0

for arg in "$@"; do
	if [ "$arg" = "--binutils-reinstall" ]; then
		BINUTILS_REINSTALL=1
	elif [ "$arg" = "--gcc-reinstall" ]; then
		GCC_REINSTALL=1
	elif [ "$arg" = "--clean-binutils" ]; then
		CLEAN_BINUTILS=1
	elif [ "$arg" = "--clean-gcc" ]; then
		CLEAN_GCC=1
	elif [ "$arg" = "--help" ] || [ "$arg" = "-h" ]; then
		echo "Usage: $0 [--binutils-reinstall] [--gcc-reinstall] [--clean-binutils] [--clean-gcc]"
		exit 0
	fi
done	

if [ "$BINUTILS_REINSTALL" = "1" ]; then
	echo " :: Reinstalling binutils"
	get_binutils
elif [ "$GCC_REINSTALL" = "1" ]; then
	echo " :: Reinstalling GCC"
	get_gcc
elif [ "$CLEAN_BINUTILS" = "1" ]; then
	echo " :: Cleaning binutils"
	clean_binutils
elif [ "$CLEAN_GCC" = "1" ]; then
	echo " :: Cleaning GCC"
	clean_gcc
else
	[ -f "build/bin/x86_64-elf-ld" ] && echo " :: Binutils already installed" || get_binutils
	[ -f "build/bin/x86_64-elf-gcc" ] && echo " :: GCC already installed" || get_gcc
fi

echo " :: Done!"
