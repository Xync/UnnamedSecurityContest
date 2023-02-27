

#pragma once
#include <windows.h>

class MyService {
public:
	static BOOL Run(MyService &service);
	
	MyService(PWSTR pszServiceName,
		BOOL fCanStop=TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue =FALSE);

	~MyService(void);

	void Stop();

protected:
	void OnStart(DWORD dwARgc, PWSTR *pszArgv);
	void OnStop();
	void OnPause();
	void OnContinue();
	void OnShutdown();
	void SetServiceStatus(DWORD dwCurrentState,
			DWORD dwWin32ExitCode = NO_ERROR,
			DWORD dwWaitHint = 0);
	void WriteEventLogEntry(PWSTR pszMessage, WORD wType);
	void WriteErrorLogEntry(PWSTR pszFunction,
			DWORD dwError = GetLastError());

private:
	static void WINAPI ServiceMain(DWORD dwArgc, LPWSTR *lpszArgv);
	static void WINAPI ServiceCtrlHandler(DWORD dwCtrl);
	void Start(DWORD dwargc, PWSTR *pszArgv);
	void Pause();
	void Continue();
	void Shutdown();

	static MyService *s_service;
	PWSTR m_name;
	SERVICE_STATUS m_status;
	SERVICE_STATUS_HANDLE m_statusHandle;
};
