#define no_init_all
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>

// https://docs.microsoft.com/en-us/windows/win32/procthread/creating-processes
// small changes
int main(int argc, CHAR* argv[])
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
#ifdef _DEBUG
    // using cmd /c [cmd] to prove it allows for arg passing.
	LPSTR cmd = (LPSTR)"cmd /c notepad";
#else
    if (argc != 2)
    {
        printf("Usage: CreateProcess.exe [cmd w/ args]\n");
        return 0;
    }

    LPSTR cmd = argv[1];
#endif

    // Start the child process. 
    if (!CreateProcessA(NULL,   // No module name (use command line)
        cmd,            // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)            // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return 0;
}