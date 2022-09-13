#include <stdarg.h>

#include <main.h>
#include <boot.h>
#include <elf.h>
#include <error.h>
#include <file.h>
#include <graphics.h>
#include <serial.h>
#include <loader.h>
#include <memory.h>
#include <font.h>

#define TARGET_SCREEN_WIDTH     1024
#define TARGET_SCREEN_HEIGHT    768
#define TARGET_PIXEL_FORMAT     PixelBlueGreenRedReserved8BitPerColor

#define DRAW_TEST_SCREEN 0

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_STATUS status;
	EFI_INPUT_KEY inputKey;
	EFI_PHYSICAL_ADDRESS* kernelEntryPtr = NULL;
	void (*kernelEntry)(KernelBootInfo* bootInfo);
	KernelBootInfo bootInfo;
	EFI_MEMORY_DESCRIPTOR* memoryMap = NULL;
	UINTN memoryMapKey = 0;
	UINTN memoryMapSize = 0;
	UINTN descriptorSize;
	UINT32 descriptorVersion;

	InitializeLib(ImageHandle, SystemTable);

	// 关闭看门狗
	status = uefi_call_wrapper(gBS->SetWatchdogTimer, 4, 0, 0, 0, NULL);
	if(checkForError(status, L"Error setting watchdog timer")) {
		return status;
	}

	status = uefi_call_wrapper(ST->ConIn->Reset, 2, SystemTable->ConIn, FALSE);
	if(checkForError(status, L"Error resetting console input")) {
		return status;
	}

	// 初始化串口服务
	status = initSerialService();
	if(EFI_ERROR(status)) {
		if(status == EFI_NOT_FOUND) {
			#ifdef DEBUG
				debugPrint(L"Debug: No serial device found\n");
			#endif
		} else {
			debugPrint(L"Fatal Error: Error initialising Serial IO service\n");

			#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
				debugPrint(L"Press any key to reboot...");
				waitForInput(&inputKey);
			#endif

			return status;
		}
	}

	// 初始化图形输出服务
	status = initGraphicsOutputService();
	if(EFI_ERROR(status)) {
		if(status == EFI_NOT_FOUND) {
			#ifdef DEBUG
				debugPrint(L"Debug: No graphics device found\n");
			#endif
		} else {
			debugPrint(L"Fatal Error: Error initialising Graphics service\n");

			#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
				debugPrint(L"Press any key to reboot...");
				waitForInput(&inputKey);
			#endif

			return status;
		}
	}

	// 打开图形协议
	status = openGraphicsProtocol(ImageHandle);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Failed to open the graphics output protocol on "
			L"the active console output device: %s\n", getEFIErrorMessage(status));
	}

	if(graphicsOutputProtocol) {
		// 设置图形模式
		status = setGraphicsMode(graphicsOutputProtocol, TARGET_SCREEN_WIDTH,
			TARGET_SCREEN_HEIGHT, TARGET_PIXEL_FORMAT);
		if(EFI_ERROR(status)) {
			return status;
		}

		#if DRAW_TEST_SCREEN
			drawTestScreen(graphicsOutputProtocol);
		#endif
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Initialising File System service\n");
	#endif
	// 初始化文件系统
	status = initFileSystemService();
	if(EFI_ERROR(status)) {
		return status;
	}

	// 打开根目录
	status = openVolume();
	if(checkForError(status, L"Error opening root volume")) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Loading Kernel image\n");
	#endif

	// 加载内核文件
	status = loadKernelImage(kernelEntryPtr);
	if(EFI_ERROR(status)) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Set Kernel Entry Point to: '0x%llx'\n",
			*kernelEntryPtr);
	#endif

	bootInfo.videoModeInfo.frameBufferPointer =
		(VOID*)graphicsOutputProtocol->Mode->FrameBufferBase;
	bootInfo.videoModeInfo.horizontalResolution =
		graphicsOutputProtocol->Mode->Info->HorizontalResolution;
	bootInfo.videoModeInfo.frameBufferSize = graphicsOutputProtocol->Mode->FrameBufferSize;
	bootInfo.videoModeInfo.verticalResolution =
		graphicsOutputProtocol->Mode->Info->VerticalResolution;
	bootInfo.videoModeInfo.pixelsPerScanline =
		graphicsOutputProtocol->Mode->Info->PixelsPerScanLine;

	// UINT32 max_mode = graphicsOutputProtocol->Mode->MaxMode;
	// debugPrint(L"Current mode: %d, max mode: %d\n", graphicsOutputProtocol->Mode->Mode, max_mode);
	// uefi_call_wrapper(graphicsOutputProtocol->SetMode, 2, graphicsOutputProtocol, max_mode / 2);
	// max_mode = graphicsOutputProtocol->Mode->MaxMode;
	// debugPrint(L"Current mode: %d, max mode: %d\n", graphicsOutputProtocol->Mode->Mode, max_mode);
	// UINTN modeInfoSize;
	// EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* modeInfo;
	// uefi_call_wrapper(graphicsOutputProtocol->QueryMode, 4, graphicsOutputProtocol, graphicsOutputProtocol->Mode->Mode, &modeInfoSize, &modeInfo);
	// debugPrint(L"Debug: Matched video mode: '%llu' for '%lu*%lu*%u'\n", graphicsOutputProtocol->Mode->Mode, modeInfo->HorizontalResolution, modeInfo->VerticalResolution, modeInfo->PixelFormat);
	
	#ifdef DEBUG
		debugPrint(L"Debug: Closing Graphics Output Service handles\n");
	#endif

	// 关闭图形服务
	status = closeGraphicOutputService();
	if(checkForError(status, L"Error closing Graphics Output service")) {
		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Loading PSD1 font\n");
	#endif

	// 加载字体
	PSF1Font* font = loadPSD1Font();
	// PSF1Font* font = NULL;
	if (!font) {
		debugPrint(L"Error: font is not valid or is not found\n");
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Getting memory map and exiting boot services\n");
	#endif

	// 获取内存映射
	status = getMemoryMap((VOID**)&memoryMap, &memoryMapSize,
		&memoryMapKey, &descriptorSize, &descriptorVersion);
	if(EFI_ERROR(status)) {
		return status;
	}
	
	#ifdef DEBUG
		debugPrint(L"Debug: Exiting boot services %d  %lx\n",memoryMapKey, &ImageHandle);
	#endif
	// 退出服务
	status = uefi_call_wrapper(gBS->ExitBootServices, 2,
		ImageHandle, memoryMapKey);
	if(checkForError(status, L"exiting boot services")) {
		return status;
	}

	bootInfo.memoryInfo.memoryMap = memoryMap;
	bootInfo.memoryInfo.mapSize = memoryMapSize;
	bootInfo.memoryInfo.mapDescriptorSize = descriptorSize;
	bootInfo.psf1Font = font;

	kernelEntry = (void (*)(KernelBootInfo*))*kernelEntryPtr;
	kernelEntry(&bootInfo);

	return EFI_LOAD_ERROR;
}



