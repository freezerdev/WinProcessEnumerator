#include "Win32Enum.h"
#include <winternl.h>

const auto SystemBasicProcessInformation = 252;
constexpr auto STATUS_INFO_LENGTH_MISMATCH = 0xC0000004;

using PFNNTQUERYSYSTEMINFORMATION = NTSTATUS(NTAPI*)(ULONG, PVOID, ULONG, PULONG);

//#################################################################################################
std::map<uint32_t, std::wstring> EnumProcessesUndocumented(void)
{
	std::map<uint32_t, std::wstring> mapProcesses;

	HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
	PFNNTQUERYSYSTEMINFORMATION pfnNtQuerySystemInformation = (PFNNTQUERYSYSTEMINFORMATION)GetProcAddress(hNtDll, "NtQuerySystemInformation");

	NTSTATUS status;
	ULONG nBufferSize = 0x80000;
	auto pProcessBuf = std::make_unique<BYTE[]>(nBufferSize);
	PSYSTEM_PROCESS_INFORMATION pspi = (PSYSTEM_PROCESS_INFORMATION)pProcessBuf.get();

	ULONG nRequired = 0;
	while((status = pfnNtQuerySystemInformation(SystemProcessInformation, pspi, nBufferSize, &nRequired)) == STATUS_INFO_LENGTH_MISMATCH && nRequired > nBufferSize)
	{	// Increase the buffer and try again
		nBufferSize = nRequired + 4096;
		pProcessBuf = std::make_unique<BYTE[]>(nBufferSize);
		pspi = (PSYSTEM_PROCESS_INFORMATION)pProcessBuf.get();
	}

	if(NT_SUCCESS(status))
	{
		while(1)
		{
			if(pspi->ImageName.Buffer)
				mapProcesses.emplace((uint32_t)pspi->UniqueProcessId, pspi->ImageName.Buffer);
			else
				mapProcesses.emplace((uint32_t)pspi->UniqueProcessId, std::wstring());	// C++ strings cannot be constructed from NULL

			if(pspi->NextEntryOffset == 0)
				break;

			pspi = (PSYSTEM_PROCESS_INFORMATION)((size_t)pspi + pspi->NextEntryOffset);
		}
	}

	return mapProcesses;
}

//#################################################################################################
std::map<uint32_t, std::wstring> EnumProcessesUndocumentedBasic(void)
{
	std::map<uint32_t, std::wstring> mapProcesses;

	struct SYSTEM_BASICPROCESS_INFORMATION
	{
		ULONG NextEntryOffset;
		HANDLE UniqueProcessId;
		HANDLE InheritedFromUniqueProcessId;
		ULONG64 SequenceNumber;
		UNICODE_STRING ImageName;
	};
	using PSYSTEM_BASICPROCESS_INFORMATION = SYSTEM_BASICPROCESS_INFORMATION*;

	HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
	PFNNTQUERYSYSTEMINFORMATION pfnNtQuerySystemInformation = (PFNNTQUERYSYSTEMINFORMATION)GetProcAddress(hNtDll, "NtQuerySystemInformation");

	NTSTATUS status;
	ULONG nBufferSize = 0x80000;
	auto pProcessBuf = std::make_unique<BYTE[]>(nBufferSize);
	PSYSTEM_BASICPROCESS_INFORMATION psbpi = (PSYSTEM_BASICPROCESS_INFORMATION)pProcessBuf.get();

	ULONG nRequired = 0;
	while((status = pfnNtQuerySystemInformation(SystemBasicProcessInformation, psbpi, nBufferSize, &nRequired)) == STATUS_INFO_LENGTH_MISMATCH && nRequired > nBufferSize)
	{	// Increase the buffer and try again
		nBufferSize = nRequired + 4096;
		pProcessBuf = std::make_unique<BYTE[]>(nBufferSize);
		psbpi = (PSYSTEM_BASICPROCESS_INFORMATION)pProcessBuf.get();
	}

	if(NT_SUCCESS(status))
	{
		while(1)
		{
			if(psbpi->ImageName.Buffer)
				mapProcesses.emplace((uint32_t)psbpi->UniqueProcessId, psbpi->ImageName.Buffer);
			else
				mapProcesses.emplace((uint32_t)psbpi->UniqueProcessId, std::wstring());	// C++ strings cannot be constructed from NULL

			if(psbpi->NextEntryOffset == 0)
				break;

			psbpi = (PSYSTEM_BASICPROCESS_INFORMATION)((size_t)psbpi + psbpi->NextEntryOffset);
		}
	}

	return mapProcesses;
}
