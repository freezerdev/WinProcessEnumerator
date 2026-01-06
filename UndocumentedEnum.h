#pragma once

#include "Common.h"
#include <map>

// Enumerate processes using the "undocumented" API NtQuerySystemInformation()
std::map<uint32_t, std::wstring> EnumProcessesUndocumented(void);
std::map<uint32_t, std::wstring> EnumProcessesUndocumentedBasic(void);
