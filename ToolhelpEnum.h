#pragma once

#include "Common.h"
#include <map>

// Enumerate processes using the Toolhelp library
std::map<uint32_t, std::wstring> EnumProcessesToolhelp(void);
