#include <io.h>

uint8_t inb(IN uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}


void outb(IN uint16_t port, IN uint8_t val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void ioWait(void) {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}