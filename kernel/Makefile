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


AS_SOURCES := \
	${SRC_DIR}/gdt.S 

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


BUILD_DIR := ../build/kernel
OBJ_DIR  := ${BUILD_DIR}/src
BINARY     := ${BUILD_DIR}/kernel.elf

OBJECTS := $(patsubst ${SRC_DIR}/%.c,${OBJ_DIR}/%.o,${C_SOURCES})
OBJECTS += $(patsubst ${SRC_DIR}/%.S,${BUILD_DIR}/asm/%.asm.o,${AS_SOURCES})

.PHONY: all clean

all: ${BINARY}

${BINARY}: dir ${OBJECTS}
	${CC} -T ${LDS} -g3 -o ${BINARY} ${LDFLAGS} ${OBJECTS} ${LIBS}

$(OBJ_DIR)/interrupts.o: $(SRC_DIR)/interrupts.c
	$(CC) ${INCLUDE_FLAG} -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	${CC} ${INCLUDE_FLAG} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}/asm/%.asm.o: ${SRC_DIR}/%.S
	${CC} ${INCLUDE_FLAG} ${CFLAGS} -o $@ -c $<

dir:
	mkdir -p ${OBJ_DIR}
	mkdir -p ${BUILD_DIR}/asm

com: ${OBJ_DIR} ${OBJECTS}
clean:
	rm -f ${OBJECTS}
	rm -rf ${OBJ_DIR}