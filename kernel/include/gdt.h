#ifndef _KERNEL_GDT_H_
#define _KERNEL_GDT_H_

#include <stdint.h>

#define GDT_SIZE 6

typedef struct _gdt {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t limitHighFlag;
    uint8_t baseHigh;
} __attribute__((packed)) GDT;

typedef struct _gdtr {
    uint16_t limit;
    GDT *offset;
} __attribute__((packed)) GDTR;

void initializeGDT(void);


#endif
