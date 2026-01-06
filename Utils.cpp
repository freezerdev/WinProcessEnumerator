#include "Utils.h"
#include "WhitespaceParser.h"

//#################################################################################################
HRESULT ComInit(const bool bMultithreaded)
{
	return CoInitializeEx(nullptr, bMultithreaded ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);
}

//#################################################################################################
HRESULT ComInitEx(const bool bMultithreaded, const DWORD dwAuthnLevel)
{
	HRESULT hr = CoInitializeEx(nullptr, bMultithreaded ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);
	if(SUCCEEDED(hr))
		hr = CoInitializeSecurity(nullptr, -1, nullptr, nullptr, dwAuthnLevel, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);

	return hr;
}

//#################################################################################################
void ComFree(void)
{
	CoUninitialize();
}

//#################################################################################################
std::wstring GetProcessFilename(const uint32_t nPid)
{
	std::wstring strFilename;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, nPid);
	if(hProcess)
	{
		wchar_t szName[1024] = {0};
		DWORD dwLen = 1024;
		if(QueryFullProcessImageNameW(hProcess, 0, szName, &dwLen))
			strFilename = szName;

		CloseHandle(hProcess);
	}

	return strFilename;
}

//#################################################################################################
std::vector<CStr> GetCommandLineArguments(void)
{
	std::vector<CStr> vecArgs;

	CWhitespaceParser cmdline(GetCommandLineW());
	for(size_t n = 0; n < cmdline.GetCount(); ++n)
		vecArgs.push_back(cmdline.Get(n));

	return vecArgs;
}
