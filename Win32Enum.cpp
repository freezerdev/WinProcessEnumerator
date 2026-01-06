#include "Win32Enum.h"
#include "Utils.h"
#define PSAPI_VERSION (2)
#include <psapi.h>

//#################################################################################################
std::map<uint32_t, std::wstring> EnumProcessesWin32(void)
{
	std::map<uint32_t, std::wstring> mapProcesses;

	size_t nAllocCount = 1024;
	auto pBuffer = std::make_unique<DWORD[]>(nAllocCount);

	size_t nLoopCount = 0;
	DWORD dwReturnedSize;
	EnumProcesses(pBuffer.get(), (DWORD)nAllocCount * sizeof(DWORD), &dwReturnedSize);
	while(dwReturnedSize / sizeof(DWORD) == nAllocCount && nLoopCount++ < 10)
	{
		nAllocCount += 1024;	// Increase the allocation and try again
		pBuffer = std::make_unique<DWORD[]>(nAllocCount);
		EnumProcesses(pBuffer.get(), (DWORD)nAllocCount * sizeof(DWORD), &dwReturnedSize);
	}

	if(dwReturnedSize)
	{
		size_t nCount = dwReturnedSize / sizeof(DWORD);
		for(size_t n = 0; n < nCount; ++n)
			mapProcesses.emplace(pBuffer.get()[n], GetProcessFilename(pBuffer.get()[n]));
	}

	return mapProcesses;
}
