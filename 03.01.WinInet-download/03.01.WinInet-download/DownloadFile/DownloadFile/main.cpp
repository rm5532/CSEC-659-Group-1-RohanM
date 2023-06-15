#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "wininet.lib")

BOOL http_request(LPCSTR host, WORD port, BOOL secure, LPCSTR verb, LPCSTR path, LPCSTR szHeaders, SIZE_T nHeaderSize,
    LPCSTR postData, SIZE_T nPostDataSize, char **data, LPDWORD dwDataSize)
{
    HINTERNET hIntSession = NULL;
    HINTERNET hHttpSession = NULL;
    HINTERNET hHttpRequest = NULL;
    DWORD dwFlags = 0;

    BOOL ret = FALSE;
	#define UA "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"

    do
    {
        // TODO 1
		hIntSession = InternetOpenA(UA, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, dwFlags);
        if (hIntSession == NULL)
        {
            break;
        }

        // TODO 2
        hHttpSession = InternetConnectA(hIntSession, host, port, NULL, NULL, INTERNET_SERVICE_HTTP, dwFlags, (DWORD_PTR)NULL);
        if (hHttpSession == NULL)
        {
            break;
        }
        
        dwFlags = INTERNET_FLAG_RELOAD
            | INTERNET_FLAG_DONT_CACHE
            | INTERNET_FLAG_NO_UI
            | INTERNET_FLAG_KEEP_CONNECTION;

        if (secure)
        {
            dwFlags |= INTERNET_FLAG_SECURE;
        }

        // TODO 3
		hHttpRequest = HttpOpenRequestA(hHttpSession, verb, path, 0, 0, 0, dwFlags, (DWORD_PTR)NULL);
        if (hHttpRequest == NULL)
        {
            break;
        }
        
        if (secure)
        {
            auto certOptions = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
                SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
                SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                SECURITY_FLAG_IGNORE_REVOCATION;
            auto modSucess = InternetSetOptionA(hHttpRequest, INTERNET_OPTION_SECURITY_FLAGS, &certOptions, sizeof(certOptions));
        }

        if (!HttpSendRequestA(hHttpRequest, szHeaders, (DWORD)nHeaderSize, (LPVOID)postData, (DWORD)nPostDataSize))
        {
            break;
        }
        
        CHAR szBuffer[1024];
        CHAR *output = (CHAR*)malloc(1024);
        DWORD dwRead = 0;
        DWORD dwTotalBytes = 0;

        memset(output, 0, 1024);
        memset(szBuffer, 0, sizeof(szBuffer));

        while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
        {
            DWORD dwOffset = dwTotalBytes;
            dwTotalBytes += dwRead;

            output = (CHAR*)realloc(output, dwTotalBytes);
            memcpy(output + dwOffset, szBuffer, dwRead);

            memset(szBuffer, 0, sizeof(szBuffer));
            dwRead = 0;
        }
        output[dwTotalBytes] = '\0';
        *data = output;
        *dwDataSize = dwTotalBytes;

        ret = TRUE;
    } while (0);

    if (hHttpRequest)
    {
        InternetCloseHandle(hHttpRequest);
    }
    
    if (hHttpSession)
    {
        InternetCloseHandle(hHttpSession);
    }
    
    if (hIntSession)
    {
        InternetCloseHandle(hIntSession);
    }
    
    return ret;
}

int main(int argc, char* argv[]) 
{
    //std::cout << argc << std::endl;

    char *data = NULL;
    DWORD dataSize = 0;

    http_request("127.0.0.1", 5000, FALSE, "GET", "/5MB.zip", NULL, 0, NULL, 0, &data, &dataSize);
    //printf("%s", data);
    //system("PAUSE");

    std::string content_to_write(data, dataSize);

#if _DEBUG
    argc = 2;
    
    TCHAR buffer[MAX_PATH] = { 0 };
    DWORD result = GetCurrentDirectoryA(MAX_PATH, buffer);
    if (result == 0)
        return 1;

    std::string file_to_write(buffer,result);
    file_to_write.append("\\5MB-new.zip");
    std::cout << file_to_write.c_str() << std::endl;
#else
    if (argc == 1)
    {
        printf("Invalid output location\n");
        return 1;
    }

    std::string file_to_write(argv[1]);
#endif
    
    
    DWORD wSize = 0;

    char expanded2[MAX_PATH] = { 0 };
    ExpandEnvironmentStringsA(file_to_write.c_str(), expanded2, MAX_PATH);

    HANDLE hFile = CreateFileA(expanded2, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "failed INVALID_HANDLE_VALUE ... \n";
        return 1;
    }

    if (!WriteFile(hFile, content_to_write.data(), (DWORD)content_to_write.size(), &wSize, NULL))
    {
        std::cout << "failed writefile ... \n";
        return 1;
    }

    CloseHandle(hFile);

    if (wSize == content_to_write.size()) 
    {
        return 0;
    }
    else 
    {
        std::cout << "FAILED to write... \n";
        return 1;
    }

    return 0;
}