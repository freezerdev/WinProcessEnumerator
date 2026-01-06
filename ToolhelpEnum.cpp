#include "Win32Enum.h"
#include <tlhelp32.h>

//#################################################################################################
std::map<uint32_t, std::wstring> EnumProcessesToolhelp(void)
{
	std::map<uint32_t, std::wstring> mapProcesses;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W pe32 = {0};
		pe32.dwSize = sizeof(pe32);

		if(Process32FirstW(hSnapshot, &pe32))
		{
			do{
				mapProcesses.emplace(pe32.th32ProcessID, pe32.szExeFile);
			}while(Process32NextW(hSnapshot, &pe32));
		}

		CloseHandle(hSnapshot);
	}

	return mapProcesses;
}
