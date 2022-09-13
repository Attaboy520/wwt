#include <stdarg.h>

#include <boot.h>
#include <error.h>
#include <serial.h>

EFI_STATUS configureSerialProtocol(IN EFI_SERIAL_IO_PROTOCOL* const protocol)
{
	EFI_STATUS status;

	#ifdef DEBUG
		debugPrint(L"Debug: Configuring serial IO protocol\n");
	#endif

	status = uefi_call_wrapper(protocol->SetAttributes, 7,
		protocol, 0, 0, 0, 0, 0, DefaultStopBits);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error configuring Serial Protocol: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	return EFI_SUCCESS;
}


EFI_STATUS initSerialService(VOID)
{
	EFI_STATUS status;

	#ifdef DEBUG
		debugPrint(L"Debug: Initialising Serial service\n");
	#endif

	status = uefi_call_wrapper(gBS->LocateProtocol, 3,
		&gEfiSerialIoProtocolGuid, NULL, &serialService.protocol);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error locating Serial Protocol: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Located Serial Protocol\n");
	#endif

	status = configureSerialProtocol(serialService.protocol);
	if(EFI_ERROR(status)) {
		return status;
	}

	return EFI_SUCCESS;
}


EFI_STATUS printToSerialOut(IN EFI_SERIAL_IO_PROTOCOL* const protocol,
	IN CHAR16* line)
{
	EFI_STATUS status;
	UINTN bufferSize = 0;
	UINTN lineLength = 0;

	if(protocol == NULL ||
		line == NULL) {
		return EFI_INVALID_PARAMETER;
	}

	lineLength = StrLen(line);

	if(lineLength > MAX_SERIAL_OUT_STRING_LENGTH) {
		return EFI_BAD_BUFFER_SIZE;
	}

	if(lineLength == 0) {
		return EFI_INVALID_PARAMETER;
	}

	bufferSize = lineLength * 2;

	status = uefi_call_wrapper(protocol->Write, 3,
		protocol, &bufferSize, (VOID*)line);
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: Error writing to serial protocol: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	if(bufferSize != (lineLength * 2)) {
		debugPrint(L"Error: Full string not printed to serial protocol\n");
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;
}
