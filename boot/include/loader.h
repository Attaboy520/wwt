#ifndef _BOOT_LOADER_H_
#define _BOOT_LOADER_H_

#include <efi.h>
#include <efilib.h>
#include <elf.h>

EFI_STATUS loadSegment(IN EFI_FILE* const kernelFile,
	IN EFI_PHYSICAL_ADDRESS const segmentFileOffset,
	IN UINTN const segmentFileSize,
	IN UINTN const segmentMemorySize,
	IN EFI_PHYSICAL_ADDRESS const segmentPhysicalAddress);

EFI_STATUS loadProgramSegments(IN EFI_FILE* const kernelFile,
	IN ELFFileClass const fileClass,
	IN VOID* const kernelHeaderBuffer,
	IN VOID* const kernelProgramHeadersBuffer);

EFI_STATUS loadKernelImage(OUT EFI_PHYSICAL_ADDRESS* kernelEntryPtr);

#endif /* _BOOT_LOADER_H_ */
