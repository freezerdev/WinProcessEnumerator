#pragma once

#include "Common.h"
#include <map>

// Enumerate processes using the WMI API
std::map<uint32_t, std::wstring> EnumProcessesWmi(void);
