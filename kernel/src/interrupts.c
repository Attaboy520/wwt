#include <interrupts.h>
#include <graphics.h>
#include <io.h>

IDT idt[256];
IDTR idtr;

void setIDTOffset(IDT* _idt, uint64_t offset) {
    _idt->offset0 = (uint16_t)(offset & 0x000000000000ffff);
    _idt->offset1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    _idt->offset2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

uint64_t getIDTOffset(IDT* _idt) {
    uint64_t offset = 0;
    offset |= (uint64_t)_idt->offset0;
    offset |= (uint64_t)_idt->offset1 << 16;
    offset |= (uint64_t)_idt->offset2 << 32;
    return offset;
}

void initializeInterrupts(void) {
    idtr.limit = sizeof(IDT) * 256 - 1;
    idtr.offset = (uint64_t)idt;

    setIRQ(PAGE_FAULT, (void*)pageFaultHandler, IDT_InterruptGate, 0x08);
    setIRQ(IRQ0, (void*)timerHandler, IDT_InterruptGate, 0x08);
    setIRQ(IRQ1, (void*)keyboardHandler, IDT_InterruptGate, 0x08);
    asm ("lidt %0" : : "m" (idtr));
    initializePIC();
}

void setIRQ(uint8_t iqrn, void* handler, uint8_t typeAttributes, uint8_t selector) {
    setIDTOffset(&idt[iqrn], (uint64_t)handler);
    idt[iqrn].type = typeAttributes;
    idt[iqrn].selector = selector;
}

void initializePIC(void) {
    uint8_t a1, a2;
    a1 = inb(PIC1_DATA);
    ioWait();
    a2 = inb(PIC2_DATA);
    ioWait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    ioWait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    ioWait;

    outb(PIC1_DATA, 0x20);
    ioWait();
    outb(PIC2_DATA, 0x28);
    ioWait();

    outb(PIC1_DATA, 4);
    ioWait();
    outb(PIC2_DATA, 2);
    ioWait();

    outb(PIC1_DATA, ICW4_8086);
    ioWait();
    outb(PIC2_DATA, ICW4_8086);
    ioWait();

    outb(PIC1_DATA, a1);
    ioWait();
    outb(PIC2_DATA, a2);
}

void picEnd(void){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

__attribute__((interrupt)) void timerHandler(struct InterruptFrame* frame) {
    debugPrint("time");
    picEnd();
}

__attribute__((interrupt)) void pageFaultHandler(struct InterruptFrame* frame) {
    debugPrint("Page Fault Detected");
    while(1);
}

__attribute__((interrupt)) void doubleFaultHandler(struct InterruptFrame* frame) {

}

__attribute__((interrupt)) void GPFaultHandler(struct InterruptFrame* frame) {

}

__attribute__((interrupt)) void keyboardHandler(struct InterruptFrame* frame) {
    debugPrint("keyboard");
    picEnd();
}

__attribute__((interrupt)) void mouseHandler(struct InterruptFrame* frame) {

}

__attribute__((interrupt)) void pitHandler(struct InterruptFrame* frame) {

}
