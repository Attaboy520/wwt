.POSIX:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ARCH := x86_64
CC   := ${ARCH}-elf-gcc
LDS = kernel.ld

SRC_DIR := src
RENDER_DIR := ${SRC_DIR}/render
INCLUDE_DIR :=  include

INCLUDE_DIRS := ${INCLUDE_DIR}
INCLUDE_FLAG := ${foreach d, ${INCLUDE_DIRS}, -I$d}

CFLAGS :=					\
	-std=gnu99				\
	-ffreestanding			\
	-O2						\
	-Wall					\
	-Wextra					\
	-Wmissing-prototypes	\
	-Wstrict-prototypes		\
	-fcommon				

LDFLAGS := \
	-ffreestanding \
	-O2 \
	-nostdlib \
	-z max-page-size=0x1000 \
	-fcommon

LIBS := -lgcc


AS_SOURCES := ${SRC_DIR}/gdt_as.S

C_SOURCES  := \
	${SRC_DIR}/kernel.c \
	${SRC_DIR}/io.c \
	${SRC_DIR}/string.c \
	${SRC_DIR}/uart.c \
	${SRC_DIR}/graphics.c \
	${SRC_DIR}/font.c \
	${SRC_DIR}/math.c \
	${SRC_DIR}/memory.c \
	${SRC_DIR}/bitmap.c \
	${SRC_DIR}/pageFrame.c \
	${SRC_DIR}/pageTable.c \
	${SRC_DIR}/gdt.c \
	${SRC_DIR}/interrupts.c

OBJECTS    := ${AS_SOURCES:.S=.o}
OBJECTS    += ${C_SOURCES:.c=.o}

BUILD_DIR  := build
BINARY     := ${BUILD_DIR}/kernel.elf


.PHONY: all clean

all: ${BINARY}

${BINARY}: ${BUILD_DIR} ${OBJECTS}
	${CC} -T ${LDS} -g3 -o ${BINARY} ${LDFLAGS} ${OBJECTS} ${LIBS}

%.o: %.S
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

$(SRC_DIR)/interrupts.o: $(SRC_DIR)/interrupts.c
	$(CC) ${INCLUDE_FLAG} -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

%.o: %.c
	${CC} ${INCLUDE_FLAG} -g -c $< -o $@ ${CFLAGS}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -f ${OBJECTS}
	rm -rf ${BUILD_DIR}