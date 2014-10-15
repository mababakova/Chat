// ChatServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Configuration.h"
#include "Log.h"
#include "SocketManager.h"

wchar_t *serviceName;
SERVICE_TABLE_ENTRYW ServiceTable[1];
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;

void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	case SERVICE_CONTROL_SHUTDOWN:

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	default:
		break;
	}

	SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

void ServiceMain(int argc, char** argv)
{
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandlerW(serviceName, (LPHANDLER_FUNCTION)ControlHandler);

	if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);



	while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		Sleep(1000);
	}
}

int main(int argc, char* argv[])
{
	// Load configuration
	std::string path(argv[0]);
	unsigned found = path.find_last_of("/\\");
	path = path.substr(0, found + 1);
	Configuration *configuration = Configuration::getInstance(path);
	Log *loggr = Log::getInstance();
	SocketManager *sock = new SocketManager();

	serviceName = L"ChatServer";
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	ServiceTable[0].lpServiceName = serviceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONW)ServiceMain;
	StartServiceCtrlDispatcherW(&ServiceTable[0]);

	return 0;
}

