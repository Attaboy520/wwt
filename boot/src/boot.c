#include <boot.h>
#include <error.h>

EFI_STATUS debugPrint(IN CHAR16* fmt, ...)
{
	EFI_STATUS status;
	va_list args;
	CHAR16 outputMessage[MAX_SERIAL_OUT_STRING_LENGTH];

	va_start(args, fmt);

	if(serialService.protocol) {
		VSPrint(outputMessage, MAX_SERIAL_OUT_STRING_LENGTH, fmt, args);

		status = printToSerialOut(serialService.protocol, outputMessage);
		if(EFI_ERROR(status)) {
			Print(L"Error: Error printing to serial output: %s\n",
				getEFIErrorMessage(status));

			return status;
		}
	} else {
		VPrint(fmt, args);
	}

	va_end(args);

	return EFI_SUCCESS;
};

EFI_STATUS waitForInput(OUT EFI_INPUT_KEY* key) {
	EFI_STATUS status;
	do {
		status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, key);
	} while(status == EFI_NOT_READY);

	return status;
}