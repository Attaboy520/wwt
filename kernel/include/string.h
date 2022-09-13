#ifndef _KERNEL_STRING_H_
#define _KERNEL_STRING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <type.h>

size_t strlen(IN char* const str);
char *strcpy(char* dest, const char* src);
char *strcat(char* dest, const char* src);
void vsprint(char* buf, const char* fmt, va_list args);
char* itos(int64_t value, char* str);
char* uitos(uint64_t value, char* str);
char* itoh(int64_t value, char* str);
char* uitoh(uint64_t value, char* str);
char* dtos(double value, char* str);
#endif /* _KERNEL_STRING_H_ */
