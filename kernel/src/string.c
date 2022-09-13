#include <string.h>
#include <type.h>
char* itoa(int value, char* str, int radix);
char* uitoa(uint32_t value, char* str, int radix);
char* gcvt(double value, int ndigit, char* buf);

size_t strlen(char* const str)
{
	size_t len = 0;
	while(str[len]) {
		len++;
	}

	return len;
}

char *strcpy(char* dest, const char* src) {
    char *addr = dest;

    while (*src){
        *dest++ = *src++;
    }
    *dest = '\0';
    return addr;
}

char *strcat(char* dest,const char* src) {
    char *addr = dest;

    while (*dest) dest++;
    do *dest++ = *src++; while (*src);
    return addr;
}

char* itoa(int value, char* str, int radix) {
    char reverse[36];   
    char *p = reverse;
    bool sign = (value >= 0) ? true : false;

    value = (value >= 0)?value:-value;
    *p++ = '\0';
    if (radix == 2) {
        *p++ = 'B';
    } else if (radix == 8) {
        *p++ = 'O';
    } else if (radix == 16) {
        *p++ = 'H';
    }
    while (value >= 0){
        *p++ = "0123456789abcdef"[value % radix];
        value /= radix;
        if (value == 0) break;
    }

    if (!sign) {
        *p = '-';
    }
    else {
        p--;
    }

    if (radix == 2) {
        *str++ = '0';
        *str++ = 'b';
    } else if (radix == 8) {
        *str++ = '0';
    } else if (radix == 16) {
        *str++ = '0';
        *str++ = 'x';
    }

    while (p >= reverse){
        *str++ = *p--;
    }

    return str;
}

char* uitoa(uint32_t value, char* str, int radix) {
    char reverse[36];   
    char *p = reverse;

    *p++ = '\0';
    while (value != 0){
        *p++ = "0123456789abcdef"[value % radix];
        value /= radix;
        if (value == 0) break;
    }
    p--;

    while (p >= reverse){
        *str++ = *p--;
    }

    return str;
}

char* gcvt(double value, int ndigit, char* buf) {
    char tmpbuf[72];
    int int_part = (int)value;

    double folat_part = value - int_part;

    if (folat_part < 0) folat_part = -folat_part;
    itoa(int_part, tmpbuf, 10);

    char *p = tmpbuf;

     while (*p != '\0') p++;

    *p++ = '.';

    while (ndigit > 0 && folat_part > 0.00000001){
        *p++ = (int)(folat_part * 10) + '0';
        folat_part = (folat_part * 10) - (int)(folat_part * 10);
        ndigit--;
    }

    *p = '\0';
    strcpy(buf, tmpbuf);
    return buf;
}

void vsprint(char* buf, const char* fmt, va_list args) {
    char *p;
    for (p = buf; *fmt; fmt++){
        if (*fmt != '%'){
            *p++ = *fmt;
            continue;
        }
        fmt++;
        switch (*fmt) {
            case 'd':
                itoa(va_arg(args, int), p, 10);
                p += strlen(p);
                break;
            case 'x':
                uitoa(va_arg(args, unsigned int), p, 16);
                p += strlen(p);
                break;
            case 'c':
            // https://blog.csdn.net/qq_39892503/article/details/107343443
                *p++ = (char)va_arg(args, int);
                break;
            case 's':
                *p = '\0';
                strcat(p, va_arg(args, char *));
                p += strlen(p);
                break;
            case 'f':
                gcvt(va_arg(args, double), 6, p);
                p += strlen(p);
                break;
            case 'b':
                itoa(va_arg(args, int), p, 2);
                p += strlen(p);
                break;
            case 'h':
                itoa(va_arg(args, int), p, 16);
                p += strlen(p);
                break;
            default:
                break;
        }
    }
    *p = '\0';
}

char* itos(int64_t value, char* str) {
    return itoa(value, str, 10);
}

char* uitos(uint64_t value, char* str) {
    return uitoa(value, str, 10);
}

char* itoh(int64_t value, char* str) {
    return itoa(value, str, 2);
}
char* uitoh(uint64_t value, char* str) {
    return uitoa(value, str, 2);
}

char* dtos(double value, char* str) {
    return gcvt(value, 6, str);
}