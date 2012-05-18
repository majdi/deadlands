/*
**
** Malicious Code - Project: deadlands
** Filename	: [ Driver.h ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

#pragma once

#include <string>

class __declspec(dllexport)	CDriver
{
public:
	/*
	** ~ Constructor / Destructor:
	*/
	CDriver(std::string filename, std::string displayName, std::string deviceName);
	~CDriver();

	/*
	** ~ Main methods:
	*/
	//int	LoadDriver();
	//int	UnloadDriver();

	/*
	** ~ Hide processes:
	*/
	int	DKOMHideProcess(std::string pid);
	int	SSDTHideProcess(std::string name);

	/*
	** ~ Hook methods:
	*/
	//int	HookIDT(std::string nInt);

private:
	/*
	** ~ Attributes:
	*/
	std::string		_filename;
	std::string		_displayName;
	std::string		_deviceName;
};

extern "C" __declspec(dllexport)
CDriver* CDriverNew(std::string filename, std::string displayName, std::string deviceName);

extern "C" __declspec(dllexport)
void CDriverDelete(CDriver* driver);
