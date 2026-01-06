#include "WmiEnum.h"
#include "Wmi.h"
#include "Utils.h"

//#################################################################################################
std::map<uint32_t, std::wstring> EnumProcessesWmi(void)
{
	std::map<uint32_t, std::wstring> mapProcesses;

	CWmiService wmiSvc;
	if(SUCCEEDED(wmiSvc.Open()))
	{
		CWmiClass wmiClass = wmiSvc.GetClassFormat(L"Win32_Process", L"ProcessId", L"Name");

		CWmiInstance wmiInst = wmiClass.GetFirstInstance();
		while(wmiInst.IsOpen())
		{
			uint32_t nPid = (uint32_t)wmiInst.GetAsUInteger(L"ProcessId");
			std::wstring str(wmiInst.GetAsString(L"Name"));
			mapProcesses.emplace(nPid, str);

			wmiInst = wmiClass.GetNextInstance();
		}
	}

	return mapProcesses;
}
