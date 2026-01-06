#include "Errors.h"
#include "Str.h"

//#################################################################################################
ERRCODE ConvertFromNativeErrorCode(const ERRCODE nNativeErrorCode) noexcept
{
	ERRCODE nErrorCode;

	switch(nNativeErrorCode)
	{
	case ERROR_ACCESS_DENIED:
	case ERROR_CANT_ACCESS_FILE:
		nErrorCode = FW_ERROR_ACCESS_DENIED;
		break;

	case ERROR_ALREADY_EXISTS:
		nErrorCode = FW_ERROR_EXISTS;
		break;

	case ERROR_ENCRYPTION_FAILED:
		nErrorCode = FW_ERROR_ENCRYPTION_FAILED;
		break;

	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	case ERROR_BAD_PATHNAME:
		nErrorCode = FW_ERROR_NOT_FOUND;
		break;

	case ERROR_HANDLE_EOF:
		nErrorCode = FW_ERROR_HANDLE_EOF;
		break;

	case ERROR_INSUFFICIENT_BUFFER:
		nErrorCode = FW_ERROR_INSUFFICIENT_BUFFER;
		break;

	case ERROR_INVALID_HANDLE:
		nErrorCode = FW_ERROR_INVALID_HANDLE;
		break;

	case ERROR_INVALID_PARAMETER:
	case ERROR_INVALID_USER_BUFFER:
	case ERROR_INVALID_NAME:
		nErrorCode = FW_ERROR_INVALID_PARAMETER;
		break;

	case ERROR_IO_PENDING:
		nErrorCode = FW_ERROR_IO_PENDING;
		break;

	case ERROR_MORE_DATA:
		nErrorCode = FW_ERROR_MORE_DATA;
		break;

	case ERROR_NO_MORE_ITEMS:
		nErrorCode = FW_ERROR_NO_MORE_DATA;
		break;

	case ERROR_NOT_ENOUGH_MEMORY:
		nErrorCode = FW_ERROR_NOT_ENOUGH_MEMORY;
		break;

	case ERROR_NOT_ENOUGH_QUOTA:
		nErrorCode = FW_ERROR_QUOTA;
		break;

	case ERROR_OPERATION_ABORTED:
		nErrorCode = FW_ERROR_ABORTED;
		break;

	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
		nErrorCode = FW_ERROR_BUSY;
		break;

	case ERROR_SUCCESS:
		nErrorCode = FW_NO_ERROR;
		break;

	case ERROR_FILENAME_EXCED_RANGE:
		nErrorCode = FW_ERROR_TOO_LARGE;
		break;

	case ERROR_DIR_NOT_EMPTY:
		nErrorCode = FW_ERROR_NOT_EMPTY;
		break;

	default:
		nErrorCode = FW_ERROR_UNKNOWN;
#ifdef _DEBUG
		CStr(CStr::EFT_Format, L"Unknown error code {0}", nNativeErrorCode).DebugPrint();
#endif
		break;
	}

	return nErrorCode;
}
