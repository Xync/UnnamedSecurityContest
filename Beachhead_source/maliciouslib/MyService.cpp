
#include "MyService.h"
#include <stdio.h>
#include <sys/stat.h>

// REM cl /nologo /GS- /DNDEBUG /DPAYLOAD_GLOBAL /TcMyService.cpp /link /OUT:DisplayManager.exe /SUBSYSTEM:CONSOLE /MACHINE:x64
// REM cl /nologo /GS- /DNDEBUG /DPAYLOAD_GLOBAL /TcServiceExecutable.cpp /link /OUT:ServiceExecutable.exe /SUBSYSTEM:CONSOLE /MACHINE:x64

MyService *MyService::s_service = NULL;

MyService::MyService(PWSTR pszServiceName,
					 BOOL fCanStop,
					 BOOL fCanShutdown,
					 BOOL fCanPauseContinue)
{
	if (pszServiceName == NULL)
	{
		m_name = L"";
	}
	else
	{
		m_name = pszServiceName;
	}

	m_statusHandle = (SERVICE_STATUS_HANDLE)NULL;
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_status.dwCurrentState = SERVICE_START_PENDING;

	DWORD dwControlsAccepted = 0;
	if (fCanStop)
		dwControlsAccepted |= SERVICE_ACCEPT_STOP;
	if (fCanShutdown)
		dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
	if (fCanPauseContinue)
		dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;

	m_status.dwControlsAccepted = dwControlsAccepted;

	m_status.dwWin32ExitCode = NO_ERROR;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;
}

MyService::~MyService()
{
}

BOOL MyService::Run(MyService &service)
{
	s_service = &service;
	SERVICE_TABLE_ENTRYW serviceTable[] =
		{
			{service.m_name, ServiceMain},
			{NULL, NULL}};

	return StartServiceCtrlDispatcherW(serviceTable);
}

void WINAPI MyService::ServiceMain(DWORD dwArgc, PWSTR *pszArgv)
{
	s_service->m_statusHandle = RegisterServiceCtrlHandlerW(
		s_service->m_name, ServiceCtrlHandler);
	if (s_service->m_statusHandle == (SERVICE_STATUS_HANDLE)NULL)
	{
		throw GetLastError();
	}

	s_service->Start(dwArgc, pszArgv);
}

