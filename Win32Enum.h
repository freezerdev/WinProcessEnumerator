#pragma once

#include "Common.h"
#include <map>

// Enumerate processes using the win32 API EnumProcesses()
std::map<uint32_t, std::wstring> EnumProcessesWin32(void);
