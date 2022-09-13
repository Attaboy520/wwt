#include <io.h>

uint8_t inb(IN uint16_t port)
{
	uint8_t ret;
	asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(port));
	return ret;
}


void outb(IN uint16_t port, IN uint8_t val)
{
	asm volatile("out %%al, %%dx" : : "a"(val), "d"(port));
}

void ioWait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

uint16_t inw(IN uint16_t port) {
	uint16_t ret;
	asm volatile("in %%dx, %%ax" : "=a"(ret) : "d"(port));
	return ret;
}

void outw(IN uint16_t port, IN uint16_t val) {
	asm volatile("out %%ax, %%dx": : "a"(val), "d"(port));
}
