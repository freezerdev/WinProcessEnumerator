#include <windows.h>
#include "ToolhelpEnum.h"
#include "UndocumentedEnum.h"
#include "Win32Enum.h"
#include "WmiEnum.h"
#include "WtsEnum.h"
#include "Str.h"
#include "Utils.h"
#include <iostream>

//#################################################################################################
int main(void)
{
	enum EEnumMethod : uint8_t
	{
		EEM_Unknown = 0,
		EEM_Toolhelp,
		EEM_Win32,
		EEM_Wmi,
		EEM_Wts,
		EEM_Undocumented,
		EEM_UndocumentedBasic,
	};

	// Process any command line parameters
	EEnumMethod eMethod = EEM_Wts;	// Default to the WTS API
	CStr strArg;
	auto vecCmdLineArgs = GetCommandLineArguments();
	auto itr = vecCmdLineArgs.begin();

	while(itr != vecCmdLineArgs.end())
	{
		if(itr->GetFirst() == L'-' || itr->GetFirst() == L'/')
		{
			strArg.Assign(*itr, 1);
			if(strArg.Compare(L"toolhelp", true) == 0)
			{
				eMethod = EEM_Toolhelp;
				break;
			}
			else if(strArg.Compare(L"win32", true) == 0)
			{
				eMethod = EEM_Win32;
				break;
			}
			else if(strArg.Compare(L"wmi", true) == 0)
			{
				eMethod = EEM_Wmi;
				break;
			}
			else if(strArg.Compare(L"wts", true) == 0)
			{
				eMethod = EEM_Wts;
				break;
			}
			else if(strArg.Compare(L"undocumented", true) == 0)
			{
				eMethod = EEM_Undocumented;
				break;
			}
			else if(strArg.Compare(L"basic", true) == 0)
			{
				eMethod = EEM_UndocumentedBasic;
				break;
			}
			else if(strArg.Compare(L"help", true) == 0 || strArg.Compare(L'h', true) == 0 || strArg.Compare(L'?') == 0)
			{	// Display application help/usage
				eMethod = EEM_Unknown;
				std::wcout << L"WinProcessEnumerator help" << std::endl;
				std::wcout << L"Demonstrate multiple different ways to enumerate running processes." << std::endl;
				std::wcout << L"Usage:  WinProcessEnumerator.exe [<method>]" << std::endl << std::endl;
				std::wcout << L"  -toolhelp      Use the Toolhelp library" << std::endl;
				std::wcout << L"  -win32         Use the Win32 API EnumProcesses()" << std::endl;
				std::wcout << L"  -wmi           Use the WMI API" << std::endl;
				std::wcout << L"  -wts           Use the WTS API WTSEnumerateProcesses()" << std::endl;
				std::wcout << L"  -undocumented  Use the undocumented API NtQuerySystemInformation() with 'SystemProcessInformation'" << std::endl;
				std::wcout << L"  -basic         Use the undocumented API NtQuerySystemInformation() with 'SystemBasicProcessInformation'" << std::endl;
				break;
			}
		}

		++itr;
	}

	if(eMethod == EEM_Toolhelp)
	{
		auto mapProcesses = EnumProcessesToolhelp();
		for(const auto &itr : mapProcesses)
			std::wcout << L'[' << itr.first << L"] " << itr.second << std::endl;
		std::wcout << mapProcesses.size() << L" processes enumerated (toolhelp)." << std::endl;
	}
	else if(eMethod == EEM_Win32)
	{
		auto mapProcesses = EnumProcessesWin32();
		for(const auto &itr : mapProcesses)
			std::wcout << L'[' << itr.first << L"] " << itr.second << std::endl;
		std::wcout << mapProcesses.size() << L" processes enumerated (win32)." << std::endl;
	}
	else if(eMethod == EEM_Wmi)
	{
		ComInit();
		auto mapProcesses = EnumProcessesWmi();
		ComFree();
		for(const auto &itr : mapProcesses)
			std::wcout << L'[' << itr.first << L"] " << itr.second << std::endl;
		std::wcout << mapProcesses.size() << L" processes enumerated (WMI)." << std::endl;
	}
	else if(eMethod == EEM_Wts)
	{
		auto mapProcesses = EnumProcessesWts();
		for(const auto &itr : mapProcesses)
			std::wcout << L'[' << itr.first << L"] " << itr.second << std::endl;
		std::wcout << mapProcesses.size() << L" processes enumerated (WTS)." << std::endl;
	}
	else if(eMethod == EEM_Undocumented)
	{
		auto mapProcesses = EnumProcessesUndocumented();
		for(const auto &itr : mapProcesses)
			std::wcout << L'[' << itr.first << L"] " << itr.second << std::endl;
		std::wcout << mapProcesses.size() << L" processes enumerated (undocumented)." << std::endl;
	}
	else if(eMethod == EEM_UndocumentedBasic)
	{
		auto mapProcesses = EnumProcessesUndocumentedBasic();
		for(const auto &itr : mapProcesses)
			std::wcout << L'[' << itr.first << L"] " << itr.second << std::endl;
		std::wcout << mapProcesses.size() << L" processes enumerated (undocumented basic)." << std::endl;
	}

	return NO_ERROR;
}