void WINAPI MyService::ServiceCtrlHandler(DWORD dwCtrl)
{
	switch (dwCtrl)
	{
	case SERVICE_CONTROL_STOP:
		s_service->Stop();
		break;
	case SERVICE_CONTROL_PAUSE:
		s_service->Pause();
		break;
	case SERVICE_CONTROL_CONTINUE:
		s_service->Continue();
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		s_service->Shutdown();
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
}

void MyService::Start(DWORD dwArgc, PWSTR *pszArgv)
{
	try
	{
		SetServiceStatus(SERVICE_START_PENDING);
		OnStart(dwArgc, pszArgv);
		SetServiceStatus(SERVICE_RUNNING);
	}
	catch (DWORD dwError)
	{
		WriteEventLogEntry(L"Service failed to start.", EVENTLOG_ERROR_TYPE);
		SetServiceStatus(SERVICE_STOPPED);
	}
}

void MyService::Stop()
{
	DWORD dwOriginalState = m_status.dwCurrentState;

	try
	{
		SetServiceStatus(SERVICE_STOP_PENDING);
		OnStop();
		SetServiceStatus(SERVICE_STOPPED);
	}
	catch (DWORD dwError)
	{
		WriteErrorLogEntry(L"Service Stop", dwError);
		SetServiceStatus(dwOriginalState);
	}
	catch (...)
	{
		WriteEventLogEntry(L"Service failed to stop.", EVENTLOG_ERROR_TYPE);
		SetServiceStatus(dwOriginalState);
	}
}

void MyService::Pause()
{
	try
	{
		SetServiceStatus(SERVICE_PAUSE_PENDING);
		OnPause();
		SetServiceStatus(SERVICE_PAUSED);
	}
	catch (DWORD dwError)
	{
		WriteErrorLogEntry(L"Service Pause", dwError);
		SetServiceStatus(SERVICE_RUNNING);
	}
	catch (...)
	{
		WriteEventLogEntry(L"Service failed to pause.", EVENTLOG_ERROR_TYPE);
		SetServiceStatus(SERVICE_RUNNING);
	}
}

void MyService::Continue()
{
	try
	{
		SetServiceStatus(SERVICE_CONTINUE_PENDING);
		OnContinue();
		SetServiceStatus(SERVICE_RUNNING);
	}
	catch (DWORD dwError)
	{
		WriteErrorLogEntry(L"Service Continue", dwError);
		SetServiceStatus(SERVICE_PAUSED);
	}
	catch (...)
	{
		WriteEventLogEntry(L"Service failed to resume.", EVENTLOG_ERROR_TYPE);
		SetServiceStatus(SERVICE_PAUSED);
	}
}

void MyService::Shutdown()
{
	try
	{
		OnShutdown();
		SetServiceStatus(SERVICE_STOPPED);
	}
	catch (DWORD dwError)
	{
		WriteErrorLogEntry(L"Service Shutdown", dwError);
	}
	catch (...)
	{
		WriteEventLogEntry(L"Service failed to shut down.", EVENTLOG_ERROR_TYPE);
	}
}

void MyService::SetServiceStatus(DWORD dwCurrentState,
								 DWORD dwWin32ExitCode,
								 DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;

	// Fill in the SERVICE_STATUS structure of the service.

	m_status.dwCurrentState = dwCurrentState;
	m_status.dwWin32ExitCode = dwWin32ExitCode;
	m_status.dwWaitHint = dwWaitHint;

	m_status.dwCheckPoint =
		((dwCurrentState == SERVICE_RUNNING) ||
		 (dwCurrentState == SERVICE_STOPPED))
			? 0
			: dwCheckPoint++;

	// Report the status of the service to the SCM.
	::SetServiceStatus(m_statusHandle, &m_status);
}

void MyService::WriteEventLogEntry(PWSTR pszMessage, WORD wType)
{
	return;
	//    HANDLE hEventSource = NULL;
	//    LPCWSTR lpszStrings[2] = { NULL, NULL };

	//  hEventSource = RegisterEventSourceW(NULL, m_name);
	//  if (hEventSource)
	//  {
	//      lpszStrings[0] = m_name;
	//      lpszStrings[1] = pszMessage;

	//     ReportEvent(hEventSource,  // Event log handle
	//          wType,                 // Event type
	//          0,                     // Event category
	//          0,                     // Event identifier
	//          NULL,                  // No security identifier
	//          2,                     // Size of lpszStrings array
	//          0,                     // No binary data
	//          lpszStrings,           // Array of strings
	//          NULL                   // No binary data
	//          );

	//     DeregisterEventSource(hEventSource);
	// }
}

void MyService::WriteErrorLogEntry(PWSTR pszFunction, DWORD dwError)
{
	return;
	wchar_t szMessage[260];
	//    StringCchPrintf(szMessage, ARRAYSIZE(szMessage),
	//        L"%s failed w/err 0x%08lx", pszFunction, dwError);
	WriteEventLogEntry(szMessage, EVENTLOG_ERROR_TYPE);
}

#define TIMER_VALUE 5000

extern "C" {
	void setmyhostname();
	void check_time_tasks();
	void check_cron_tasks();
	void check_dns_tasks();
	void check_temp_tasks();
}


DWORD WINAPI threadMain(LPVOID param)
{
    setmyhostname();
	
    while (true)
    {
        check_temp_tasks();
        check_time_tasks();
        check_cron_tasks();
        check_dns_tasks();
        Sleep(TIMER_VALUE);
    }
}

void MyService::OnStart(DWORD dwArgc, PWSTR *pszArgv)
{
	CreateThread(NULL, 0, threadMain, NULL, 0, NULL);
}

void MyService::OnStop()
{
}

void MyService::OnPause()
{
}

void MyService::OnContinue()
{
}

void MyService::OnShutdown()
{
}

//int main(int argc, char* argv[]) {
//	return 0;
//}