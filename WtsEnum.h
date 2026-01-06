#pragma once

#include "Common.h"
#include <map>

// Enumerate processes using the Windows Terminal Services API WTSEnumerateProcesses()
std::map<uint32_t, std::wstring> EnumProcessesWts(void);
