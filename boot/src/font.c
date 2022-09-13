#include <font.h>
#include <file.h>
#include <error.h>
#include <memory.h>
#include <boot.h>

#define KERNEL_PATH L"\\zap-light16.psf"

PSF1Font* loadPSD1Font(VOID) {
    EFI_STATUS status;
    EFI_FILE* font;
    status = openFile(KERNEL_PATH, &font);
    if (!font || EFI_ERROR(status))
        return NULL;

    PSF1Header* fontHeader;
    status = allocateMemory(EfiLoaderData, sizeof(PSF1Header), (VOID**)&fontHeader);
    if (EFI_ERROR(status))
    {
        debugPrint(L"Error: allocating psf1 header pool. %s\n", getEFIErrorMessage(status));
        return NULL;
    }

    UINTN size = sizeof(PSF1Header);
    status = readFile(font, &size, fontHeader);
    if (EFI_ERROR(status))
    {
        debugPrint(L"Error: read psf1 file.\n");
        return NULL;
    }

    if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1)
    {
        debugPrint(L"Error: magic is not emp PSF1_MAGIC0\n");
        return NULL;
    }
    
    UINTN glyphBufferSize = fontHeader->charSize * 256;
    if (fontHeader->mode == 1) // 512 字形模式
        glyphBufferSize = fontHeader->charSize * 512;
    
    void* glyphBuffer;
    status = setPosition(font, sizeof(PSF1Header));
    if (EFI_ERROR(status))
    {
        debugPrint(L"Error: psf1 header set position.\n");
        return NULL;
    }

    status = allocateMemory(EfiLoaderData, glyphBufferSize, (VOID**)&glyphBuffer);
    if (EFI_ERROR(status))
    {
        debugPrint(L"Error: allocating glyphbuffer.\n");
        return NULL;
    }

    status = readFile(font, &glyphBufferSize, glyphBuffer);
    if (EFI_ERROR(status))
    {
        debugPrint(L"Error: read glyphbuffer.\n");
        return NULL;
    }

    PSF1Font* finishedFont;

    status = allocateMemory(EfiLoaderData, sizeof(PSF1Font), (VOID**)&finishedFont);
    if (EFI_ERROR(status))
    {
        debugPrint(L"Error: allocating psf1font.\n");
        return NULL;
    }
    closeFile(font);
    finishedFont->psf1Header = fontHeader;
    finishedFont->glyphBuffer = glyphBuffer;
    return finishedFont;
}