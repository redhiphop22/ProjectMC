#include "S2Engine.h"
#include "S2Service.h"

namespace s2 {

void S2Service::Install(TCHAR* path, TCHAR* name, TCHAR* description)
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (0 == hSCManager)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenSCManager failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());
		
		return;
	}

	SC_HANDLE hService = CreateService
	(
		hSCManager,					/* SCManager database      */
		name,						/* name of service         */
		name,						/* service name to display */
		SERVICE_ALL_ACCESS,			/* desired access          */
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, /* service type            */
		SERVICE_DEMAND_START,		/* start type              */
		SERVICE_ERROR_NORMAL,		/* error control type      */
		path,						/* service's binary        */
		NULL,						/* no load ordering group  */
		NULL,						/* no tag identifier       */
		NULL,						/* no dependencies         */
		NULL,						/* LocalSystem account     */
		NULL						/* no password             */
	);

	if (0 == hService)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "Failed to create service %s, error code = %d\n", name, ui32Error);
		_tprintf(temp.c_str());
	}
	else
	{
		//¼º°ø 
		SERVICE_DESCRIPTION lpDes;
		lpDes.lpDescription = description;
		ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &lpDes);

		std::string temp;
		s2::string::Format(temp, "Service %s installed\n", name);
		_tprintf(temp.c_str());
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCManager);
}

void S2Service::UnInstall(TCHAR* name)
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (0 == hSCManager)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenSCManager failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());

		return;
	}

	SC_HANDLE hService = OpenService(hSCManager, name, SERVICE_ALL_ACCESS);
	if (0 == hService)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenService failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());
	}
	else
	{
		if (!DeleteService(hService))
		{
			std::string temp;
			s2::string::Format(temp, "Failed to delete service %s\n", name);
			_tprintf(temp.c_str());
		}
		else
		{
			std::string temp;
			s2::string::Format(temp, "Service %s removed\n", name);
			_tprintf(temp.c_str());
		}
		CloseServiceHandle(hService);
	}
	CloseServiceHandle(hSCManager);
}

bool S2Service::RunService(TCHAR* name)
{
	// run service with given name
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (0 == hSCManager)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenSCManager failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());

		return false;
	}

	// open the service
	SC_HANDLE hService = OpenService(hSCManager, name, SERVICE_ALL_ACCESS);
	if (0 == hService)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenService failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());

		CloseServiceHandle(hSCManager);

		return false;
	}

	// call StartService to run the service
	if (FALSE == StartService(hService, 0, (const TCHAR**)NULL))
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "StartService failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());

		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);

		return false;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return true;
}

bool S2Service::StopService(TCHAR* name)
{
	// Stop service with given name
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (0 == hSCManager)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenSCManager failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());

		return false;
	}

	// open the service
	SC_HANDLE hService = OpenService(hSCManager, name, SERVICE_ALL_ACCESS);
	if (0 == hService)
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "OpenService failed, error code = %d\n", ui32Error);
		_tprintf(temp.c_str());

		CloseServiceHandle(hSCManager);

		return false;
	}

	// call ControlService to kill the given service
	SERVICE_STATUS status;
	if (FALSE == ControlService(hService, SERVICE_CONTROL_STOP, &status))
	{
		uint32_t ui32Error = GetLastError();
		std::string temp;
		s2::string::Format(temp, "ControlService failed, error code = %d", ui32Error);
		_tprintf(temp.c_str());

		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);

		return false;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return true;
}
}