#ifndef _BOOT_MAIN_H_
#define _BOOT_MAIN_H_

#include <efi.h>
#include <efilib.h>

#define PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR 1

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE* SystemTable);

#endif /* _BOOT_MAIN_H_ */
