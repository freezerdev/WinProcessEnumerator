#include "Win32Enum.h"
#include "Utils.h"
#include <wtsapi32.h>

//#################################################################################################
std::map<uint32_t, std::wstring> EnumProcessesWts(void)
{
	std::map<uint32_t, std::wstring> mapProcesses;

	PWTS_PROCESS_INFOW pwtspi;
	DWORD dwCount;
	if(WTSEnumerateProcessesW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pwtspi, &dwCount))
	{
		for(DWORD dw = 0; dw < dwCount; ++dw)
			mapProcesses.emplace(pwtspi[dw].ProcessId, pwtspi[dw].pProcessName);

		WTSFreeMemory(pwtspi);
	}

	return mapProcesses;
}
