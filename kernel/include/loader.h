#ifndef _KERNEL_LOADER_H_ 
#define _KERNEL_LOADER_H_

#include <stdint.h>
#include <memory.h>
#include <graphics.h>
#include <font.h>

typedef struct _bootInfo {
	MemoryInfo memoryMap;
	KernelBootVideoModeInfo videoModeInfo;
	PSF1Font* psf1Font;
} BootInfo;

#endif /* _KERNEL_LOADER_H_ */