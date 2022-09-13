#ifndef _BOOT_SERIAL_H_
#define _BOOT_SERIAL_H_

#include <efi.h>
#include <efilib.h>

#define MAX_SERIAL_OUT_STRING_LENGTH 256

typedef struct _uefiSerialService {
	EFI_SERIAL_IO_PROTOCOL* protocol;
} UEFSerialService;

EFI_STATUS configureSerialProtocol(IN EFI_SERIAL_IO_PROTOCOL* const protocol);

EFI_STATUS initSerialService(VOID);

EFI_STATUS printToSerialOut(IN EFI_SERIAL_IO_PROTOCOL* const protocol,
	IN CHAR16* line);

UEFSerialService serialService;
#endif /* _BOOT_SERIAL_H_ */
