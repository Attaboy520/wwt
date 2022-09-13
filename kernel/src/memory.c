#include <memory.h>
#include <graphics.h>
#include <string.h>
static const char* EFI_MEMORY_TYPE_STRINGS[] = {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode"
};

void initializeMemory(MemoryInfo* memoryMap)
{
    memoryMapInfo = memoryMap;
}

uint64_t getMemorySize(void) {
    if (!memoryMapInfo)
        return 0;
    static uint64_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;
    uint64_t mapEntries = memoryMapInfo->mapSize / memoryMapInfo->mapDescriptorSize;
    for (uint64_t i = 0; i < mapEntries; i++)
    {
        MemoryMapDescriptor* descriptor = (MemoryMapDescriptor*)((uint64_t)memoryMapInfo->memoryMap + (i * memoryMapInfo->mapDescriptorSize));
        memorySizeBytes += descriptor->count * 4096; 
    }
    return memorySizeBytes;
}

void memset(void* entry, uint8_t value, uint64_t num) {
    for (uint64_t i = 0; i < num; ++i) {
        *(uint8_t*)((uint64_t)entry + i) = value;
    }
}

void test(void) {
    MemoryMapDescriptor* descriptor = (MemoryMapDescriptor*)(memoryMapInfo->memoryMap);
    uint64_t mapEntries = memoryMapInfo->mapSize / memoryMapInfo->mapDescriptorSize;
    debugPrint("mapEntries: %d\n", mapEntries);
    for (uint64_t i = 0; i < mapEntries; i++)
    {
        descriptor = (MemoryMapDescriptor*)((uint64_t)memoryMapInfo->memoryMap + (i * memoryMapInfo->mapDescriptorSize));
        debugPrint("type: %s   ", EFI_MEMORY_TYPE_STRINGS[descriptor->type]);
        // debugPrint("physicalStart: %d\n", descriptor->physicalStart);
        // debugPrint("virtualStart: %d\n", descriptor->virtualStart);
        debugPrint("count: %dKB\n", (descriptor->count * 4096 / 1024));
        // debugPrint("attributes: %d\n", descriptor->attributes);
    }

}