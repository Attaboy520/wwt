#include <graphics.h>
#include <font.h>
#include <uart.h>
#include <type.h>
#include <string.h>

void initializeGraphics(IN KernelBootVideoModeInfo* _bootVideoModeInfo) {
    bootVideoInfo = _bootVideoModeInfo;
}

bool getResolution(OUT uint32_t* horizontal, OUT uint32_t* vertical) {
    if (!bootVideoInfo)
        return false;
    *horizontal = bootVideoInfo->horizontalResolution;
    *vertical = bootVideoInfo->verticalResolution;

    return true;
}

void drawRect(IN const uint16_t x, IN const uint16_t y, IN const uint16_t width, IN const uint16_t height, IN const uint32_t color) {
    if (bootVideoInfo)
    {
        uint32_t* at;
        uint16_t row = 0;
        uint16_t col = 0;

        for (row = 0; row < height; row++) {
            for (col =0; col < width; col++) {
                at = bootVideoInfo->frameBufferPointer + x + col;
                at += ((y + row) * bootVideoInfo->pixelsPerScanline);
                *at = color;
            }
        }
    }
}

void print(IN const char* chr) {
    if (!chr) {
        uartPuts("chr invalid parame\n");
        return;
    }
    char* c = (char*)chr;
    while (*c != 0)
    {
        if (*c == '\n')
            next();
        else 
            putChar(*c);
        ++c;
    }
}

void putChar(IN char chr) {
    put(chr, font.cursorPosition.x, font.cursorPosition.y);
    font.cursorPosition.x += 8;
    if (font.cursorPosition.x + 8 > bootVideoInfo->horizontalResolution) {
        font.cursorPosition.x = 0;
        font.cursorPosition.y += 16;
    }
}

void put(IN char chr, IN uint32_t xOffset, IN uint32_t yOffset) {
    uint32_t* pixelPtr = (uint32_t*)bootVideoInfo->frameBufferPointer;
    char* fontPtr = (char*)font.psf1Font->glyphBuffer + (chr * font.psf1Font->psf1Header->charSize);
    for (uint64_t y = yOffset; y < yOffset + 16; ++y) {
        for (uint32_t x = xOffset; x < xOffset + 8; ++x) {
            if ((*fontPtr & (0b10000000 >> (x - xOffset))) > 0) {
                *(uint32_t*)(pixelPtr + x + (y * bootVideoInfo->pixelsPerScanline)) = font.color;
            }
        }
        ++fontPtr;
    }
}

void next(void) {
    font.cursorPosition.x = 0;
    font.cursorPosition.y += 16;
}

void clear(void) {
    // TODO: have bug
    // uint64_t bytesPerScanline = bootVideoInfo->pixelsPerScanline * 4;
    // uint64_t pixelPtrBase;
    // for (uint64_t verticalScanline = 0; verticalScanline < bootVideoInfo->verticalResolution; ++verticalScanline) {
    //     pixelPtrBase = bootVideoInfo->verticalResolution + (bytesPerScanline * verticalScanline);
    //     for (uint64_t* pixelPtr = (uint64_t*)pixelPtrBase; pixelPtr < (uint64_t*)(pixelPtrBase + bytesPerScanline); ++pixelPtr) {
    //         *pixelPtr = font.clearColor;
    //     }
    // }
    
    // font.cursorPosition.x = 0;
    // font.cursorPosition.y = 0;
}

void debugPrint(IN char* fmt, ...)
{
	va_list args;
	char outputMessage[64] = {0};
	va_start(args, fmt);
    vsprint(outputMessage, fmt, args);
    print(outputMessage);
	va_end(args);
};