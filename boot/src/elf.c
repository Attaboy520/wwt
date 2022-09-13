#include <elf.h>
#include <boot.h>
#include <error.h>
#include <memory.h>

VOID printELFFileInfo(IN VOID* const headerPtr,
	IN VOID* const programHeadersPtr)
{
	ELF32Ehdr* header = (ELF32Ehdr*)headerPtr;
	ELF64Ehdr* header64 = (ELF64Ehdr*)headerPtr;

	debugPrint(L"Debug: ELF Header Info:\n");

	debugPrint(L"  Magic:                    ");
	UINTN i = 0;
	for(i = 0; i < 4; i++) {
		debugPrint(L"0x%x ", header->e_ident[i]);
	}
	debugPrint(L"\n");

	debugPrint(L"  Class:                    ");
	if(header->e_ident[EI_CLASS] == ELFCLASS32) {
		debugPrint(L"32bit");
	} else if(header->e_ident[EI_CLASS] == ELFCLASS64) {
		debugPrint(L"64bit");
	}
	debugPrint(L"\n");

	debugPrint(L"  Endianness:               ");
	if(header->e_ident[EI_DATA] == 1) {
		debugPrint(L"Little-Endian");
	} else if(header->e_ident[EI_DATA] == 2) {
		debugPrint(L"Big-Endian");
	}
	debugPrint(L"\n");

	debugPrint(L"  Version:                  0x%x\n",
		header->e_ident[EI_VERSION]);

	debugPrint(L"  OS ABI:                   ");
	if(header->e_ident[EI_OSABI] == 0x00) {
		debugPrint(L"System V");
	} else if(header->e_ident[EI_OSABI] == 0x01) {
		debugPrint(L"HP-UX");
	} else if(header->e_ident[EI_OSABI] == 0x02) {
		debugPrint(L"NetBSD");
	} else if(header->e_ident[EI_OSABI] == 0x03) {
		debugPrint(L"Linux");
	} else if(header->e_ident[EI_OSABI] == 0x04) {
		debugPrint(L"GNU Hurd");
	} else if(header->e_ident[EI_OSABI] == 0x06) {
		debugPrint(L"Solaris");
	} else if(header->e_ident[EI_OSABI] == 0x07) {
		debugPrint(L"AIX");
	} else if(header->e_ident[EI_OSABI] == 0x08) {
		debugPrint(L"IRIX");
	} else if(header->e_ident[EI_OSABI] == 0x09) {
		debugPrint(L"FreeBSD");
	} else if(header->e_ident[EI_OSABI] == 0x0A) {
		debugPrint(L"Tru64");
	} else if(header->e_ident[EI_OSABI] == 0x0B) {
		debugPrint(L"Novell Modesto");
	} else if(header->e_ident[EI_OSABI] == 0x0C) {
		debugPrint(L"OpenBSD");
	} else if(header->e_ident[EI_OSABI] == 0x0D) {
		debugPrint(L"OpenVMS");
	} else if(header->e_ident[EI_OSABI] == 0x0E) {
		debugPrint(L"NonStop Kernel");
	} else if(header->e_ident[EI_OSABI] == 0x0F) {
		debugPrint(L"AROS");
	} else if(header->e_ident[EI_OSABI] == 0x10) {
		debugPrint(L"Fenix OS");
	} else if(header->e_ident[EI_OSABI] == 0x11) {
		debugPrint(L"CloudABI");
	}
	debugPrint(L"\n");

	debugPrint(L"  File Type:                ");
	if(header->e_type == 0x00) {
		debugPrint(L"None");
	} else if(header->e_type == 0x01) {
		debugPrint(L"Relocatable");
	} else if(header->e_type == 0x02) {
		debugPrint(L"Executable");
	} else if(header->e_type == 0x03) {
		debugPrint(L"Dynamic");
	} else {
		debugPrint(L"Other");
	}
	debugPrint(L"\n");

	debugPrint(L"  Machine Type:             ");
	if(header->e_machine == 0x00) {
		debugPrint(L"No specific instruction set");
	} else if(header->e_machine == 0x02) {
		debugPrint(L"SPARC");
	} else if(header->e_machine == 0x03) {
		debugPrint(L"x86");
	} else if(header->e_machine == 0x08) {
		debugPrint(L"MIPS");
	} else if(header->e_machine == 0x14) {
		debugPrint(L"PowerPC");
	} else if(header->e_machine == 0x16) {
		debugPrint(L"S390");
	} else if(header->e_machine == 0x28) {
		debugPrint(L"ARM");
	} else if(header->e_machine == 0x2A) {
		debugPrint(L"SuperH");
	} else if(header->e_machine == 0x32) {
		debugPrint(L"IA-64");
	} else if(header->e_machine == 0x3E) {
		debugPrint(L"x86-64");
	} else if(header->e_machine == 0xB7) {
		debugPrint(L"AArch64");
	} else if(header->e_machine == 0xF3) {
		debugPrint(L"RISC-V");
	}
	debugPrint(L"\n");

	if(header->e_ident[EI_CLASS] == ELFCLASS32) {
		debugPrint(L"  Entry point:              0x%lx\n", header->e_entry);
		debugPrint(L"  Program header offset:    0x%lx\n", header->e_phoff);
		debugPrint(L"  Section header offset:    0x%lx\n", header->e_shoff);
		debugPrint(L"  Program header count:     %u\n", header->e_phnum);
		debugPrint(L"  Section header count:     %u\n", header->e_shnum);

		ELF32Phdr* programHeaders = programHeadersPtr;

		debugPrint(L"\nProgram Headers:\n");
		UINTN p = 0;
		for(p = 0; p < header->e_phnum; p++) {
			debugPrint(L"[%u]:\n", p);
			debugPrint(L"  p_type:      0x%lx\n", programHeaders[p].p_type);
			debugPrint(L"  p_offset:    0x%lx\n", programHeaders[p].p_offset);
			debugPrint(L"  p_vaddr:     0x%lx\n", programHeaders[p].p_vaddr);
			debugPrint(L"  p_paddr:     0x%lx\n", programHeaders[p].p_paddr);
			debugPrint(L"  p_filesz:    0x%lx\n", programHeaders[p].p_filesz);
			debugPrint(L"  p_memsz:     0x%lx\n", programHeaders[p].p_memsz);
			debugPrint(L"  p_flags:     0x%lx\n", programHeaders[p].p_flags);
			debugPrint(L"  p_align:     0x%lx\n", programHeaders[p].p_align);
			debugPrint(L"\n");
		}
	} else if(header->e_ident[EI_CLASS] == ELFCLASS64) {
		debugPrint(L"  Entry point:              0x%llx\n", header64->e_entry);
		debugPrint(L"  Program header offset:    0x%llx\n", header64->e_phoff);
		debugPrint(L"  Section header offset:    0x%llx\n", header64->e_shoff);
		debugPrint(L"  Program header count:     %u\n", header64->e_phnum);
		debugPrint(L"  Section header count:     %u\n", header64->e_shnum);

		ELF64Phdr* programHeaders = programHeadersPtr;

		debugPrint(L"\nDebug: Program Headers:\n");
		UINTN p = 0;
		for(p = 0; p < header64->e_phnum; p++) {
			debugPrint(L"[%u]:\n", p);
			debugPrint(L"  p_type:      0x%lx\n",  programHeaders[p].p_type);
			debugPrint(L"  p_flags:     0x%lx\n",  programHeaders[p].p_flags);
			debugPrint(L"  p_offset:    0x%llx\n", programHeaders[p].p_offset);
			debugPrint(L"  p_vaddr:     0x%llx\n", programHeaders[p].p_vaddr);
			debugPrint(L"  p_paddr:     0x%llx\n", programHeaders[p].p_paddr);
			debugPrint(L"  p_filesz:    0x%llx\n", programHeaders[p].p_filesz);
			debugPrint(L"  p_memsz:     0x%llx\n", programHeaders[p].p_memsz);
			debugPrint(L"  p_align:     0x%llx\n", programHeaders[p].p_align);
			debugPrint(L"\n");
		}
	}
}


