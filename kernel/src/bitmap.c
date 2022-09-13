#include <bitmap.h>

bool getBitmap(Bitmap* bit, uint64_t index) {
    if (index > bit->size * 8) return false;
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    if ((bit->bits[byteIndex] & bitIndexer) > 0){
        return true;
    }
    return false;
}

bool setBitmap(Bitmap* bit, uint64_t index, bool value) {
    if (index > bit->size * 8) return false;
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    bit->bits[byteIndex] &= ~bitIndexer;
    if (value){
        bit->bits[byteIndex] |= bitIndexer;
    }
    return true;
}