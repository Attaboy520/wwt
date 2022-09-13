#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <io.h>
#include <string.h>
#include <uart.h>

#define UART_PORT_COM1 0x3f8

void uartInitialize(void)
{
	outb(UART_PORT_COM1 + 1, 0x00);    // Disable all interrupts
	outb(UART_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(UART_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(UART_PORT_COM1 + 1, 0x00);    //                  (hi byte)
	outb(UART_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(UART_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(UART_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

bool uartIsRecieveBufferEmpty(void)
{
	return inb(UART_PORT_COM1 + 5) & 1;
}

char uartGetChar(void)
{
	while(!uartIsRecieveBufferEmpty());

	return inb(UART_PORT_COM1);
}

bool uartIsTransmitBufferEmpty(void)
{
	return (inb(UART_PORT_COM1 + 5) & 0x20) != 0;
}

void uartPutChar(IN const char a)
{
	while(!uartIsTransmitBufferEmpty());

	outb(UART_PORT_COM1, a);
}

void uartPuts(IN char* const str)
{
	size_t strLen = strlen(str);
	for(size_t i = 0; i < strLen; i++) {
		uartPutChar(str[i]);
	}
}

void uartPutInt(IN const uint64_t value) {
	char a[256];
	uitos(value, a);
	uartPuts(a);
}