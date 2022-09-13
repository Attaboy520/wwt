#ifndef _BOOT_ERROR_H_
#define _BOOT_ERROR_H_

#include <efi.h>
#include <efilib.h>
#include <stdbool.h>

const CHAR16* getEFIErrorMessage(IN EFI_STATUS const status);

bool checkForError(IN EFI_STATUS const status,
	IN const CHAR16* errorMessage);

#endif /* _BOOT_ERROR_H_ */
