#ifndef _KERNEL_BITMAP_H_
#define _KERNEL_BITMAP_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct bitmap
{
    size_t size;   // 位图字节长度(注意是字节长度)
    uint8_t* bits;             // 位图的起始地址(字节)
} Bitmap;

bool getBitmap(Bitmap* bit, uint64_t index);
bool setBitmap(Bitmap* bit, uint64_t index, bool value);

#endif /* _KERNEL_BITMAP_H_ */