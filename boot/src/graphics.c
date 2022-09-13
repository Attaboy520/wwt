#include <graphics.h>
#include <error.h>
#include <boot.h>

#define TEST_SCREEN_COL_NUM             4
#define TEST_SCREEN_ROW_NUM             3
#define TEST_SCREEN_TOTAL_TILES         TEST_SCREEN_COL_NUM * TEST_SCREEN_ROW_NUM
#define TEST_SCREEN_PRIMARY_COLOUR      0x00FF4000
#define TEST_SCREEN_SECONDARY_COLOUR    0x00FF80BF

EFI_STATUS closeGraphicOutputService()
{
	EFI_STATUS status = uefi_call_wrapper(gBS->FreePool, 1,
		graphicsService.handleBuffer);

	return status;
}


VOID drawRect(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT16 _x,
	IN const UINT16 _y,
	IN const UINT16 width,
	IN const UINT16 height,
	IN const UINT32 color)
{
	UINT32* at;

	UINT16 row = 0;
	UINT16 col = 0;
	for(row = 0; row < height; row++) {
		for(col = 0; col < width; col++) {
			at = (UINT32*)protocol->Mode->FrameBufferBase + _x + col;
			at += ((_y + row) * protocol->Mode->Info->PixelsPerScanLine);

			*at = color;
		}
	}
}

VOID drawTestScreen(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol)
{
	const UINT16 tileWidth = protocol->Mode->Info->HorizontalResolution /
		TEST_SCREEN_COL_NUM;
	const UINT16 tileHeight = protocol->Mode->Info->VerticalResolution /
		TEST_SCREEN_ROW_NUM;

	UINT8 p = 0;
	for(p = 0; p < TEST_SCREEN_TOTAL_TILES; p++) {
		UINT8 _x = p % TEST_SCREEN_COL_NUM;
		UINT8 _y = p / TEST_SCREEN_COL_NUM;

		UINT32 color = TEST_SCREEN_PRIMARY_COLOUR;
		if(((_y % 2) + _x) % 2) {
			color = TEST_SCREEN_SECONDARY_COLOUR;
		}

		drawRect(protocol, tileWidth * _x, tileHeight * _y,
			tileWidth, tileHeight, color);
	}
}

EFI_STATUS findVideoMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 width,
	IN const UINT32 height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT pixelFormat,
	OUT UINTN* videoMode)
{
	EFI_STATUS status;
	UINTN modeInfoSize;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* modeInfo;

	UINTN i = 0;
	for(i = 0; i < protocol->Mode->MaxMode; i++) {
		#ifdef DEBUG
			debugPrint(L"Debug: Testing video mode: '%llu'\n", i);
		#endif

		status = uefi_call_wrapper(protocol->QueryMode, 4,
			protocol, i, &modeInfoSize, &modeInfo);
		if(EFI_ERROR(status)) {
			debugPrint(L"Error: Error querying video mode: %s\n",
				getEFIErrorMessage(status));

			return status;
		}

		if(modeInfo->HorizontalResolution == width &&
			modeInfo->VerticalResolution == height &&
			modeInfo->PixelFormat == pixelFormat) {

			#ifdef DEBUG
				debugPrint(L"Debug: Matched video mode: '%llu' for '%lu*%lu*%u'\n", i,
					width, height, pixelFormat);
			#endif

			*videoMode = i;
			return EFI_SUCCESS;
		}
	}

	return EFI_UNSUPPORTED;
}


EFI_STATUS initGraphicsOutputService(VOID)
{
	EFI_STATUS status;

	#ifdef DEBUG
		debugPrint(L"Debug: Initialising Graphics Output Service\n");
	#endif

	status = uefi_call_wrapper(gBS->LocateHandleBuffer, 5,
		ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL,
		&graphicsService.handleCount, &graphicsService.handleBuffer);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error locating GOP handle buffer: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Located GOP handle buffer with %u handles\n",
			graphicsService.handleCount);
	#endif

	return EFI_SUCCESS;
}

EFI_STATUS openGraphicsProtocol(IN EFI_HANDLE* ImageHandle) {
    if (!gBS || !ST || !ImageHandle ) {
        debugPrint(L"Error: invalid paramerter\n");
        return EFI_INVALID_PARAMETER;
    }

    return uefi_call_wrapper(gBS->OpenProtocol, 6,
    ST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, 
    &graphicsOutputProtocol, *ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

EFI_STATUS setGraphicsMode(IN EFI_GRAPHICS_OUTPUT_PROTOCOL* const protocol,
	IN const UINT32 width,
	IN const UINT32 height,
	IN const EFI_GRAPHICS_PIXEL_FORMAT pixelFormat)
{
	EFI_STATUS status;
	UINTN graphics_mode_num = 0;

	status = findVideoMode(protocol, width, height,
		pixelFormat, &graphics_mode_num);
	if(EFI_ERROR(status)) {
		return status;
	}

	status = uefi_call_wrapper(protocol->SetMode, 2,
		protocol, graphics_mode_num);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error setting graphics mode: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	return EFI_SUCCESS;
}
