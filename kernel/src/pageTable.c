#include <pageTable.h>
#include <memory.h>
#include <pageFrame.h>
#include <graphics.h>


void setPageTableFlag(uint64_t* entry, PTFlag flag, bool enabled) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    *entry &= ~bitSelector;
    if (enabled) {
        *entry |= bitSelector;
    }
}

bool getPageTableFlag(uint64_t* const entry, PTFlag flag) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    return *entry & bitSelector;
}

uint64_t getPageTableAddress(uint64_t* const entry) {
    return (*entry & PAGE_MASK) >> 12;
}

void setPageTableAddress(uint64_t* entry, uint64_t address) {
    address &= 0x000000ffffffffff;
    *entry &= 0xfff0000000000fff;
    *entry |= (address << 12);
}

void getPageTableIndex(PageTableIndex* const p, uint64_t virtualAddress) {
    virtualAddress >>= 12;
    p->P_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    p->PT_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    p->PD_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    p->PDP_i = virtualAddress & 0x1ff;
}

void initializePageTable(PageTable* const pml4Address) {
    pageTable = pml4Address;
}

void mapMemory(void* const virtualMemory, void* const physicalMemory) {
    PageTableIndex index;
    getPageTableIndex(&index, (uint64_t)virtualMemory);
    uint64_t PDE;
    PDE = pageTable->entries[index.PDP_i];
    PageTable* PDP;
    // 第一次创建
    if (!getPageTableFlag(&PDE, Present)) {
        PDP = (PageTable*)requestPage();
        memset(PDP, 0, PAGE_SIZE);
        setPageTableAddress(&PDE, (uint64_t)PDP >> 12);
        // debugPrint("create PDP: %h %h ", (uint64_t)&PDE, (uint64_t)PDP);
        setPageTableFlag(&PDE, Present, true);
        setPageTableFlag(&PDE, ReadWrite, true);
        pageTable->entries[index.PDP_i] = PDE;
    } else {
        PDP = (PageTable*)((uint64_t)getPageTableAddress(&PDE) << 12);
    }

    PDE = PDP->entries[index.PD_i];
    PageTable* PD;
    if (!getPageTableFlag(&PDE, Present)) {
        PD = (PageTable*)requestPage();
        // debugPrint("create PD %h %h ", (uint64_t)&PDE, (uint64_t)PD);
        memset(PD, 0, PAGE_SIZE);
        setPageTableAddress(&PDE, (uint64_t)PD >> 12);
        setPageTableFlag(&PDE, Present, true);
        setPageTableFlag(&PDE, ReadWrite, true);
        PDP->entries[index.PD_i] = PDE;
    } else {
        PD = (PageTable*)((uint64_t)getPageTableAddress(&PDE) << 12);
    }

    PDE = PD->entries[index.PT_i];
    PageTable* PT;
    if (!getPageTableFlag(&PDE, Present)) {
        PT = (PageTable*)requestPage();
        // debugPrint("create PT %h %h ", (uint64_t)PDE, (uint64_t)PT);
        memset(PT, 0, PAGE_SIZE);
        setPageTableAddress(&PDE, (uint64_t)PT >> 12);
        setPageTableFlag(&PDE, Present, true);
        setPageTableFlag(&PDE, ReadWrite, true);
        PD->entries[index.PT_i] = PDE;
    } else {
        PT = (PageTable*)((uint64_t)getPageTableAddress(&PDE) << 12);
    }

    PDE = PT->entries[index.P_i];
    setPageTableAddress(&PDE, (uint64_t)physicalMemory >> 12);
    // debugPrint("create P %h %h | ", (uint64_t)PDE, (uint64_t)physicalMemory);
    setPageTableFlag(&PDE, Present, true);
    setPageTableFlag(&PDE, ReadWrite, true);
    PT->entries[index.P_i] = PDE;
}
