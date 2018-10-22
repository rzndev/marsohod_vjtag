#include "MarsohodError.h"

CMarsohodError::CMarsohodError(void)
{
}


CMarsohodError::~CMarsohodError(void)
{
}


LPCWSTR CMarsohodError::ErrorMessage(DWORD error)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);
	return LPCWSTR(lpMsgBuf);
}


void CMarsohodError::LogError(LPCWSTR error_string, char *filename, int lineno, DWORD error)
{
	LPCWSTR str = ErrorMessage(error);
	wprintf(L"Error in %S:%d: %d - %s\n%s", filename, lineno, error, error_string, str);
}

