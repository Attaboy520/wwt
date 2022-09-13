#include <font.h>

void initializeFont(IN PSF1Font* _font) {
    font.psf1Font = _font;
    font.clearColor = 0x00000000;
    font.color = 0xffffffff;
    font.cursorPosition.x = 0;
    font.cursorPosition.y = 0;
}