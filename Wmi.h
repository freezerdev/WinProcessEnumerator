#pragma once

#include "Str.h"
#include <comdef.h>
#include <wbemidl.h>
#include <string>
#include <vector>

static PCWSTR g_szCimV2Namespace = L"root\\cimv2";

// Forward declarations
class CWmiClass;
class CWmiService;

//#################################################################################################
class CWmiInstance final
{
	friend class CWmiClass;

public:
	CWmiInstance(void);
	CWmiInstance(const CWmiInstance &src);
	CWmiInstance(CWmiInstance &&src) noexcept;
	~CWmiInstance(void);

	CWmiInstance &operator=(const CWmiInstance &src);
	CWmiInstance &operator=(CWmiInstance &&src) noexcept;

	void Close(void);

	_variant_t Get(PCWSTR szProperty) const;
	VARTYPE GetType(PCWSTR szProperty) const;

	// Returns the property value only if the property type matches the requested type and it is not an array
	bool GetBool(PCWSTR szProperty) const;
	int64_t GetInteger(PCWSTR szProperty) const;
	uint64_t GetUInteger(PCWSTR szProperty) const;
	double GetDouble(PCWSTR szProperty) const;
	CStrW GetString(PCWSTR szProperty) const;

	// Returns the property value converting from other property types if necessary but only if it is not an array
	bool GetAsBool(PCWSTR szProperty) const;
	int64_t GetAsInteger(PCWSTR szProperty) const;
	uint64_t GetAsUInteger(PCWSTR szProperty) const;
	double GetAsDouble(PCWSTR szProperty) const;
	CStrW GetAsString(PCWSTR szProperty) const;

	// Returns the property value only if the property type matches the requested type
	std::vector<bool> GetBoolVector(PCWSTR szProperty) const;
	std::vector<int64_t> GetIntegerVector(PCWSTR szProperty) const;
	std::vector<uint64_t> GetUIntegerVector(PCWSTR szProperty) const;
	std::vector<double> GetDoubleVector(PCWSTR szProperty) const;
	std::vector<CStrW> GetStringVector(PCWSTR szProperty) const;

	// Returns the property value converting from other property types if necessary
	std::vector<bool> GetAsBoolVector(PCWSTR szProperty) const;
	std::vector<int64_t> GetAsIntegerVector(PCWSTR szProperty) const;
	std::vector<uint64_t> GetAsUIntegerVector(PCWSTR szProperty) const;
	std::vector<double> GetAsDoubleVector(PCWSTR szProperty) const;
	std::vector<CStrW> GetAsStringVector(PCWSTR szProperty) const;

	bool IsOpen(void) const noexcept;

private:
	IWbemClassObject *m_pClass;
};


//#################################################################################################
class CWmiClass final
{
	friend class CWmiService;

public:
	CWmiClass(void);
	CWmiClass(const CWmiClass &src);
	CWmiClass(CWmiClass &&src) noexcept;
	~CWmiClass(void);

	CWmiClass &operator=(const CWmiClass &src);
	CWmiClass &operator=(CWmiClass &&src) noexcept;

	void Close(void);

	bool IsOpen(void) const noexcept;

	CWmiInstance GetFirstInstance(void) const;
	CWmiInstance GetNextInstance(void) const;

private:
	IEnumWbemClassObject *m_pClassEnumerator;
};


//#################################################################################################
class CWmiService final
{
public:
	CWmiService(void);
	CWmiService(const CWmiService &src);
	CWmiService(CWmiService &&src) noexcept;
	explicit CWmiService(PCWSTR szNamespace);
	~CWmiService(void);

	CWmiService &operator=(const CWmiService &src);
	CWmiService &operator=(CWmiService &&src) noexcept;

	HRESULT Open(PCWSTR szNamespace = g_szCimV2Namespace, const DWORD dwAuthnLevel = RPC_C_AUTHN_LEVEL_DEFAULT);
	void Close(void);

	bool IsOpen(void) const noexcept;

	CWmiClass GetClass(PCWSTR szClassName) const;
	CWmiClass GetClass(PCWSTR szClassName, const std::vector<CStrW> &vecProperties) const;

	template<typename ...ARGS>
	CWmiClass GetClassFormat(PCWSTR szClassName, const ARGS&... args) const
	{
		CWmiClass wmiClass;

		if(m_pService)
		{
			const std::wstring arrArgs[] = {ToString(args)...};
			size_t nArgsCount = sizeof...(args);

			CStrW strQuery(L"select ");
			if(nArgsCount == 0)
				strQuery += L'*';
			else
			{
				bool bFirst = true;
				for(size_t n = 0; n < nArgsCount; ++n)
				{
					if(!bFirst)
						strQuery += L", ";

					strQuery += arrArgs[n].c_str();
					bFirst = false;
				}
			}
			strQuery += L" from ";
			strQuery += szClassName;

			BSTR bstrWql = SysAllocString(L"WQL");
			if(bstrWql)
			{
				BSTR bstrQuery = SysAllocString(strQuery);
				if(bstrQuery)
				{
					m_pService->ExecQuery(bstrWql, bstrQuery, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &wmiClass.m_pClassEnumerator);
					SysFreeString(bstrQuery);
				}

				SysFreeString(bstrWql);
			}
		}

		return wmiClass;
	}

	CWmiClass GetClassByQuery(PCWSTR szQuery) const;

private:
	IWbemLocator *m_pLocator;
	IWbemServices *m_pService;

	template<typename T>
	static std::wstring ToString(const T &val)
	{
		std::wostringstream stream;
		stream << val;
		return stream.str();
	}
};
