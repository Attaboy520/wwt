#include <loader.h>
#include <elf.h>
#include <error.h>
#include <serial.h>
#include <boot.h>
#include <memory.h>

#define KERNEL_PATH L"\\kernel.elf"
#define PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR 1

EFI_STATUS loadSegment(IN EFI_FILE* const kernelFile,
	IN EFI_PHYSICAL_ADDRESS const segmentFileOffset,
	IN UINTN const segmentFileSize,
	IN UINTN const segmentMemorySize,
	IN EFI_PHYSICAL_ADDRESS const segmentPhysicalAddress)
{
	EFI_STATUS status;
	VOID* programData = NULL;
	UINTN bufferSize = 0;
	UINTN segmentPageCount = EFI_SIZE_TO_PAGES(segmentMemorySize);
	EFI_PHYSICAL_ADDRESS zeroFillStart = 0;
	UINTN zeroFillCount = 0;

	#ifdef DEBUG
		debugPrint(L"Debug: Setting file pointer to segment "
			"offset '0x%llx'\n", segmentFileOffset);
	#endif

	// status = uefi_call_wrapper(kernelFile->SetPosition, 2, kernelFile, segmentFileOffset);
	status = setPosition(kernelFile, segmentFileOffset);
	if(checkForError(status, L"Error setting file pointer to segment offset")) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Allocating %lu pages at address '0x%llx'\n",
			segmentPageCount, segmentPhysicalAddress);
	#endif

	status = uefi_call_wrapper(gBS->AllocatePages, 4,
		AllocateAddress, EfiLoaderData, segmentPageCount,
		(EFI_PHYSICAL_ADDRESS*)&segmentPhysicalAddress);
	if(checkForError(status, L"Error allocating pages for ELF segment")) {
		return status;
	}

	if(segmentFileSize > 0) {
		bufferSize = segmentFileSize;

		#ifdef DEBUG
			debugPrint(L"Debug: Allocating segment buffer with size '0x%llx'\n",
				bufferSize);
		#endif

		status = allocateMemory(EfiLoaderCode, bufferSize, (VOID**)&programData);
		if(checkForError(status, L"Error allocating kernel segment buffer")) {
			return status;
		}

		#ifdef DEBUG
			debugPrint(L"Debug: Reading segment data with file size '0x%llx'\n",
				bufferSize);
		#endif

		status = uefi_call_wrapper(kernelFile->Read, 3,
			kernelFile, &bufferSize, (VOID*)programData);
		if(checkForError(status, L"Error reading segment data")) {
			return status;
		}

		#ifdef DEBUG
			debugPrint(L"Debug: Copying segment to memory address '0x%llx'\n",
				segmentPhysicalAddress);
		#endif

		status = uefi_call_wrapper(gBS->CopyMem, 3,
			segmentPhysicalAddress, programData, segmentFileSize);
		if(checkForError(status, L"Error copying program section into memory")) {
			return status;
		}

		#ifdef DEBUG
			debugPrint(L"Debug: Freeing program section data buffer\n");
		#endif

		status = uefi_call_wrapper(gBS->FreePool, 1, programData);
		if(checkForError(status, L"Error freeing program section")) {
			return status;
		}
	}

	zeroFillStart = segmentPhysicalAddress + segmentFileSize;
	zeroFillCount = segmentMemorySize - segmentFileSize;

	if(zeroFillCount > 0) {
		#ifdef DEBUG
			debugPrint(L"Debug: Zero-filling %llu bytes at address '0x%llx'\n",
				zeroFillCount, zeroFillStart);
		#endif

		status = uefi_call_wrapper(gBS->SetMem, 3,
			zeroFillStart, zeroFillCount, 0);
		if(checkForError(status, L"Error zero filling segment")) {
			return status;
		}
	}

	return EFI_SUCCESS;
}

