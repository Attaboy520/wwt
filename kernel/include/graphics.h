#ifndef _KERNEL_GRAPHICS_H_
#define _KERNEL_GRAPHICS_H_

#include <stdint.h>
#include <stdbool.h>
#include <type.h>

typedef struct _bootVideoInfo {
	uint32_t* frameBufferPointer;
	uint64_t frameBufferSize;
	uint32_t horizontalResolution;
	uint32_t verticalResolution;
	uint32_t pixelsPerScanline;
} KernelBootVideoModeInfo;

void initializeGraphics(IN KernelBootVideoModeInfo* _bootVideoModeInfo);
bool getResolution(OUT uint32_t* horizontal, OUT uint32_t* vertical);
void drawRect(IN const uint16_t x, IN const uint16_t y, IN const uint16_t width, IN const uint16_t height, IN const uint32_t color);
void print(IN const char* chr);
void putChar(IN char chr);
void put(IN char chr, IN uint32_t xOffset, IN uint32_t yOffset);
void next(void);
void clear(void);
void debugPrint(IN char* fmt, ...);

KernelBootVideoModeInfo* bootVideoInfo;
#endif /* _KERNEL_GRAPHICS_H_ */