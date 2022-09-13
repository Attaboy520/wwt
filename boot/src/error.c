#include <error.h>
#include <boot.h>

CHAR16 errorMessageBuffer[256];


const CHAR16* getEFIErrorMessage(IN EFI_STATUS const status)
{
	StatusToString(errorMessageBuffer, status);
	return errorMessageBuffer;
}

bool checkForError(IN EFI_STATUS const status,
	IN const CHAR16* errorMessage)
{
	if(EFI_ERROR(status)) {
		debugPrint(L"Error: %s: %s\n", errorMessage,
			getEFIErrorMessage(status));

		#if PROMPT_FOR_INPUT_BEFORE_REBOOT_ON_FATAL_ERROR
			EFI_INPUT_KEY inputKey;
			debugPrint(L"Press any key to reboot...");
			waitForInput(&inputKey);
		#endif

		return TRUE;
	}

	return FALSE;
}
