#ifndef _BOOT_H_
#define _BOOT_H_

#include <efi.h>
#include <efilib.h>
#include <file.h>
#include <graphics.h>
#include <serial.h>
#include <font.h>

/**
 * 关于内容
 * 
 * 
 */

typedef struct _bootVideoInfo {
	VOID* frameBufferPointer;
	UINTN frameBufferSize;
	UINT32 horizontalResolution;
	UINT32 verticalResolution;
	UINT32 pixelsPerScanline;
} KernelBootVideoModeInfo;

typedef struct _memoryInfo
{
	EFI_MEMORY_DESCRIPTOR* memoryMap;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
} MemoryInfo;

typedef struct _bootInfo {
	MemoryInfo memoryInfo;
	KernelBootVideoModeInfo videoModeInfo;
	PSF1Font* psf1Font;
} KernelBootInfo;

EFI_STATUS waitForInput(EFI_INPUT_KEY* key);

EFI_STATUS debugPrint(IN CHAR16* fmt, ...);




#endif /* _BOOT_H_ */