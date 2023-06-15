#include <Windows.h>
#include <iostream>

void printError(DWORD dwResult)
{
	// Get the error message for the last error code
	LPSTR lpMsgBuf;
	DWORD dwMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf,
		0, 
		NULL
	);

	// Print the error message
	if (dwMsgLen > 0)
	{
		printf("Error: %s\n", lpMsgBuf);
	}
	else
	{
		printf("Unknown error: %lu\n", dwResult);
	}

	// Free the buffer allocated by FormatMessage
	LocalFree(lpMsgBuf);
}

int main()
{
	// CreateDirectoryA will fail
	if (!CreateDirectoryA("",NULL))
	{
		// capture the error message in a DWORD
		DWORD dwError = GetLastError();
		// print the error message to the screen
		printError(dwError);
		return -1;
	}

	return 0;
}