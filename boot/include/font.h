#ifndef _BOOT_FONT_H_
#define _BOOT_FONT_H_

#include <efi.h>
#include <efilib.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct _psf1Header{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charSize;
} PSF1Header;

typedef struct _psf1Font{
	PSF1Header* psf1Header;
	void* glyphBuffer;
} PSF1Font;

PSF1Font* loadPSD1Font(VOID);

#endif /* _BOOT_FONT_H_ */