#ifndef MARSOHOD_ERROR
#define MARSOHOD_ERROR

#include <stdio.h>
#include <Windows.h>

#define LOG_MESSAGE(...) {wprintf(__VA_ARGS__);}

class CMarsohodError
{
public:
	CMarsohodError(void);
	~CMarsohodError(void);
	static LPCWSTR ErrorMessage(DWORD error);
	static void LogError(LPCWSTR error_string, char *filename, int lineno, DWORD error);
};

#endif
