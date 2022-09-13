#include <pageFrame.h>
#include <memory.h>

uint64_t FREE_MEMORY;
uint64_t RESERVED_MEMORY;
uint64_t USED_MEMORY;
uint64_t pageBitmapIndex = 0;

void initializePageFrame(void) {
    if (!memoryMapInfo)
        return;
    uint64_t memoryMapEntries = memoryMapInfo->mapSize / memoryMapInfo->mapDescriptorSize;

    void* largestFreeMemorySegment = NULL;
    size_t largestFreeMemorySegmentSize = 0;

    MemoryMapDescriptor* descriptor;
    for (uint64_t i = 0; i < memoryMapEntries; ++i) {
        descriptor = (MemoryMapDescriptor*)((uint64_t)memoryMapInfo->memoryMap + (i * memoryMapInfo->mapDescriptorSize));
        if (descriptor->type == 7) {
            if (descriptor->count * 4096 > largestFreeMemorySegmentSize) {
                largestFreeMemorySegment = (void*)descriptor->physicalStart;
                largestFreeMemorySegmentSize = descriptor->count * 4096;
            }
        }
    }

    uint64_t memorySize = getMemorySize();
    FREE_MEMORY = memorySize;

    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
    initializeBitmap(bitmapSize, largestFreeMemorySegment);

    reservePages(0, memorySize / 4096 + 1);
    for (uint64_t i = 0; i < memoryMapEntries; ++i) {
        descriptor = (MemoryMapDescriptor*)((uint64_t)memoryMapInfo->memoryMap + (i * memoryMapInfo->mapDescriptorSize));
        if (descriptor->type == 7) {
            unreservePages((void*)descriptor->physicalStart, descriptor->count);
        }
    }

    reservePages(0, 0x100);
    lockPages(pageBitmap.bits, pageBitmap.size / 4096 + 1);
}

void initializeBitmap(size_t bitmapSize, void* bufferAddress) {
    pageBitmap.size = bitmapSize;
    pageBitmap.bits = (uint8_t*)bufferAddress;
    for (uint64_t i = 0; i < bitmapSize; ++i) {
        *(uint8_t*)(pageBitmap.bits + i) = 0;
    }
}

void reservePages(void* address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; ++i) {
        reservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void reservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (getBitmap(&pageBitmap, index)) return;
    if (setBitmap(&pageBitmap, index, true)) {
        FREE_MEMORY -= 4096;
        RESERVED_MEMORY += 4096;
    }
}

void unreservePages(void* address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; ++i) {
        unreservePage((void*)((uint64_t)address + (i * 4096)));
    }
}
void unreservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (!getBitmap(&pageBitmap, index)) return;
    if (setBitmap(&pageBitmap, index, false)) {
        FREE_MEMORY += 4096;
        RESERVED_MEMORY -= 4096;
        if (pageBitmapIndex > index)
            pageBitmapIndex = index;
    }
}

void lockPages(void* address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; ++i) {
        lockPage((void*)((uint64_t)address + (i * 4096)));
    }
}
void lockPage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (getBitmap(&pageBitmap, index)) return;
    if (setBitmap(&pageBitmap, index, true)) {
        FREE_MEMORY -= 4096;
        USED_MEMORY += 4096;
    }
}

void freePages(void* address, uint64_t pageCount) {
    for (uint64_t i = 0; i < pageCount; ++i) {
        freePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void freePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (!getBitmap(&pageBitmap, index)) return;
    if (setBitmap(&pageBitmap, index, false)) {
        FREE_MEMORY += 4096;
        USED_MEMORY -= 4096;
        if (pageBitmapIndex > index)
            pageBitmapIndex = index;
    }
}

void* requestPage(void) {
    for (; pageBitmapIndex < pageBitmap.size * 8; ++pageBitmapIndex) {
        if (getBitmap(&pageBitmap, pageBitmapIndex)) continue;
        lockPage((void*)(pageBitmapIndex * 4096));
        return (void*) (pageBitmapIndex * 4096);
    }
    return NULL;
}

uint64_t getFreeRAM(void) {
    return FREE_MEMORY;
}

uint64_t getUsedRAM(void) {
    return USED_MEMORY;
}

uint64_t getReservedRAM(void) {
    return RESERVED_MEMORY;
}