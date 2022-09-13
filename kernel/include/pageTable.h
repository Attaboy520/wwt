#ifndef _KERNEL_PAGE_H_
#define _KERNEL_PAGE_H_

#include <stdint.h>
#include <stdbool.h>

/* 页掩码，取高20位 */
#define PAGE_MASK 0x000ffffffffff000
#define PAGE_SIZE 0x1000

typedef enum _PTFlag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 // only if supported
} PTFlag;

/**
 * struct _HARDWARE_PTE_X86 (sizeof=4)
 * bits0-0 Valid
 * bits1-1 Write
 * bits2-2 Owner
 * bits3-3 WriteThrough
 * bits4-4 CacheDisable
 * bits5-5 Accessed
 * bits6-6 Dirty
 * bits7-7 LargePage
 * bits8-8 Global
 * bits9-11 reserved 没有定义
 * bits12-31 PageFrameNumber
 */


typedef struct _pageTable { 
    uint64_t entries [512];
} PageTable;

/* Each level of page tables is responsible for 9 bits of the virtual address.
 * PML4 39:47 (inclusive)
 * PDPT 30:38 (inclusive)
 * PD   21:29 (inclusive)
 * PT   12:20 (inclusive)
 * (0:11 are the offset in a 4K page)
 * The position of the bit of the virtual memory address that the page table level refers to.
 */
#define PML_INDEX(x) 0x1ff & (x >> 39)
#define PDPT_INDEX(x) 0x1ff & (x >> 30)
#define PD_INDEX(x) 0x1ff & (x >> 21)
#define PT_INDEX(x) 0x1ff & (x >> 12)

typedef struct _pageTableIndex {
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
} PageTableIndex;

void setPageTableFlag(uint64_t* entry, PTFlag flag, bool enabled);
bool getPageTableFlag(uint64_t* const entry, PTFlag flag);
uint64_t getPageTableAddress(uint64_t* const entry);
void setPageTableAddress(uint64_t* entry, uint64_t address);
void getPageTableIndex(PageTableIndex* const p, uint64_t virtualAddress);

void initializePageTable(PageTable* const pml4Address);
void mapMemory(void* const virtualMemory, void* const physicalMemory);

PageTable* pageTable;
#endif /* _KERNEL_PAGE_H_ */