#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_
#include <stdint.h>

typedef struct _memoryMapDescriptor {
	uint32_t type;
	uintptr_t physicalStart;
	uintptr_t virtualStart;
	uint64_t count;
	uint64_t attributes;
} MemoryMapDescriptor;

typedef struct _memoryInfo
{
	MemoryMapDescriptor* memoryMap;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
} MemoryInfo;


void initializeMemory(MemoryInfo* memoryMap);
uint64_t getMemorySize(void);
void memset(void* entry, uint8_t value, uint64_t num);

void test(void);

MemoryInfo* memoryMapInfo;
#endif /* _KERNEL_MEMORY_H_ */