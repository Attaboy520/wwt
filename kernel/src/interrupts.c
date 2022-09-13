#include <interrupts.h>
#include <graphics.h>
#include <io.h>

#define INTERRUPTS_DESCRIPTOR_COUNT 256
IDT idt[INTERRUPTS_DESCRIPTOR_COUNT];
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
    idtr.limit = sizeof(IDT) * INTERRUPTS_DESCRIPTOR_COUNT;
    idtr.offset = (uint64_t)idt;

    setIRQ(DE, (void*)divideZeroHandler, TRAP_GATE, 0x08);
    setIRQ(DB, (void*)debugHandler, TRAP_GATE, 0x08);
    setIRQ(NMI, (void*)NMIHandler, TRAP_GATE, 0x08);
    setIRQ(BP, (void*)breakpointHandler, TRAP_GATE, 0x08);
    setIRQ(OF, (void*)overflowHandler, TRAP_GATE, 0x08);
    setIRQ(BR, (void*)boundRangeHandler, TRAP_GATE, 0x08);
    setIRQ(UD, (void*)undefinedHandler, TRAP_GATE, 0x08);
    setIRQ(NM, (void*)NMHandler, TRAP_GATE, 0x08);
    setIRQ(DF, (void*)doubleFaultHandler, TRAP_GATE, 0x08);
    setIRQ(TS, (void*)tssHandler, TRAP_GATE, 0x08);
    setIRQ(NP, (void*)NPHandler, TRAP_GATE, 0x08);
    setIRQ(SS, (void*)stackSegmentHandler, TRAP_GATE, 0x08);
    setIRQ(GP, (void*)generalPageHandler, TRAP_GATE, 0x08);
    setIRQ(PF, (void*)pageFaultHandler, TRAP_GATE, 0x08);
    setIRQ(MF, (void*)MFHandler, TRAP_GATE, 0x08);
    setIRQ(AC, (void*)alignmentCheckHandler, TRAP_GATE, 0x08);
    setIRQ(MC, (void*)machineCheckHandler, TRAP_GATE, 0x08);
    setIRQ(XF, (void*)XFHandler, TRAP_GATE, 0x08);

    setIRQ(IRQ0, (void*)timerHandler, INTERRUPT_GATE, 0x08);
    setIRQ(IRQ1, (void*)keyboardHandler, INTERRUPT_GATE, 0x08);
    asm ("lidt %0" : : "m" (idtr));
    asm ("sti");
    // asm ("cli");
    initializePIC(PIC_1_OFFSET, PIC_2_OFFSET);
}

void setIRQ(uint8_t iqrn, void* handler, uint8_t typeAttributes, uint8_t selector) {
    setIDTOffset(&idt[iqrn], (uint64_t)handler);
    idt[iqrn].type = typeAttributes;
    idt[iqrn].selector = selector;
}

void initializePIC(uint8_t offset1, uint8_t offset2) {
    	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	ioWait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	ioWait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	ioWait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	ioWait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	ioWait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	ioWait();
 
	outb(PIC1_DATA, ICW4_8086);
	ioWait();
	outb(PIC2_DATA, ICW4_8086);
	ioWait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

void picEnd(void){
    outb(PIC_1_COMMAND_PORT, PIC_ACKNOWLEDGE);
    outb(PIC_2_COMMAND_PORT, PIC_ACKNOWLEDGE);
}

__attribute__((interrupt)) void divideZeroHandler(struct InterruptFrame* frame) {
    debugPrint("divide zero");
    while(1);
}

__attribute__((interrupt)) void debugHandler(struct InterruptFrame* frame) {
    debugPrint("debug");
    while(1);
}

__attribute__((interrupt)) void NMIHandler(struct InterruptFrame* frame) {
    debugPrint("NMI");
    while(1);
}

__attribute__((interrupt)) void breakpointHandler(struct InterruptFrame* frame) {
    debugPrint("breakpoint");
    while(1);
}

__attribute__((interrupt)) void overflowHandler(struct InterruptFrame* frame) {
    debugPrint("overflow");
    while(1);
}

__attribute__((interrupt)) void boundRangeHandler(struct InterruptFrame* frame) {
    debugPrint("bound range");
    while(1);
}

__attribute__((interrupt)) void undefinedHandler(struct InterruptFrame* frame) {
    debugPrint("undefined");
    while(1);
}

__attribute__((interrupt)) void NMHandler(struct InterruptFrame* frame) {
    debugPrint("NM");
    while(1);
}

__attribute__((interrupt)) void doubleFaultHandler(struct InterruptFrame* frame) {
    debugPrint("double fault");
    while(1);
}

__attribute__((interrupt)) void tssHandler(struct InterruptFrame* frame) {
    debugPrint("tss");
    while(1);
}

__attribute__((interrupt)) void NPHandler(struct InterruptFrame* frame) {
    debugPrint("NP");
    while(1);
}

__attribute__((interrupt)) void stackSegmentHandler(struct InterruptFrame* frame) {
    debugPrint("stack segment");
    while(1);
}

__attribute__((interrupt)) void generalPageHandler(struct InterruptFrame* frame) {
    debugPrint("general page");
    while(1);
}

__attribute__((interrupt)) void pageFaultHandler(struct InterruptFrame* frame) {
    debugPrint("page fault");
    while(1);
}

__attribute__((interrupt)) void MFHandler(struct InterruptFrame* frame) {
    debugPrint("MF");
    while(1);
}

__attribute__((interrupt)) void alignmentCheckHandler(struct InterruptFrame* frame) {
    debugPrint("alignment check");
    while(1);
}

__attribute__((interrupt)) void machineCheckHandler(struct InterruptFrame* frame) {
    debugPrint("machine check");
    while(1);
}

__attribute__((interrupt)) void XFHandler(struct InterruptFrame* frame) {
    debugPrint("XF");
    while(1);
}

__attribute__((interrupt)) void timerHandler(struct InterruptFrame* frame) {
    debugPrint("timer");
    // picEnd();
    while(1);
}

__attribute__((interrupt)) void keyboardHandler(struct InterruptFrame* frame) {
    debugPrint("keyboard\n");
    picEnd();
}

__attribute__((interrupt)) void mouseHandler(struct InterruptFrame* frame) {
    debugPrint("mouse");
    while(1);
}
