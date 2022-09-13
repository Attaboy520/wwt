#include <gdt.h>

extern void _x86_64_asm_lgdt(GDTR* gdtr);
void setGDT(GDT *offset, uint8_t number, uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity);

GDT gdt[GDT_SIZE];

void initializeGDT(void) {
    GDTR gdtr;
    gdtr.offset = gdt;
    gdtr.limit = sizeof(GDT) * (GDT_SIZE + 1) - 1;

    setGDT(gdtr.offset, 0, 0, 0, 0, 0); // null descriptor
    setGDT(gdtr.offset, 1, 0, 0, 0x9A, 0xA0); // kernel code
    setGDT(gdtr.offset, 2, 0, 0, 0x92, 0xA0); // kernel data
    setGDT(gdtr.offset, 3, 0, 0, 0xF2, 0xA0); // user data
    setGDT(gdtr.offset, 4, 0, 0, 0xFA, 0xA0); // user code

    _x86_64_asm_lgdt((GDTR*)&gdtr);
}

/* limit has to be page aligned */
void setGDT(GDT* offset, uint8_t number, uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    offset[number].baseLow = (base & 0xFFFF);
    offset[number].baseMiddle = (base >> 16) & 0xFF;
    offset[number].baseHigh = (base >> 24) & 0xFF;

    offset[number].limitLow = (limit & 0xFFFF);
    offset[number].limitHighFlag = (limit >> 16) & 0x0F;

    offset[number].limitHighFlag |= granularity & 0xF0;
    offset[number].access = access;
}