EFI_STATUS readELFFile(IN EFI_FILE* const kernelFile,
	IN ELFFileClass const fileClass,
	OUT VOID** kernelHeaderBuffer,
	OUT VOID** kernelProgramHeadersBuffer)
{
	UINTN bufferSize = 0;
	UINTN programHeadersOffset = 0;

	#ifdef DEBUG
		debugPrint(L"Debug: Setting file pointer to "
			"read executable header\n");
	#endif

	// EFI_STATUS status = uefi_call_wrapper(kernelFile->SetPosition, 2, kernelFile, 0);
	EFI_STATUS status = setPosition(kernelFile, 0);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error setting file pointer position: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	if(fileClass == ELFCLASS32) {
		bufferSize = sizeof(ELF32Ehdr);
	} else if(fileClass == ELFCLASS64) {
		bufferSize = sizeof(ELF64Ehdr);
	} else {
		debugPrint(L"Error: Invalid file class\n", status);
		return EFI_INVALID_PARAMETER;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Allocating '0x%lx' for ", bufferSize);
		debugPrint(L"kernel executable header buffer\n");
	#endif

	status = allocateMemory(EfiLoaderData, bufferSize, kernelHeaderBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error allocating kernel header buffer: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Reading kernel executable header\n");
	#endif

	status = uefi_call_wrapper(kernelFile->Read, 3,
		kernelFile, &bufferSize, *kernelHeaderBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error reading kernel header: %s\n",
			getEFIErrorMessage(status));

		return status;
	}


	if(fileClass == ELFCLASS32) {
		programHeadersOffset = ((ELF32Ehdr*)*kernelHeaderBuffer)->e_phoff;
		bufferSize = sizeof(ELF32Phdr) *
			((ELF32Ehdr*)*kernelHeaderBuffer)->e_phnum;
	} else if(fileClass == ELFCLASS64) {
		programHeadersOffset = ((ELF64Ehdr*)*kernelHeaderBuffer)->e_phoff;
		bufferSize = sizeof(ELF64Phdr) *
			((ELF64Ehdr*)*kernelHeaderBuffer)->e_phnum;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Setting file offset to '0x%lx' "
			"to read program headers\n", programHeadersOffset);
	#endif

	// status = uefi_call_wrapper(kernelFile->SetPosition, 2, kernelFile, programHeadersOffset);
	status = setPosition(kernelFile, programHeadersOffset);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error setting file pointer position: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Allocating '0x%lx' for program headers buffer\n",
			bufferSize);
	#endif

	status = allocateMemory(EfiLoaderData, bufferSize, kernelProgramHeadersBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error allocating kernel "
			"program header buffer: %s\n", getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Reading program headers\n");
	#endif

	status = uefi_call_wrapper(kernelFile->Read, 3,
		kernelFile, &bufferSize, *kernelProgramHeadersBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error reading kernel program headers: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	return EFI_SUCCESS;
}

EFI_STATUS readELFIdentity(IN EFI_FILE* const kernelFile,
	OUT UINT8** elfIdentityBuffer)
{
	UINTN bufferSize = EI_NIDENT;

	#ifdef DEBUG
		debugPrint(L"Debug: Setting file pointer position "
			"to read ELF identity\n");
	#endif

	// EFI_STATUS status = uefi_call_wrapper(kernelFile->SetPosition, 2, kernelFile, 0);
	EFI_STATUS status = setPosition(kernelFile, 0);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error resetting file pointer position: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Allocating buffer for ELF identity\n");
	#endif

	status = allocateMemory(EfiLoaderData, EI_NIDENT, (VOID**)elfIdentityBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error allocating kernel identity buffer: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Reading ELF identity\n");
	#endif

	status = uefi_call_wrapper(kernelFile->Read, 3,
		kernelFile, &bufferSize, (VOID*)*elfIdentityBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error reading kernel identity: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	return EFI_SUCCESS;
}

EFI_STATUS validateELFIdentity(IN UINT8* const elfIdentityBuffer)
{
	if((elfIdentityBuffer[EI_MAG0] != 0x7F) ||
		(elfIdentityBuffer[EI_MAG1] != 0x45) ||
		(elfIdentityBuffer[EI_MAG2] != 0x4C) ||
		(elfIdentityBuffer[EI_MAG3] != 0x46)) {
		debugPrint(L"Fatal Error: Invalid ELF header\n");
		return EFI_INVALID_PARAMETER;
	}

	if(elfIdentityBuffer[EI_CLASS] == ELFCLASS32) {
		#ifdef DEBUG
			debugPrint(L"Debug: Found 32bit executable\n");
		#endif
	} else if(elfIdentityBuffer[EI_CLASS] == ELFCLASS64) {
		#ifdef DEBUG
			debugPrint(L"Debug: Found 64bit executable\n");
		#endif
	} else {
		debugPrint(L"Fatal Error: Invalid executable\n");

		return EFI_UNSUPPORTED;
	}

	if(elfIdentityBuffer[EI_DATA] != 1) {
		debugPrint(L"Fatal Error: Only LSB ELF executables "
			"current supported\n");

		return EFI_INCOMPATIBLE_VERSION;
	}

	return EFI_SUCCESS;
}
