.POSIX:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ARCH := x86_64
CC   := ${ARCH}-elf-gcc
LDS = kernel.ld

SRC_DIR := src
INCLUDE_DIR := include

INCLUDE_DIRS := ${INCLUDE_DIR}
INCLUDE_FLAG := ${foreach d, ${INCLUDE_DIRS}, -I$d}

CFLAGS :=               \
	-std=gnu99            \
	-ffreestanding        \
	-O2                   \
	-Wall                 \
	-Wextra               \
	-Wmissing-prototypes  \
	-Wstrict-prototypes	  \
	-fcommon			  \
	# -nostdinc			  
	# -nostdlib

LDFLAGS :=          \
	-T $(LDS)			\
	-ffreestanding		\
	-O2					\
	-nostdlib			\
	-z max-page-size=0x1000 \
	-fcommon

LIBS := -lgcc


AS_SOURCES :=

C_SOURCES  :=              \
	${SRC_DIR}/graphics.c    \
	${SRC_DIR}/kernel.c      \
	${SRC_DIR}/port_io.c     \
	${SRC_DIR}/string.c      \
	${SRC_DIR}/uart.c        \
	${SRC_DIR}/vga.c

OBJECTS    := ${AS_SOURCES:.S=.o}
OBJECTS    += ${C_SOURCES:.c=.o}

BUILD_DIR  := build
BINARY     := ${BUILD_DIR}/kernel.elf


.PHONY: all clean

%.o: %.S
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

%.o: %.c
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -f ${OBJECTS}
	rm -rf ${BUILD_DIR}