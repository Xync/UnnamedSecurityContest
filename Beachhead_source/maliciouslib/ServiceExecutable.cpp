
#include <stdio.h>
#include <windows.h>
#include "MyService.h"

//HOW TO COMPILE:
//gcc -Wno-write-strings ServiceExecutable.cpp MyService.cpp -o serviceExecutable.exe -lstdc++


//
// Settings of the service
//

// Internal name of the service
#define SERVICE_NAME             L"DisplayManagerService"

// Displayed name of the service
#define SERVICE_DISPLAY_NAME     L"Windows Monitoring Instrumentation"

// Service start options.
#define SERVICE_START_TYPE       SERVICE_AUTO_START

// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     L""

// The name of the account under which the service should run
#define SERVICE_ACCOUNT          L"NT AUTHORITY\\SYSTEM"

// The password to the service account name
#define SERVICE_PASSWORD         NULL

void InstallService(PWSTR pszServiceName,
                    PWSTR pszDisplayName,
                    DWORD dwStartType,
                    PWSTR pszDependencies,
                    PWSTR pszAccount,
                    PWSTR pszPassword)
{
    wchar_t szPath[MAX_PATH];
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;


    wprintf(L"the potential length is %d\n", wcslen(szPath));
    if (GetModuleFileNameW(NULL, szPath, MAX_PATH) == 0)
    {
        wprintf(L"GetModuleFileName failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }
    wprintf(L"The pathname is %s\n", szPath);

    // Open the local default service control manager database
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT |
        SC_MANAGER_CREATE_SERVICE);
    if (schSCManager == NULL)
    {
        wprintf(L"OpenSCManager failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }

    // Install the service into SCM by calling CreateService
    schService = CreateServiceW(
        schSCManager,                   // SCManager database
        pszServiceName,                 // Name of service
        pszDisplayName,                 // Name to display
        SERVICE_QUERY_STATUS,           // Desired access
        SERVICE_WIN32_OWN_PROCESS,      // Service type
        dwStartType,                    // Service start type
        SERVICE_ERROR_NORMAL,           // Error control type
        szPath,                         // Service's binary
        NULL,                           // No load ordering group
        NULL,                           // No tag identifier
        pszDependencies,                // Dependencies
        pszAccount,                     // Service running account
        pszPassword                     // Password of the account
        );
    if (schService == NULL)
    {
        wprintf(L"CreateService failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }

    wprintf(L"%s is installed.\n", pszServiceName);

Cleanup:
    // Centralized cleanup for all allocated resources.
    if (schSCManager)
    {
        CloseServiceHandle(schSCManager);
        schSCManager = NULL;
    }
    if (schService)
    {
        CloseServiceHandle(schService);
        schService = NULL;
    }
}


void UninstallService(PWSTR pszServiceName)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;
    SERVICE_STATUS ssSvcStatus = {};

    // Open the local default service control manager database
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (schSCManager == NULL)
    {
        wprintf(L"OpenSCManager failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }

    // Open the service with delete, stop, and query status permissions
    schService = OpenServiceW(schSCManager, pszServiceName, SERVICE_STOP |
        SERVICE_QUERY_STATUS | DELETE);
    if (schService == NULL)
    {
        wprintf(L"OpenService failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }

    // Try to stop the service
    if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
    {
        wprintf(L"Stopping %s.", pszServiceName);
        Sleep(1000);

        while (QueryServiceStatus(schService, &ssSvcStatus))
        {
            if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
            {
                wprintf(L".");
                Sleep(1000);
            }
            else break;
        }

        if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
        {
            wprintf(L"\n%s is stopped.\n", pszServiceName);
        }
        else
        {
            wprintf(L"\n%s failed to stop.\n", pszServiceName);
        }
    }

    // Now remove the service by calling DeleteService.
    if (!DeleteService(schService))
    {
        wprintf(L"DeleteService failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }

    wprintf(L"%s is removed.\n", pszServiceName);

Cleanup:
    // Centralized cleanup for all allocated resources.
    if (schSCManager)
    {
        CloseServiceHandle(schSCManager);
        schSCManager = NULL;
    }
    if (schService)
    {
        CloseServiceHandle(schService);
        schService = NULL;
    }
}


//int WinMain(int argc, wchar_t **)
//int WinMain(HINSTANCE,HINSTANCE,LPSTR argv,int argc)
int WinMain(HINSTANCE,HINSTANCE, LPSTR lpcmdline,int argc)
{


    wprintf (L"The second char is %c\n", lpcmdline[1]);

    if ((argc > 1) && (lpcmdline[0] == L'-') || (lpcmdline[0] == L'/'))
    {
        wprintf(L"commandlinearg is %S\n",lpcmdline);
        if (strcmp(lpcmdline, "-install") == 0)
//        if (_wcsicmp(L"-install", (wchar_t *)lpcmdline) == 0)
        {
            wprintf(L"Made it into install\n");
            // Install the service when the command is
            // "-install" or "/install".
            InstallService(
                SERVICE_NAME,               // Name of service
                SERVICE_DISPLAY_NAME,       // Name to display
                SERVICE_START_TYPE,         // Service start type
                SERVICE_DEPENDENCIES,       // Dependencies
                SERVICE_ACCOUNT,            // Service running account
                SERVICE_PASSWORD            // Password of the account
                );
        }
        else if (_wcsicmp(L"remove", (wchar_t *)(&lpcmdline + 1)) == 0)
        {
            // Uninstall the service when the command is
            // "-remove" or "/remove".
            UninstallService(SERVICE_NAME);
        }
    }
    else
    {
        wprintf(L"Parameters:\n");
        wprintf(L" -install  to install the service.\n");
        wprintf(L" -remove   to remove the service.\n");

        MyService service(SERVICE_NAME);
        if (!MyService::Run(service))
        {
            wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
        }
    }

    return 0;
}

