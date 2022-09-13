.POSIX:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ARCH := x86_64
CC   := ${ARCH}-elf-gcc

BUILD_DIR := ../build/boot
OBJ_DIR  := ${BUILD_DIR}/src
SRC_DIR    := src
INCLUDE_DIR := include

EFI_INC_DIR  := /usr/local/include/efi
INCLUDE_DIRS := ${EFI_INC_DIR} \
	${EFI_INC_DIR}/${ARCH} \
	${EFI_INC_DIR}/protocol \
	${INCLUDE_DIR}

# The full list of includes in correct format for gcc.
INCLUDE_FLAG := $(foreach d, $(INCLUDE_DIRS), -I$d)

CFLAGS := ${INCLUDE_FLAG}		\
	-ffreestanding				\
	-fno-stack-protector		\
	-fpic						\
	-fshort-wchar				\
	-mno-red-zone				\
	-Wall						\
	-Wextra						\
	-Wmissing-prototypes		\
	-Wstrict-prototypes			\
	-DEFI_FUNCTION_WRAPPER		\
	-fcommon

LIB          := /usr/local/lib
EFI_LIB      := /usr/local/lib
EFI_CRT_OBJS := ${EFI_LIB}/crt0-efi-${ARCH}.o
EFI_LDS      := ${EFI_LIB}/elf_${ARCH}_efi.lds
LDFLAGS      := -nostdlib		\
	-znocombreloc				\
	-T ${EFI_LDS}				\
	-shared						\
	-Bsymbolic					\
	-L ${EFI_LIB}				\
	-L ${LIB} ${EFI_CRT_OBJS}	\
	-fcommon


C_SOURCES :=  \
	${SRC_DIR}/boot.c \
	${SRC_DIR}/elf.c \
	${SRC_DIR}/error.c \
	${SRC_DIR}/file.c \
	${SRC_DIR}/graphics.c \
	${SRC_DIR}/loader.c \
	${SRC_DIR}/main.c \
	${SRC_DIR}/memory.c \
	${SRC_DIR}/serial.c \
	${SRC_DIR}/font.c

AS_SOURCES :=


# OBJECTS := ${C_SOURCES:.c=.o}
# OBJECTS += ${AS_SOURCES:.S=.o}

OBJECTS := $(patsubst ${SRC_DIR}/%.c,${OBJ_DIR}/%.o,${C_SOURCES})
OBJECTS += $(patsubst ${SRC_DIR}/%.S,${OBJ_DIR}/%.o,${AS_SOURCES})


# The boot is initially compiled into a shared lib in ELF format. GNU-EFI links
# the lib in a special format as to be easily copied into a PE32+ compatible executable
# suitable for use as a UEFI boot.
BINARY_ELF := ${BUILD_DIR}/bootx64.so
# This is the ELF shared lib copied into the PE32+ format.
BINARY_EFI := ${BUILD_DIR}/bootx64.efi


.PHONY: all clean emu

all: ${BINARY_EFI}

${BINARY_EFI}: ${BINARY_ELF}
	objcopy -j .text          \
		-j .sdata               \
		-j .data                \
		-j .dynamic             \
		-j .dynsym              \
		-j .rel                 \
		-j .rela                \
		-j .reloc               \
		--target=efi-app-${ARCH} $^ $@

${BINARY_ELF}: ${OBJ_DIR} ${OBJECTS}
	ld ${LDFLAGS} ${OBJECTS} -o $@ -lefi -lgnuefi

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -o $@ -c $<

${OBJ_DIR}/%.o: ${SRC_DIR}/%.S
	${CC} ${CFLAGS} -o $@ -c $<

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}

${BUILD_DIR}:
	@mkdir -p ${BUILD_DIR}

test1: ${OBJ_DIR}/1.txt

%.txt :
	echo 1 >> ${OBJ_DIR}/1.txt