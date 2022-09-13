#ifndef _KERNEL_PAGE_FRAME_H_
#define _KERNEL_PAGE_FRAME_H_

#include <stdint.h>
#include <bitmap.h>

void initializePageFrame(void);
void initializeBitmap(size_t bitmapSize, void* bufferAddress);
void reservePages(void* address, uint64_t pageCount);
void reservePage(void* address);
void unreservePages(void* address, uint64_t pageCount);
void unreservePage(void* address);
void lockPages(void* address, uint64_t pageCount);
void lockPage(void* address);
void freePages(void* address, uint64_t pageCount);
void freePage(void* address);
void* requestPage(void);
uint64_t getFreeRAM(void);
uint64_t getUsedRAM(void);
uint64_t getReservedRAM(void);

Bitmap pageBitmap;
#endif /* _KERNEL_PAGE_FRAME_H_ */