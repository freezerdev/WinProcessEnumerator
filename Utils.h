#pragma once

#include "Common.h"
#include "Str.h"
#include <vector>

HRESULT ComInit(const bool bMultithreaded = true);
HRESULT ComInitEx(const bool bMultithreaded = true, const DWORD dwAuthnLevel = RPC_C_AUTHN_LEVEL_DEFAULT);
void ComFree(void);

std::wstring GetProcessFilename(const uint32_t nPid);
std::vector<CStr> GetCommandLineArguments(void);
