#ifndef _BOOT_GRAPHICS_H_
#define _BOOT_GRAPHICS_H_

#include <efi.h>
#include <efilib.h>

typedef struct _uefiGraphicsService {
	EFI_HANDLE* handleBuffer;
	UINTN handleCount;
} UEFIGraphicsService;

VOID drawRect(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT16 _x,
	IN const UINT16 _y,
	IN const UINT16 _width,
	IN const UINT16 _height,
	IN const UINT32 color);

VOID drawTestScreen(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol);

EFI_STATUS closeGraphicOutputService(VOID);

EFI_STATUS initGraphicsOutputService(VOID);
EFI_STATUS openGraphicsProtocol(IN EFI_HANDLE* ImageHandle);
EFI_STATUS findVideoMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol, IN const UINT32 width, IN const UINT32 height,
 	IN const EFI_GRAPHICS_PIXEL_FORMAT pixelFormat, OUT UINTN* videoMode);
EFI_STATUS setGraphicsMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 width,
	IN const UINT32 height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT pixelFormat);

UEFIGraphicsService graphicsService;
EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOutputProtocol;
#endif /* _BOOT_GRAPHICS_H_ */
