#include <gdt.h>

void _x86_64_asm_lgdt(void *gdtr, uint64_t cs_idx, uint64_t ds_idx);
void _x86_64_asm_ltr(uint64_t tss_idx);

static uint64_t gdt[MAX_GDT] = {
  0, // null descriptor
  GDT_CS | P | DPL0 | L, // kernel code
  GDT_DS | P | W | DPL0, // kernel data
  0, // user data (32-bit)
  GDT_DS | P | W | DPL3, // user data
  GDT_CS | P | DPL3 | L, // user code (64-bit)
  0, 0 // tss
};

static GDTR gdtr = {sizeof(gdt), (uint64_t)gdt};
static TSS tss;

void initializeGDT(void) {
  SystemSegmentDescriptor* ssd = (SystemSegmentDescriptor*)&gdt[6];
  ssd->lsbLimit = sizeof(TSS) - 1;
  ssd->lsbBase = ((uint64_t)&tss);
  ssd->type = 9;
  ssd->dpl = 0;
  ssd->present = 1;
  ssd->msbLimit = 0;
  ssd->gran = 0;
  ssd->msbBase = ((uint64_t)&tss) >> 24;

  _x86_64_asm_lgdt((void*)&gdtr, 8, 16);
  _x86_64_asm_ltr(0x30);
}