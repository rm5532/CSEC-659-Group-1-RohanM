#define no_init_all
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory> // std::make_unique
#include <string>
#include <iostream>

// ignore warning for deprecated API call: GetVersionEx
#pragma warning(disable : 4996)

#include <Lm.h> // NetGetJoinInformation
#pragma comment(lib, "Netapi32.lib") // NetGetJoinInformation

// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcomputernamea
std::string get_hostname()
{
	char hostname[255];
    DWORD nameSize = 255;
	if (GetComputerNameA(hostname, &nameSize))
	{
		return std::string(hostname);
	}
	else
	{
		return "";
	}
}

// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea
std::string get_username()
{
	char username[256];
    DWORD nameSize = 255;
	if (GetComputerNameA(username, &nameSize))
	{
		return std::string(username);
	}
	else
	{
		return "";
	}
}

// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocessid
int get_pid()
{
	return GetCurrentProcessId();
}

// NetGetJoinInformation - https://docs.microsoft.com/en-us/windows/win32/api/lmjoin/nf-lmjoin-netgetjoininformation
std::wstring get_wdomain()
{
    LPWSTR lpNameBuffer = NULL;
    BOOL bRet = FALSE;
    NET_API_STATUS nas;
    NETSETUP_JOIN_STATUS joinStatus;

    std::wstring domain = L"WORKGROUP";
    nas = NetGetJoinInformation(NULL, &lpNameBuffer, &joinStatus);
    if (nas != NERR_Success)
        return domain;

    if (joinStatus == NetSetupDomainName)
    {
        // domain joined
        domain = std::wstring(lpNameBuffer);
    }

    NetApiBufferFree(lpNameBuffer);
    return domain;
}

std::string get_domain()
{
    std::wstring wdomain = get_wdomain();
    return std::string(wdomain.begin(), wdomain.end());
}

std::string getuid()
{
    std::string result = "";
    result = get_domain() + "\\" + get_username();
    return result;
}

// GetCurrentThreadToken - https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentthreadtoken
// GetTokenInformation - https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-gettokeninformation
// LookupAccountSidA - https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupaccountsida
/*
std::string getuid()
{
    CHAR username_only[256] = { 0 }, domainname_only[256] = { 0 };
    LPVOID TokenUserInfo[4096];
    DWORD user_length = sizeof(username_only), domain_length = sizeof(domainname_only);
    DWORD sid_type = 0, returned_tokinfo_length;

    std::string result = "";
    if (!GetTokenInformation(GetCurrentThreadToken(), TokenUser, TokenUserInfo, 4096, &returned_tokinfo_length)) {
        return result;
    }

    if (!LookupAccountSidA(NULL, ((TOKEN_USER*)TokenUserInfo)->User.Sid, username_only, &user_length, domainname_only, &domain_length, (PSID_NAME_USE)&sid_type))
        return result;

    result = std::string(domainname_only) + "\\" + std::string(username_only);

    return result;
}
*/

// https://github.com/chromium/chromium/blob/master/base/process/process_info_win.cc
HANDLE CreateCurrentProcessToken() {
    // OpenProcessToken can fail due to permission or allocation failures
    // for the kernel object. Return nullptr in those cases.
    HANDLE process_token = nullptr;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &process_token);
    return process_token;
}

enum IntegrityLevel {
    INTEGRITY_UNKNOWN,
    UNTRUSTED_INTEGRITY,
    LOW_INTEGRITY,
    MEDIUM_INTEGRITY,
    HIGH_INTEGRITY,
    SYSTEM_INTEGRITY // added
};

bool IsCurrentProcessElevated() {

    // Unlike TOKEN_ELEVATION_TYPE which returns TokenElevationTypeDefault when
    // UAC is turned off, TOKEN_ELEVATION returns whether the process is elevated.
    DWORD size;
    TOKEN_ELEVATION elevation;
    if (!GetTokenInformation(CreateCurrentProcessToken(), TokenElevation,
        &elevation, sizeof(elevation), &size)) {
        printf("GetTokenInformation() failed\n");
        return false;
    }
    return !!elevation.TokenIsElevated;
}

IntegrityLevel get_integrity() {
    HANDLE scoped_process_token = CreateCurrentProcessToken();

    DWORD token_info_length = 0;
    if (::GetTokenInformation(scoped_process_token, TokenIntegrityLevel,
        nullptr, 0, &token_info_length) ||
        ::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        return INTEGRITY_UNKNOWN;
    }

    auto token_label_bytes = std::make_unique<char[]>(token_info_length);
    TOKEN_MANDATORY_LABEL* token_label =
        reinterpret_cast<TOKEN_MANDATORY_LABEL*>(token_label_bytes.get());
    if (!::GetTokenInformation(scoped_process_token, TokenIntegrityLevel,
        token_label, token_info_length,
        &token_info_length)) {
        return INTEGRITY_UNKNOWN;
    }
    DWORD integrity_level = *::GetSidSubAuthority(
        token_label->Label.Sid,
        static_cast<DWORD>(*::GetSidSubAuthorityCount(token_label->Label.Sid) -
            1));

    if (integrity_level < SECURITY_MANDATORY_LOW_RID)
        return UNTRUSTED_INTEGRITY;

    if (integrity_level < SECURITY_MANDATORY_MEDIUM_RID)
        return LOW_INTEGRITY;

    if (integrity_level >= SECURITY_MANDATORY_MEDIUM_RID &&
        integrity_level < SECURITY_MANDATORY_HIGH_RID) {
        return MEDIUM_INTEGRITY;
    }

    if (integrity_level >= SECURITY_MANDATORY_HIGH_RID)
        return HIGH_INTEGRITY;
    
    // added
    if (integrity_level >= SECURITY_MANDATORY_SYSTEM_RID)
        return SYSTEM_INTEGRITY;

    return INTEGRITY_UNKNOWN;
}

// https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa
// https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversion
std::string get_os_version()
{
    OSVERSIONINFOA info;
    ZeroMemory(&info, sizeof(OSVERSIONINFOA));
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

    GetVersionExA(&info);
        std::string result = std::to_string(info.dwMajorVersion) + "." + std::to_string(info.dwMinorVersion) + " " +
        "(" + std::to_string(info.dwBuildNumber) + ")";
    return result;
}

std::string get_process_arch()
{
    // Get system architecture
    BOOL bIsWow64 = FALSE;
    IsWow64Process(GetCurrentProcess(), &bIsWow64);
    if (bIsWow64 == TRUE)
        return "SysWoW64";
    else {
        SYSTEM_INFO sysInfo;
        ZeroMemory(&sysInfo, sizeof(SYSTEM_INFO));
        GetNativeSystemInfo(&sysInfo);
        if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
            return "x64";
        else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
            return "x86";
    }
    return "??";
}
std::string get_build_arch()
{
#ifdef _WIN64
    return "x64";
#else
    return "x86";
#endif
}
int main()
{
	//Uncommented code
    std::string hostname = get_hostname();
    std::string username = get_username();
    printf("hostname: %s\n", hostname.c_str());
    printf("username: %s\n", username.c_str());
    

    int integrity = get_integrity();
    printf("integrity: %d\n", integrity);

    int pid = get_pid();
    printf("PID: %d\n", pid);

    std::string arch = get_process_arch();
    printf("ARCH: %s\n", arch.c_str());

    std::string os = get_os_version();
    printf("OS: %s\n", os.c_str());
}