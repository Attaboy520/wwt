#include <file.h>
#include <error.h>
#include <boot.h>

EFI_STATUS initFileSystemService(VOID)
{
	EFI_STATUS status = uefi_call_wrapper(gBS->LocateProtocol, 3,
		&gEfiSimpleFileSystemProtocolGuid, NULL, &fileSystemService.protocol);
	if(EFI_ERROR(status)) {
		debugPrint(L"Fatal Error: Error locating Simple File System Protocol: %s\n",
			getEFIErrorMessage(status));

		return status;
	}

	#ifdef DEBUG
		debugPrint(L"Debug: Located Simple File System Protocol\n");
	#endif

	return status;
}

EFI_STATUS openVolume(VOID) {
    if (!fileSystemService.protocol)
		return EFI_PROTOCOL_ERROR;

	return uefi_call_wrapper(fileSystemService.protocol->OpenVolume, 2,
		fileSystemService.protocol, &rootHandle);
}

EFI_STATUS openFile(IN CHAR16* const fileName, OUT EFI_FILE** file) {
	if (!rootHandle)
	{
		EFI_STATUS status = openVolume();
		if (EFI_ERROR(status))
			return status;
	}
    return uefi_call_wrapper(rootHandle->Open, 5, rootHandle, file, fileName, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
}

EFI_STATUS readFile(IN EFI_FILE* const file, IN OUT UINTN* bufferSize, OUT VOID* buffer) {
    if (!file)
        return EFI_INVALID_PARAMETER;
    return uefi_call_wrapper(file->Read, 3, file, bufferSize, buffer);
}

EFI_STATUS setPosition(IN EFI_FILE* const file, IN UINT64 const offset) {
    if (!file)
        return EFI_INVALID_PARAMETER;
    return uefi_call_wrapper(file->SetPosition, 2, file, offset);
}

EFI_STATUS closeFile(IN EFI_FILE* const file) {
    if (!file)
        return EFI_INVALID_PARAMETER;
    return uefi_call_wrapper(file->Close, 1, file);
}