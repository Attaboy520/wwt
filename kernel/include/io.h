#ifndef _KERNEL_IO_H_
#define _KERNEL_IO_H_

#include <stdint.h>
#include <type.h>

uint8_t inb(IN uint16_t port);
void outb(IN uint16_t port, IN uint8_t val);
void ioWait(void);

#endif /* _KERNEL_IO_H_ */
