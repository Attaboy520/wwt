#! /usr/bin/bash

sudo apt update && \
    sudo apt install -y \
	build-essential \
	bison \
	flex \
	libgmp3-dev \
	libmpc-dev \
	libmpfr-dev \
	texinfo \
	qemu-system-x86 \
	make \
	mtools

BINUTIL_VERSION=2.37
BINUTIL_URL=https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.xz

GCC_VERSION=11.2.0
GCC_URL=https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.xz

GCC_SRC="gcc-${GCC_VERSION}"
BINUTIL_SRC="binutils-${BINUTIL_VERSION}"

# download gcc & binutil src code

#export TARGET=i686-elf
export TARGET=x86_64-elf
export TOOLS_DIR=$HOME/cross-compiler-tools
export PREFIX="$HOME/cross-compiler/$TARGET"
export PATH="$PREFIX/bin:$PATH"

echo "Building gnu-efi"

(make -C $(pwd)/gnu-efi && sudo make install -C $(pwd)/gnu-efi) || exit

mkdir -p "${PREFIX}"
mkdir -p "$TOOLS_DIR/binutils-build"
mkdir -p "$TOOLS_DIR/gcc-build"

cd "$TOOLS_DIR"

if [ ! -d "$TOOLS_DIR/${GCC_SRC}" ]
then
	if [ -e ${GCC_SRC}.tar ]
	then
		tar -xf "${GCC_SRC}.tar" || exit
	else
		(wget -O "${GCC_SRC}.tar" ${GCC_URL} \
			&& tar -xf "${GCC_SRC}.tar") || exit
		rm -f "${GCC_SRC}.tar"
	fi
else
	echo "skip downloading gcc"
fi

if [ ! -d "$TOOLS_DIR/${BINUTIL_SRC}" ]
then
	if [ -e ${BINUTIL_SRC}.tar ]
	then
		tar -xf "${BINUTIL_SRC}.tar" || exit
	else
		(wget -O "${BINUTIL_SRC}.tar" ${BINUTIL_URL} \
			&& tar -xf "${BINUTIL_SRC}.tar") || exit
		rm -f "${BINUTIL_SRC}.tar"
	fi
else
	echo "skip downloading binutils"
fi

echo "Building binutils"

cd "$TOOLS_DIR/binutils-build"

("$TOOLS_DIR/${BINUTIL_SRC}/configure" --target=$TARGET --prefix="$PREFIX" \
	--with-sysroot --disable-nls --disable-werror) || exit

(make && sudo make install) || exit

echo "Binutils build successfully!"

echo "Building GCC"

cd "$TOOLS_DIR/gcc-build"

which -- "$TARGET-as" || echo "$TARGET-as is not in the PATH"

("$TOOLS_DIR/${GCC_SRC}/configure" --target=$TARGET --prefix="$PREFIX" \
	--disable-nls --enable-languages=c,c++ --without-headers) || exit

(make all-gcc &&\
 make all-target-libgcc &&\
 sudo make install-gcc &&\
 sudo make install-target-libgcc) || exit

echo "done"
