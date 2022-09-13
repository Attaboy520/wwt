#ifndef _BOOT_FILE_H_
#define _BOOT_FILE_H_

#include <efi.h>
#include <efilib.h>
#include <stdint.h>


typedef struct _uefiSimpleFileSystemService {
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* protocol;
} UEFIFileSystemService;

EFI_STATUS initFileSystemService(VOID);
EFI_STATUS openVolume(VOID);
EFI_STATUS openFile(IN CHAR16* const fileName, OUT EFI_FILE** file);
EFI_STATUS readFile(IN EFI_FILE* const file, IN OUT UINTN* bufferSize, OUT VOID* buffer);
EFI_STATUS setPosition(IN EFI_FILE* const file, IN UINT64 const offset);
EFI_STATUS closeFile(IN EFI_FILE* const file);

EFI_FILE* rootHandle;
UEFIFileSystemService fileSystemService;
#endif /* _BOOT_FILE_H_ */