EFI_STATUS loadProgramSegments(IN EFI_FILE* const kernelFile,
	IN ELFFileClass const fileClass,
	IN VOID* const kernelHeaderBuffer,
	IN VOID* const kernelProgramHeadersBuffer)
{
	EFI_STATUS status;
	UINT16 programHeadersSize = 0;
	UINT16 segmentsLoadedSize = 0;
	UINTN p = 0;

	if(fileClass == ELFCLASS32) {
		programHeadersSize = ((ELF32Ehdr*)kernelHeaderBuffer)->e_phnum;
	} else if(fileClass == ELFCLASS64) {
		programHeadersSize = ((ELF64Ehdr*)kernelHeaderBuffer)->e_phnum;
	}

	if(programHeadersSize == 0) {
		debugPrint(L"Fatal Error: No program segments to load ");
		debugPrint(L"in Kernel image\n");

		return EFI_INVALID_PARAMETER;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Loading %u segments\n", programHeadersSize);
	#endif


	if(fileClass == ELFCLASS32) {
		ELF32Phdr* programHeaders = (ELF32Phdr*)kernelProgramHeadersBuffer;

		for(p = 0; p < programHeadersSize; p++) {
			if(programHeaders[p].p_type == PT_LOAD) {
				status = loadSegment(kernelFile,
					programHeaders[p].p_offset,
					programHeaders[p].p_filesz,
					programHeaders[p].p_memsz,
					programHeaders[p].p_paddr);
				if(EFI_ERROR(status)) {
					return status;
				}

				segmentsLoadedSize++;
			}
		}
	} else if(fileClass == ELFCLASS64) {
		ELF64Phdr* programHeaders = (ELF64Phdr*)kernelProgramHeadersBuffer;

		for(p = 0; p < programHeadersSize; p++) {
			if(programHeaders[p].p_type == PT_LOAD){
				status = loadSegment(kernelFile,
					programHeaders[p].p_offset,
					programHeaders[p].p_filesz,
					programHeaders[p].p_memsz,
					programHeaders[p].p_paddr);
				if(EFI_ERROR(status)) {
					return status;
				}

				segmentsLoadedSize++;
			}
		}
	}

	if(segmentsLoadedSize == 0) {
		debugPrint(L"Fatal Error: No loadable program segments ");
		debugPrint(L"found in Kernel image\n");

		return EFI_NOT_FOUND;
	}

	return EFI_SUCCESS;
}

EFI_STATUS loadKernelImage(OUT EFI_PHYSICAL_ADDRESS* kernelEntryPtr)
{
	EFI_STATUS status;
	EFI_FILE* kernelFile;
	VOID* kernelHeader = NULL;
	VOID* kernelProgramHeaders = NULL;
	UINT8* elfIdentityBuffer = NULL;
	ELFFileClass fileClass = ELFCLASSNONE;

	#ifdef DEBUG
		debugPrint(L"Debug: Reading kernel image file\n");
	#endif

	status = openFile(KERNEL_PATH, &kernelFile);
	if(checkForError(status, L"Error opening kernel file")) {
		return status;
	}

	status = readELFIdentity(kernelFile, &elfIdentityBuffer);
	if(checkForError(status, L"Error reading executable identity")) {
		return status;
	}

	fileClass = elfIdentityBuffer[EI_CLASS];

	status = validateELFIdentity(elfIdentityBuffer);
	if(EFI_ERROR(status)) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: ELF header is valid\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, elfIdentityBuffer);
	if(checkForError(status, L"Error freeing kernel identity buffer")) {
		return status;
	}

	status = readELFFile(kernelFile, fileClass,
		&kernelHeader, &kernelProgramHeaders);
	if(checkForError(status, L"Error reading ELF file")) {
		return status;
	}

	#ifdef DEBUG
		printELFFileInfo(kernelHeader, kernelProgramHeaders);
	#endif

	if(fileClass == ELFCLASS32) {
		*kernelEntryPtr = ((ELF32Ehdr*)kernelHeader)->e_entry;
	} else if(fileClass == ELFCLASS64) {
		*kernelEntryPtr = ((ELF64Ehdr*)kernelHeader)->e_entry;
	}

	status = loadProgramSegments(kernelFile, fileClass,
		kernelHeader, kernelProgramHeaders);
	if(EFI_ERROR(status)) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Closing kernel binary\n");
	#endif

	// status = uefi_call_wrapper(kernelFile->Close, 1, kernelFile);
	status = closeFile(kernelFile);
	if(checkForError(status, L"Error closing kernel image")) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Freeing kernel header buffer\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, (VOID*)kernelHeader);
	if(checkForError(status, L"Error freeing kernel header buffer")) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Freeing kernel program header buffer\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, (VOID*)kernelProgramHeaders);
	if(checkForError(status, L"Error freeing kernel program headers buffer")) {
		return status;
	}


	return status;
}
