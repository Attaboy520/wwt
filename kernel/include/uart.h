#ifndef _KERNEL_UART_H_
#define _KERNEL_UART_H_

#include <stdbool.h>
#include <type.h>

void uartInitialize(void);
bool uartIsRecieveBufferEmpty(void);
char uartGetChar(void);
bool uartIsTransmitBufferEmpty(void);
void uartPutChar(IN const char a);
void uartPuts(IN char* const str);
void uartPutInt(IN const uint64_t value);


#endif /* _KERNEL_UART_H_ */
