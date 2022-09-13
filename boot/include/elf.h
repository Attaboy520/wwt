#ifndef _BOOT_ELF_H_
#define _BOOT_ELF_H_

#include <efi.h>
#include <efilib.h>


#define EI_NIDENT        16

#define EI_MAG0          0
#define EI_MAG1          0x1
#define EI_MAG2          0x2
#define EI_MAG3          0x3
#define EI_CLASS         0x4
#define EI_DATA          0x5
#define EI_VERSION       0x6
#define EI_OSABI         0x7
#define EI_ABIVERSION    0x8

#define PT_NULL            0
#define PT_LOAD            1
#define PT_DYNAMIC         2
#define PT_INTERP          3
#define PT_NOTE            4
#define PT_SHLIB           5
#define PT_PHDR            6
#define PT_TLS             7


/**
 * @brief The ELF file class.
 */
typedef enum fileClass {
	ELFCLASSNONE = 0,
	ELFCLASS32 = 1,
	ELFCLASS64 = 2,
} ELFFileClass;

/**
 * @brief The 32-bit ELF header.
 */
typedef struct _elf32_hdr {
	unsigned char	e_ident[EI_NIDENT];
	UINT16 e_type;
	UINT16 e_machine;
	UINT32 e_version;
	UINT32 e_entry;
	UINT32 e_phoff;
	UINT32 e_shoff;
	UINT32 e_flags;
	UINT16 e_ehsize;
	UINT16 e_phentsize;
	UINT16 e_phnum;
	UINT16 e_shentsize;
	UINT16 e_shnum;
	UINT16 e_shstrndx;
} ELF32Ehdr;

/**
 * @brief The 64-bit ELF header.
 */
typedef struct _elf64_hdr {
	unsigned char	e_ident[EI_NIDENT];
	UINT16 e_type;
	UINT16 e_machine;
	UINT32 e_version;
	UINT64 e_entry;
	UINT64 e_phoff;
	UINT64 e_shoff;
	UINT32 e_flags;
	UINT16 e_ehsize;
	UINT16 e_phentsize;
	UINT16 e_phnum;
	UINT16 e_shentsize;
	UINT16 e_shnum;
	UINT16 e_shstrndx;
} ELF64Ehdr;

/**
 * @brief The 32-bit ELF program header.
 */
typedef struct _elf32_phdr {
	UINT32 p_type;
	UINT32 p_offset;
	UINT32 p_vaddr;
	UINT32 p_paddr;
	UINT32 p_filesz;
	UINT32 p_memsz;
	UINT32 p_flags;
	UINT32 p_align;
} ELF32Phdr;

/**
 * @brief The 64-bit ELF program header.
 */
typedef struct _elf64_phdr {
	UINT32 p_type;
	UINT32 p_flags;
	UINT64 p_offset;
	UINT64 p_vaddr;
	UINT64 p_paddr;
	UINT64 p_filesz;
	UINT64 p_memsz;
	UINT64 p_align;
} ELF64Phdr;


VOID printELFFileInfo(IN VOID* const headerPtr,
	IN VOID* const programHeadersPtr);


EFI_STATUS readELFFile(IN EFI_FILE* const kernelFile,
	IN ELFFileClass const fileClass,
	OUT VOID** kernelHeaderBuffer,
	OUT VOID** kernelProgramHeadersBuffer);

EFI_STATUS readELFIdentity(IN EFI_FILE* const kernelFile,
	OUT UINT8** elfIdentityBuffer);

EFI_STATUS validateELFIdentity(IN UINT8* const elfIdentityBuffer);

#endif /* _BOOT_ELF_H_ */
