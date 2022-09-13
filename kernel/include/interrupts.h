#ifndef _KERNEL_INTERRUPTS_H_
#define _KERNEL_INTERRUPTS_H_
#include <stdint.h>

#define PAGE_FAULT 0xE
#define SOFT_INTR 0x80
#define DOUBLE_PAGE_FAULT 0x08

/***************** 主片 ********************/
#define IRQ0 0x20 // 时钟中断
#define IRQ1 0x21 // 键盘中断
#define IRQ2 0x22 // 级联
#define IRQ3 0x23 // 串口2
#define IRQ4 0x24 // 串口1
#define IRQ5 0x25 // 串口2
#define IRQ6 0x26 // 软盘
#define IRQ7 0x27 // 并口1
/***************** 从片 ********************/
#define IRQ8 0x28 // 实时时钟
#define IRQ9 0x29 // 重定向
#define IRQ10 0x2a // 保留
#define IRQ11 0x2b // 保留
#define IRQ12 0x2c // ps/2 鼠标
#define IRQ13 0x2d // fpu 浮点单元异常
#define IRQ14 0x2e // 硬盘
#define IRQ15 0x2f // 保留

#define IDT_InterruptGate    0x8e
#define IDT_CallGate         0x8c
#define IDT_TrapGate         0x8f

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

typedef struct _idtr {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed)) IDTR;

typedef struct _idt {
    uint16_t offset0;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t reserved;
} __attribute__((packed)) IDT;

void setIDTOffset(IDT* _idt, uint64_t offset);
uint64_t getIDTOffset(IDT* _idt);
void initializeInterrupts(void);
void setIRQ(uint8_t iqrn, void* handler, uint8_t typeAttributes, uint8_t selector);
void initializePIC(void);

struct InterruptFrame;
__attribute__((interrupt)) void timerHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void pageFaultHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void doubleFaultHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void GPFaultHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void keyboardHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void mouseHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void pitHandler(struct InterruptFrame* frame);

#endif /* _KERNEL_INTERRUPTS_H_ */