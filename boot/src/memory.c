#include <memory.h>
#include <error.h>
#include <serial.h>
#include <boot.h>

EFI_STATUS getMemoryMap(OUT VOID** memoryMap,
	OUT UINTN* memoryMapSize,
	OUT UINTN* memoryMapKey,
	OUT UINTN* descriptorSize,
	OUT UINT32* descriptorVersion)
{
	EFI_STATUS status;

	#ifdef DEBUG
		debugPrint(L"Debug: Allocating memory map\n");
	#endif

	status = uefi_call_wrapper(gBS->GetMemoryMap, 5,
		memoryMapSize, *memoryMap, memoryMapKey,
		descriptorSize, descriptorVersion);
	if(EFI_ERROR(status)) {
		if(status != EFI_BUFFER_TOO_SMALL) {
			debugPrint(L"Fatal Error: Error getting memory map size: %s\n",
				getEFIErrorMessage(status));

			#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
				EFI_INPUT_KEY inputKey;
				debugPrint(L"Press any key to reboot...");
				waitForInput(&inputKey);
			#endif

			return status;
		}
	}

	*memoryMapSize += 2 * (*descriptorSize);

	status = allocateMemory(EfiLoaderData, *memoryMapSize, (VOID**)memoryMap);
	if(checkForError(status, L"Error allocating memory map buffer")) {
		return status;
	}

	status = uefi_call_wrapper(gBS->GetMemoryMap, 5,
		memoryMapSize, *memoryMap, memoryMapKey,
		descriptorSize, descriptorVersion);
	if(checkForError(status, L"Error getting memory map")) {
		return status;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateMemory(IN EFI_MEMORY_TYPE PoolType, IN UINTN Size, OUT VOID **Buffer) {
	return uefi_call_wrapper(gBS->AllocatePool, 3,
		PoolType, Size, Buffer);
}