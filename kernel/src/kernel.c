#include <kernel.h>
#include <uart.h>
#include <graphics.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <bitmap.h>
#include <pageFrame.h>
#include <pageTable.h>
#include <gdt.h>
#include <interrupts.h>

#define TEST_SCREEN_COL_NUM             40
#define TEST_SCREEN_ROW_NUM             35
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOUR      0x00FF40FF
#define TEST_SCREEN_SECONDARY_COLOUR    0x00FFB400

extern uint64_t _kernelStart;
extern uint64_t _kernelEnd;

void drawTest(void);

void drawTest(void) {
	uint32_t width, height;
	if (getResolution(&width, &height))
	{
		width /= TEST_SCREEN_COL_NUM;
		height /= TEST_SCREEN_ROW_NUM;
		uint32_t p = 0;
		for (p = 0; p < TEST_SCREEN_TOTAL_TILES; p++) {
			uint8_t x = p % TEST_SCREEN_COL_NUM;
			uint8_t y = p / TEST_SCREEN_COL_NUM;
			uint32_t color = TEST_SCREEN_PRIMARY_COLOUR;
			if (((y %2) + x) %2) {
				color = TEST_SCREEN_SECONDARY_COLOUR;
			}
			drawRect(width * x, height * y, width, height, color);
		}
	}
}
// void _x86_64_asm_lgdt(GDTDescriptor* descriptor);
void kernel_main(IN BootInfo* bootInfo)
{
	uartInitialize();
	initializeFont(bootInfo->psf1Font);
	initializeGraphics(&bootInfo->videoModeInfo);
	initializeGDT();
	initializeMemory(&bootInfo->memoryMap);
	initializePageFrame();

	uint64_t kernelSize = (uint64_t)&_kernelEnd - (uint64_t)&_kernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	lockPages(&_kernelStart, kernelPages);

	PageTable* PML4 = (PageTable*)requestPage();
	debugPrint("Size : %d\n", sizeof(PML4));
	memset(PML4, 0, 0x1000);
	initializePageTable(PML4);
	for (uint64_t i = 0; i < getMemorySize(); i+=0x1000) {
		mapMemory((void*)i, (void*)i);
	}


	uint64_t fbBase = (uint64_t)bootInfo->videoModeInfo.frameBufferPointer;
	uint64_t fbSize = (uint64_t)bootInfo->videoModeInfo.frameBufferSize;
	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) {
		mapMemory((void*)i, (void*)i);
	}

	asm volatile("movq %0, %%cr3;"::"r"(PML4));

	initializeInterrupts();

	// asm ("int $0x21");
	// int* test1 = 0x800000000000000;
	// *test1 = 2;
	// int a = 2 / 0;

	debugPrint("Free RAM: %d KB\n", getFreeRAM() / 1024);
	debugPrint("Used RAM: %d KB\n", getUsedRAM() / 1024);
	debugPrint("Reserved RAM: %d KB\n", getReservedRAM() / 1024);
	// mapMemory((void*)0x600000000, (void*)0x80000);
	// asm ("int $0x21");
	// uint64_t* test = (uint64_t*)0x600000000;
	// *test = 26;
	// debugPrint("====== %d\n", (uint64_t)*test);

	asm ("int $0x21");
	while(1) {
		asm volatile ("hlt");
	}
}