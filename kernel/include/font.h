#ifndef _KERNEL_FONT_H_
#define _KERNEL_FONT_H_

#include <math.h>
#include <stdint.h>
#include <type.h>

typedef struct _psf1Header{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charSize;
} PSF1Header;

typedef struct _psf1Font{
	PSF1Header* psf1Header;
	void* glyphBuffer;
} PSF1Font;

typedef struct _font {
	PSF1Font* psf1Font;
	Point cursorPosition;
	uint32_t color;
	uint32_t clearColor;
} Font;

Font font;
void initializeFont(IN PSF1Font* _font);


#endif /* _KERNEL_FONT_H_ */