#ifndef _BOOT_MEMORY_H_
#define _BOOT_MEMORY_H_

#include <efi.h>
#include <efilib.h>

EFI_STATUS getMemoryMap(OUT VOID** memoryMap,
	OUT UINTN* memoryMapSize,
	OUT UINTN* memoryMapKey,
	OUT UINTN* descriptorSize,
	OUT UINT32* descriptorVersion);
EFI_STATUS allocateMemory(IN EFI_MEMORY_TYPE PoolType, IN UINTN Size, OUT VOID **Buffer);

#endif /* _BOOT_MEMORY_H_ */