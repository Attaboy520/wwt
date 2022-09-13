#ifndef _KERNEL_INTERRUPTS_H_
#define _KERNEL_INTERRUPTS_H_
#include <stdint.h>

#define DE 0x00 // 0 除零错误
#define DB 0x01 // 调试异常，用于软件调试
#define NMI 0x02 // NMI中断，不可屏蔽的外部中断触发
#define BP 0x03 // 断点，INT 3指令触发
#define OF 0x04 // 溢出， INTO指令触发
#define BR 0x05 // 数组越界，BOUND指令触发
#define UD 0x06 // 无效指令（没有定义的指令）
#define NM 0x07 // 数学协处理器不存在或不可用 浮点或WAIT/FWAIT指令触发
#define DF 0x08 // 双重错误，由任何可能产生异常的指令、不可屏蔽的中断和可屏蔽的中断触发
#define TS 0x0a // 10 无效TSS，任务切换或访问TSS触发
#define NP 0x0b // 11 段不存在，加载段寄存器或访问系统段触发
#define SS 0x0c // 12 栈段错误，栈操作或加载SS寄存器触发
#define GP 0x0d // 13 一般保护异常，如果一个操作违反了保护模式下的规定，而且该情况不属于其他异常，CPU就认为是该异常。任何内存引用或保护性检查触发
#define PF 0x0e // 14 页错误，任何内存引用触发。
#define MF 0x10 // 16 浮点错误，浮点或WAIT/FWAIT指令触发
#define AC 0x11 // 17 对齐检查，对内存中数据的引用触发
#define MC 0x12 // 18 机器检查，错误代码和来源与型号有关触发
#define XF 0x13 // 19 SIMD浮点异常，SIMD浮点指令付出
/***************** 主片 ********************/
#define IRQ0 0x20 // 时钟中断 Programmable Interrupt Timer Interrupt
#define IRQ1 0x21 // 键盘中断 Keyboard Interrupt
#define IRQ2 0x22 // 级联 Cascade (used internally by the two PICs. never raised)
#define IRQ3 0x23 // 串口2 COM2 (if enabled)
#define IRQ4 0x24 // 串口1 COM1 (if enabled)
#define IRQ5 0x25 // 并口2 LPT2 (if enabled)
#define IRQ6 0x26 // 软盘 Floppy Disk
#define IRQ7 0x27 // 并口1 LPT1 / Unreliable "spurious" interrupt (usually)
/***************** 从片 ********************/
#define IRQ8 0x28 // 实时时钟 CMOS real-time clock (if enabled)
#define IRQ9 0x29 // 重定向 Free for peripherals / legacy SCSI / NIC
#define IRQ10 0x2a // Free for peripherals / SCSI / NIC
#define IRQ11 0x2b // Free for peripherals / SCSI / NIC
#define IRQ12 0x2c // PS2 鼠标 PS2 Mouse
#define IRQ13 0x2d // fpu 浮点单元异常 FPU / Coprocessor / Inter-processor
#define IRQ14 0x2e // 主硬盘 Primary ATA Hard Disk
#define IRQ15 0x2f // 次硬盘 Secondary ATA Hard Disk

#define SOFT_INTR 0x80

#define INTERRUPT_GATE    0x8e
#define TRAP_GATE         0x8f

#define PIC1 0x20		/* IO base address for master PIC */
#define PIC2 0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28
#define PIC_2_END PIC_2_OFFSET + 7

#define PIC_1_COMMAND_PORT 0x20
#define PIC_2_COMMAND_PORT 0xA0
#define PIC_ACKNOWLEDGE 0x20

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

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
void initializePIC(uint8_t offset1, uint8_t offset2);

struct InterruptFrame;
__attribute__((interrupt)) void divideZeroHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void debugHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void NMIHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void breakpointHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void overflowHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void boundRangeHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void undefinedHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void NMHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void doubleFaultHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void tssHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void NPHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void stackSegmentHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void generalPageHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void pageFaultHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void MFHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void alignmentCheckHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void machineCheckHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void XFHandler(struct InterruptFrame* frame);

__attribute__((interrupt)) void timerHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void keyboardHandler(struct InterruptFrame* frame);
__attribute__((interrupt)) void mouseHandler(struct InterruptFrame* frame);


#endif /* _KERNEL_INTERRUPTS_H_ */