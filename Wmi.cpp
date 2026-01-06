#include "Wmi.h"
#include "StrUtils.h"

static PCWSTR g_szWql = L"WQL";

#ifndef HAS_FLAG
#define HAS_FLAG(var, flag)	(((var) & (flag)) == (flag))
#endif

/////////////////////////////////////////////////
// CWmiInstance
/////////////////////////////////////////////////

//#################################################################################################
CWmiInstance::CWmiInstance(void)
	: m_pClass(nullptr)
{
}

//#################################################################################################
CWmiInstance::CWmiInstance(const CWmiInstance &src)
	: m_pClass(nullptr)
{
	*this = src;
}

//#################################################################################################
CWmiInstance::CWmiInstance(CWmiInstance &&src) noexcept
	: m_pClass(src.m_pClass)
{
	src.m_pClass = nullptr;
}

//#################################################################################################
CWmiInstance::~CWmiInstance(void)
{
	Close();
}

//#################################################################################################
CWmiInstance &CWmiInstance::operator=(const CWmiInstance &src)
{
	if(this != &src)
	{
		Close();
		if(src.m_pClass)
		{
			m_pClass = src.m_pClass;
			m_pClass->AddRef();
		}
	}

	return *this;
}

//#################################################################################################
CWmiInstance &CWmiInstance::operator=(CWmiInstance &&src) noexcept
{
	if(this != &src)
	{
		if(m_pClass)
			m_pClass->Release();

		m_pClass = src.m_pClass;

		src.m_pClass = nullptr;
	}

	return *this;
}

//#################################################################################################
void CWmiInstance::Close(void)
{
	if(m_pClass)
	{
		m_pClass->Release();
		m_pClass = nullptr;
	}
}

//#################################################################################################
_variant_t CWmiInstance::Get(PCWSTR szProperty) const
{
	_variant_t var;

	if(m_pClass)
		m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);

	return var;
}

//#################################################################################################
VARTYPE CWmiInstance::GetType(PCWSTR szProperty) const
{
	VARTYPE vt = VT_EMPTY;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
			vt = var.vt;
	}

	return vt;
}

//#################################################################################################
bool CWmiInstance::GetBool(PCWSTR szProperty) const
{
	bool bProperty = false;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr) && var.vt == VT_BOOL)
			bProperty = (var.boolVal != VARIANT_FALSE);
	}

	return bProperty;
}

//#################################################################################################
int64_t CWmiInstance::GetInteger(PCWSTR szProperty) const
{
	int64_t nProperty = 0;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_I1)
				nProperty = var.cVal;
			else if(var.vt == VT_I2)
				nProperty = var.iVal;
			else if(var.vt == VT_I4)
				nProperty = var.lVal;
			else if(var.vt == VT_I8)
				nProperty = var.llVal;
			else if(var.vt == VT_INT)
				nProperty = var.intVal;
		}
	}

	return nProperty;
}

//#################################################################################################
uint64_t CWmiInstance::GetUInteger(PCWSTR szProperty) const
{
	uint64_t nProperty = 0;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_UI1)
				nProperty = var.bVal;
			else if(var.vt == VT_UI2)
				nProperty = var.uiVal;
			else if(var.vt == VT_UI4)
				nProperty = var.ulVal;
			else if(var.vt == VT_UI8)
				nProperty = var.ullVal;
			else if(var.vt == VT_UINT)
				nProperty = var.uintVal;
		}
	}

	return nProperty;
}

//#################################################################################################
double CWmiInstance::GetDouble(PCWSTR szProperty) const
{
	double dProperty = 0;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_R4)
				dProperty = var.fltVal;
			else if(var.vt == VT_R8)
				dProperty = var.dblVal;
			else if(var.vt == VT_DATE)
				dProperty = var.date;
		}
	}

	return dProperty;
}

//#################################################################################################
CStrW CWmiInstance::GetString(PCWSTR szProperty) const
{
	CStrW strProperty;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr) && var.vt == VT_BSTR)
			strProperty = var.bstrVal;
	}

	return strProperty;
}

//#################################################################################################
bool CWmiInstance::GetAsBool(PCWSTR szProperty) const
{
	bool bProperty = false;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_BOOL)
				bProperty = (var.boolVal != VARIANT_FALSE);
			else if(var.vt == VT_I1)
				bProperty = (var.cVal != 0);
			else if(var.vt == VT_I2)
				bProperty = (var.iVal != 0);
			else if(var.vt == VT_I4)
				bProperty = (var.lVal != 0);
			else if(var.vt == VT_I8)
				bProperty = (var.llVal != 0);
			else if(var.vt == VT_INT)
				bProperty = (var.intVal != 0);
			else if(var.vt == VT_UI1)
				bProperty = (var.bVal != 0);
			else if(var.vt == VT_UI2)
				bProperty = (var.uiVal != 0);
			else if(var.vt == VT_UI4)
				bProperty = (var.ulVal != 0);
			else if(var.vt == VT_UI8)
				bProperty = (var.ullVal != 0);
			else if(var.vt == VT_UINT)
				bProperty = (var.uintVal != 0);
			else if(var.vt == VT_R4)
				bProperty = !FloatIsZero(var.fltVal);
			else if(var.vt == VT_R8)
				bProperty = !FloatIsZero(var.dblVal);
			else if(var.vt == VT_DATE)
				bProperty = !FloatIsZero(var.date);
			else if(var.vt == VT_BSTR)
				bProperty = (StringToIntegerW<int64_t>(var.bstrVal) != 0);
		}
	}

	return bProperty;
}

//#################################################################################################
int64_t CWmiInstance::GetAsInteger(PCWSTR szProperty) const
{
	int64_t nProperty = 0;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_BOOL)
				nProperty = (var.boolVal == VARIANT_FALSE) ? 0 : 1;
			else if(var.vt == VT_I1)
				nProperty = var.cVal;
			else if(var.vt == VT_I2)
				nProperty = var.iVal;
			else if(var.vt == VT_I4)
				nProperty = var.lVal;
			else if(var.vt == VT_I8)
				nProperty = var.llVal;
			else if(var.vt == VT_INT)
				nProperty = var.intVal;
			else if(var.vt == VT_UI1)
				nProperty = (int64_t)var.bVal;
			else if(var.vt == VT_UI2)
				nProperty = (int64_t)var.uiVal;
			else if(var.vt == VT_UI4)
				nProperty = (int64_t)var.ulVal;
			else if(var.vt == VT_UI8)
				nProperty = (int64_t)var.ullVal;
			else if(var.vt == VT_UINT)
				nProperty = (int64_t)var.uintVal;
			else if(var.vt == VT_R4)
				nProperty = (int64_t)var.fltVal;	// Truncate, may result in data loss
			else if(var.vt == VT_R8)
				nProperty = (int64_t)var.dblVal;	// Truncate, may result in data loss
			else if(var.vt == VT_DATE)
				nProperty = (int64_t)var.date;		// Truncate, may result in data loss
			else if(var.vt == VT_BSTR)
				nProperty = StringToIntegerW<int64_t>(var.bstrVal);
		}
	}

	return nProperty;
}

//#################################################################################################
uint64_t CWmiInstance::GetAsUInteger(PCWSTR szProperty) const
{
	uint64_t nProperty = 0;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_BOOL)
				nProperty = (var.boolVal == VARIANT_FALSE) ? 0 : 1;
			else if(var.vt == VT_I1)
				nProperty = (uint64_t)var.cVal;
			else if(var.vt == VT_I2)
				nProperty = (uint64_t)var.iVal;
			else if(var.vt == VT_I4)
				nProperty = (uint64_t)var.lVal;
			else if(var.vt == VT_I8)
				nProperty = (uint64_t)var.llVal;
			else if(var.vt == VT_INT)
				nProperty = (uint64_t)var.intVal;
			else if(var.vt == VT_UI1)
				nProperty = var.bVal;
			else if(var.vt == VT_UI2)
				nProperty = var.uiVal;
			else if(var.vt == VT_UI4)
				nProperty = var.ulVal;
			else if(var.vt == VT_UI8)
				nProperty = var.ullVal;
			else if(var.vt == VT_UINT)
				nProperty = var.uintVal;
			else if(var.vt == VT_R4)
				nProperty = (uint64_t)var.fltVal;	// Truncate, may result in data loss
			else if(var.vt == VT_R8)
				nProperty = (uint64_t)var.dblVal;	// Truncate, may result in data loss
			else if(var.vt == VT_DATE)
				nProperty = (uint64_t)var.date;		// Truncate, may result in data loss
			else if(var.vt == VT_BSTR)
				nProperty = StringToIntegerW<uint64_t>(var.bstrVal);
		}
	}

	return nProperty;
}

//#################################################################################################
double CWmiInstance::GetAsDouble(PCWSTR szProperty) const
{
	double dProperty = 0;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_BOOL)
				dProperty = (var.boolVal == VARIANT_FALSE) ? 0.0 : 1.0;
			else if(var.vt == VT_I1)
				dProperty = var.cVal;
			else if(var.vt == VT_I2)
				dProperty = var.iVal;
			else if(var.vt == VT_I4)
				dProperty = var.lVal;
			else if(var.vt == VT_I8)
				dProperty = (double)var.llVal;
			else if(var.vt == VT_INT)
				dProperty = var.intVal;
			else if(var.vt == VT_UI1)
				dProperty = var.bVal;
			else if(var.vt == VT_UI2)
				dProperty = var.uiVal;
			else if(var.vt == VT_UI4)
				dProperty = var.ulVal;
			else if(var.vt == VT_UI8)
				dProperty = (double)var.ullVal;
			else if(var.vt == VT_UINT)
				dProperty = var.uintVal;
			else if(var.vt == VT_R4)
				dProperty = var.fltVal;
			else if(var.vt == VT_R8)
				dProperty = var.dblVal;
			else if(var.vt == VT_DATE)
				dProperty = var.date;
			else if(var.vt == VT_BSTR)
				dProperty = StringToFloatW<double>(var.bstrVal);
		}
	}

	return dProperty;
}

//#################################################################################################
CStrW CWmiInstance::GetAsString(PCWSTR szProperty) const
{
	CStrW strProperty;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			if(var.vt == VT_BOOL)
				strProperty = (var.boolVal == VARIANT_FALSE) ? L'0' : L'1';
			else if(var.vt == VT_I1)
				strProperty = IntegerToStringW(var.cVal);
			else if(var.vt == VT_I2)
				strProperty = IntegerToStringW(var.iVal);
			else if(var.vt == VT_I4)
				strProperty = IntegerToStringW(var.lVal);
			else if(var.vt == VT_I8)
				strProperty = IntegerToStringW(var.llVal);
			else if(var.vt == VT_INT)
				strProperty = IntegerToStringW(var.intVal);
			else if(var.vt == VT_UI1)
				strProperty = IntegerToStringW(var.bVal);
			else if(var.vt == VT_UI2)
				strProperty = IntegerToStringW(var.uiVal);
			else if(var.vt == VT_UI4)
				strProperty = IntegerToStringW(var.ulVal);
			else if(var.vt == VT_UI8)
				strProperty = IntegerToStringW(var.ullVal);
			else if(var.vt == VT_UINT)
				strProperty = IntegerToStringW(var.uintVal);
			else if(var.vt == VT_R4)
				strProperty = FloatToStringW(var.fltVal);
			else if(var.vt == VT_R8)
				strProperty = FloatToStringW(var.dblVal);
			else if(var.vt == VT_DATE)
				strProperty = FloatToStringW(var.date);
			else if(var.vt == VT_BSTR)
				strProperty = var.bstrVal;
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				SAFEARRAY *psa = V_ARRAY(&var);
				if(psa)
				{
					long nLower = 0;
					long nUpper = 0;
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);

					for(long l = nLower; l <= nUpper; ++l)
					{
						BSTR value;
						if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						{
							if(!strProperty.IsEmpty())
								strProperty += L' ';
							strProperty += value;
							SysFreeString(value);
						}
					}
				}
			}
		}
	}

	return strProperty;
}

//#################################################################################################
std::vector<bool> CWmiInstance::GetBoolVector(PCWSTR szProperty) const
{
	std::vector<bool> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BOOL)
				vecProperties.push_back(var.boolVal != VARIANT_FALSE);
			else if(var.vt == (VT_ARRAY | VT_BOOL))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					VARIANT_BOOL value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != VARIANT_FALSE);
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<int64_t> CWmiInstance::GetIntegerVector(PCWSTR szProperty) const
{
	std::vector<int64_t> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_I1)
				vecProperties.push_back(var.cVal);
			else if(var.vt == VT_I2)
				vecProperties.push_back(var.iVal);
			else if(var.vt == VT_I4)
				vecProperties.push_back(var.lVal);
			else if(var.vt == VT_I8)
				vecProperties.push_back(var.llVal);
			else if(var.vt == VT_INT)
				vecProperties.push_back(var.intVal);
			else if(var.vt == (VT_ARRAY | VT_I1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					CHAR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					SHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_INT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					INT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<uint64_t> CWmiInstance::GetUIntegerVector(PCWSTR szProperty) const
{
	std::vector<uint64_t> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_UI1)
				vecProperties.push_back(var.bVal);
			else if(var.vt == VT_UI2)
				vecProperties.push_back(var.uiVal);
			else if(var.vt == VT_UI4)
				vecProperties.push_back(var.ulVal);
			else if(var.vt == VT_UI8)
				vecProperties.push_back(var.ullVal);
			else if(var.vt == VT_UINT)
				vecProperties.push_back(var.uintVal);
			else if(var.vt == (VT_ARRAY | VT_UI1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BYTE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					USHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UINT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					UINT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<double> CWmiInstance::GetDoubleVector(PCWSTR szProperty) const
{
	std::vector<double> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_R4)
				vecProperties.push_back(var.fltVal);
			else if(var.vt == VT_R8)
				vecProperties.push_back(var.dblVal);
			else if(var.vt == VT_DATE)
				vecProperties.push_back(var.date);
			else if(var.vt == (VT_ARRAY | VT_R4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					FLOAT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DOUBLE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_DATE))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DATE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<CStrW> CWmiInstance::GetStringVector(PCWSTR szProperty) const
{
	std::vector<CStrW> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BSTR)
				vecProperties.emplace_back(var.bstrVal);
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					BSTR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
					{
						vecProperties.emplace_back(value);
						SysFreeString(value);
					}
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<bool> CWmiInstance::GetAsBoolVector(PCWSTR szProperty) const
{
	std::vector<bool> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BOOL)
				vecProperties.push_back(var.boolVal != VARIANT_FALSE);
			else if(var.vt == VT_I1)
				vecProperties.push_back(var.cVal != 0);
			else if(var.vt == VT_I2)
				vecProperties.push_back(var.iVal != 0);
			else if(var.vt == VT_I4)
				vecProperties.push_back(var.lVal != 0);
			else if(var.vt == VT_I8)
				vecProperties.push_back(var.llVal != 0);
			else if(var.vt == VT_INT)
				vecProperties.push_back(var.intVal != 0);
			else if(var.vt == VT_UI1)
				vecProperties.push_back(var.bVal != 0);
			else if(var.vt == VT_UI2)
				vecProperties.push_back(var.uiVal != 0);
			else if(var.vt == VT_UI4)
				vecProperties.push_back(var.ulVal != 0);
			else if(var.vt == VT_UI8)
				vecProperties.push_back(var.ullVal != 0);
			else if(var.vt == VT_UINT)
				vecProperties.push_back(var.uintVal != 0);
			else if(var.vt == VT_R4)
				vecProperties.push_back(!FloatIsZero(var.fltVal));
			else if(var.vt == VT_R8)
				vecProperties.push_back(!FloatIsZero(var.dblVal));
			else if(var.vt == VT_DATE)
				vecProperties.push_back(!FloatIsZero(var.date));
			else if(var.vt == VT_BSTR)
				vecProperties.push_back(StringToIntegerW<int64_t>(var.bstrVal) != 0);
			else if(var.vt == (VT_ARRAY | VT_BOOL))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					VARIANT_BOOL value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != VARIANT_FALSE);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					CHAR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					SHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_INT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					INT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BYTE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					USHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UINT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					UINT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value != 0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					FLOAT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(!FloatIsZero(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DOUBLE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(!FloatIsZero(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_DATE))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DATE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(!FloatIsZero(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BSTR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
					{
						vecProperties.push_back(StringToIntegerW<int64_t>(value) != 0);
						SysFreeString(value);
					}
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<int64_t> CWmiInstance::GetAsIntegerVector(PCWSTR szProperty) const
{
	std::vector<int64_t> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BOOL)
				vecProperties.push_back((var.boolVal == VARIANT_FALSE) ? 0 : 1);
			else if(var.vt == VT_I1)
				vecProperties.push_back(var.cVal);
			else if(var.vt == VT_I2)
				vecProperties.push_back(var.iVal);
			else if(var.vt == VT_I4)
				vecProperties.push_back(var.lVal);
			else if(var.vt == VT_I8)
				vecProperties.push_back(var.llVal);
			else if(var.vt == VT_INT)
				vecProperties.push_back(var.intVal);
			else if(var.vt == VT_UI1)
				vecProperties.push_back((int64_t)var.bVal);
			else if(var.vt == VT_UI2)
				vecProperties.push_back((int64_t)var.uiVal);
			else if(var.vt == VT_UI4)
				vecProperties.push_back((int64_t)var.ulVal);
			else if(var.vt == VT_UI8)
				vecProperties.push_back((int64_t)var.ullVal);
			else if(var.vt == VT_UINT)
				vecProperties.push_back((int64_t)var.uintVal);
			else if(var.vt == VT_R4)
				vecProperties.push_back((int64_t)var.fltVal);	// Truncate, may result in data loss
			else if(var.vt == VT_R8)
				vecProperties.push_back((int64_t)var.dblVal);	// Truncate, may result in data loss
			else if(var.vt == VT_DATE)
				vecProperties.push_back((int64_t)var.date);		// Truncate, may result in data loss
			else if(var.vt == VT_BSTR)
				vecProperties.push_back(StringToIntegerW<int64_t>(var.bstrVal));
			else if(var.vt == (VT_ARRAY | VT_BOOL))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					VARIANT_BOOL value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((value == VARIANT_FALSE) ? 0 : 1);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					CHAR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					SHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_INT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					INT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BYTE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					USHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UINT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					UINT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					FLOAT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);	// Truncate, may result in data loss
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DOUBLE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);	// Truncate, may result in data loss
				}
			}
			else if(var.vt == (VT_ARRAY | VT_DATE))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DATE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((int64_t)value);	// Truncate, may result in data loss
				}
			}
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BSTR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
					{
						vecProperties.push_back(StringToIntegerW<int64_t>(value));
						SysFreeString(value);
					}
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<uint64_t> CWmiInstance::GetAsUIntegerVector(PCWSTR szProperty) const
{
	std::vector<uint64_t> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BOOL)
				vecProperties.push_back((var.boolVal == VARIANT_FALSE) ? 0 : 1);
			else if(var.vt == VT_I1)
				vecProperties.push_back((uint64_t)var.cVal);
			else if(var.vt == VT_I2)
				vecProperties.push_back((uint64_t)var.iVal);
			else if(var.vt == VT_I4)
				vecProperties.push_back((uint64_t)var.lVal);
			else if(var.vt == VT_I8)
				vecProperties.push_back((uint64_t)var.llVal);
			else if(var.vt == VT_INT)
				vecProperties.push_back((uint64_t)var.intVal);
			else if(var.vt == VT_UI1)
				vecProperties.push_back(var.bVal);
			else if(var.vt == VT_UI2)
				vecProperties.push_back(var.uiVal);
			else if(var.vt == VT_UI4)
				vecProperties.push_back(var.ulVal);
			else if(var.vt == VT_UI8)
				vecProperties.push_back(var.ullVal);
			else if(var.vt == VT_UINT)
				vecProperties.push_back(var.uintVal);
			else if(var.vt == VT_R4)
				vecProperties.push_back((uint64_t)var.fltVal);	// Truncate, may result in data loss
			else if(var.vt == VT_R8)
				vecProperties.push_back((uint64_t)var.dblVal);	// Truncate, may result in data loss
			else if(var.vt == VT_DATE)
				vecProperties.push_back((uint64_t)var.date);	// Truncate, may result in data loss
			else if(var.vt == VT_BSTR)
				vecProperties.push_back(StringToIntegerW<uint64_t>(var.bstrVal));
			else if(var.vt == (VT_ARRAY | VT_BOOL))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					VARIANT_BOOL value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((value == VARIANT_FALSE) ? 0 : 1);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					CHAR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					SHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_INT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					INT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BYTE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					USHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UINT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					UINT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					FLOAT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);	// Truncate, may result in data loss
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DOUBLE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);	// Truncate, may result in data loss
				}
			}
			else if(var.vt == (VT_ARRAY | VT_DATE))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DATE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((uint64_t)value);	// Truncate, may result in data loss
				}
			}
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BSTR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
					{
						vecProperties.push_back(StringToIntegerW<uint64_t>(value));
						SysFreeString(value);
					}
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<double> CWmiInstance::GetAsDoubleVector(PCWSTR szProperty) const
{
	std::vector<double> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BOOL)
				vecProperties.push_back((var.boolVal == VARIANT_FALSE) ? 0.0 : 1.0);
			else if(var.vt == VT_I1)
				vecProperties.push_back(var.cVal);
			else if(var.vt == VT_I2)
				vecProperties.push_back(var.iVal);
			else if(var.vt == VT_I4)
				vecProperties.push_back(var.lVal);
			else if(var.vt == VT_I8)
				vecProperties.push_back((double)var.llVal);
			else if(var.vt == VT_INT)
				vecProperties.push_back(var.intVal);
			else if(var.vt == VT_UI1)
				vecProperties.push_back(var.bVal);
			else if(var.vt == VT_UI2)
				vecProperties.push_back(var.uiVal);
			else if(var.vt == VT_UI4)
				vecProperties.push_back(var.ulVal);
			else if(var.vt == VT_UI8)
				vecProperties.push_back((double)var.ullVal);
			else if(var.vt == VT_UINT)
				vecProperties.push_back(var.uintVal);
			else if(var.vt == VT_R4)
				vecProperties.push_back(var.fltVal);
			else if(var.vt == VT_R8)
				vecProperties.push_back(var.dblVal);
			else if(var.vt == VT_DATE)
				vecProperties.push_back(var.date);
			else if(var.vt == VT_BSTR)
				vecProperties.push_back(StringToFloatW<double>(var.bstrVal));
			else if(var.vt == (VT_ARRAY | VT_BOOL))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					VARIANT_BOOL value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((var.boolVal == VARIANT_FALSE) ? 0.0 : 1.0);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					CHAR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					SHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					LONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((double)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_INT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					INT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BYTE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					USHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					ULONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back((double)value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UINT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					UINT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					FLOAT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DOUBLE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_DATE))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					DATE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
						vecProperties.push_back(value);
				}
			}
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long n = nLower; n <= nUpper; ++n)
				{
					BSTR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &n, &value)))
					{
						vecProperties.push_back(StringToFloatW<double>(value));
						SysFreeString(value);
					}
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
std::vector<CStrW> CWmiInstance::GetAsStringVector(PCWSTR szProperty) const
{
	std::vector<CStrW> vecProperties;

	if(m_pClass)
	{
		_variant_t var;
		HRESULT hr = m_pClass->Get(szProperty, 0, &var, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			SAFEARRAY *psa = nullptr;
			long nLower = 0;
			long nUpper = 0;

			if(HAS_FLAG(var.vt, VT_ARRAY))
			{
				psa = V_ARRAY(&var);
				if(psa)
				{
					SafeArrayGetLBound(psa, 1, &nLower);
					SafeArrayGetUBound(psa, 1, &nUpper);
				}
			}

			if(var.vt == VT_BOOL)
				vecProperties.emplace_back((var.boolVal == VARIANT_FALSE) ? L'0' : L'1');
			else if(var.vt == VT_I1)
				vecProperties.push_back(IntegerToStringW(var.cVal));
			else if(var.vt == VT_I2)
				vecProperties.push_back(IntegerToStringW(var.iVal));
			else if(var.vt == VT_I4)
				vecProperties.push_back(IntegerToStringW(var.lVal));
			else if(var.vt == VT_I8)
				vecProperties.push_back(IntegerToStringW(var.llVal));
			else if(var.vt == VT_INT)
				vecProperties.push_back(IntegerToStringW(var.intVal));
			else if(var.vt == VT_UI1)
				vecProperties.push_back(IntegerToStringW(var.bVal));
			else if(var.vt == VT_UI2)
				vecProperties.push_back(IntegerToStringW(var.uiVal));
			else if(var.vt == VT_UI4)
				vecProperties.push_back(IntegerToStringW(var.ulVal));
			else if(var.vt == VT_UI8)
				vecProperties.push_back(IntegerToStringW(var.ullVal));
			else if(var.vt == VT_UINT)
				vecProperties.push_back(IntegerToStringW(var.uintVal));
			else if(var.vt == VT_R4)
				vecProperties.push_back(FloatToStringW(var.fltVal));
			else if(var.vt == VT_R8)
				vecProperties.push_back(FloatToStringW(var.dblVal));
			else if(var.vt == VT_DATE)
				vecProperties.push_back(FloatToStringW(var.date));
			else if(var.vt == VT_BSTR)
				vecProperties.emplace_back(var.bstrVal);
			else if(var.vt == (VT_ARRAY | VT_BOOL))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					VARIANT_BOOL value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.emplace_back((value == VARIANT_FALSE) ? L'0' : L'1');
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					CHAR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					SHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					LONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_I8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					LONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_INT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					INT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI1))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					BYTE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI2))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					USHORT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					ULONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UI8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					ULONGLONG value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_UINT))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					UINT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(IntegerToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R4))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					FLOAT value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(FloatToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_R8))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					DOUBLE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(FloatToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_DATE))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					DATE value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
						vecProperties.push_back(FloatToStringW(value));
				}
			}
			else if(var.vt == (VT_ARRAY | VT_BSTR))
			{
				vecProperties.reserve((size_t)nUpper - (size_t)nLower + 1);
				for(long l = nLower; l <= nUpper; ++l)
				{
					BSTR value;
					if(SUCCEEDED(SafeArrayGetElement(psa, &l, &value)))
					{
						vecProperties.emplace_back(value);
						SysFreeString(value);
					}
				}
			}
		}
	}

	return vecProperties;
}

//#################################################################################################
bool CWmiInstance::IsOpen(void) const noexcept
{
	return (m_pClass != nullptr);
}




/////////////////////////////////////////////////
// CWmiClass
/////////////////////////////////////////////////

//#################################################################################################
CWmiClass::CWmiClass(void)
	: m_pClassEnumerator(nullptr)
{
}

//#################################################################################################
CWmiClass::CWmiClass(const CWmiClass &src)
	: m_pClassEnumerator(nullptr)
{
	*this = src;
}

//#################################################################################################
CWmiClass::CWmiClass(CWmiClass &&src) noexcept
	: m_pClassEnumerator(src.m_pClassEnumerator)
{
	src.m_pClassEnumerator = nullptr;
}

//#################################################################################################
CWmiClass::~CWmiClass(void)
{
	Close();
}

//#################################################################################################
CWmiClass &CWmiClass::operator=(const CWmiClass &src)
{
	if(this != &src)
	{
		Close();
		if(src.m_pClassEnumerator)
		{
			m_pClassEnumerator = src.m_pClassEnumerator;
			m_pClassEnumerator->AddRef();
		}
	}

	return *this;
}

//#################################################################################################
CWmiClass &CWmiClass::operator=(CWmiClass &&src) noexcept
{
	if(this != &src)
	{
		if(m_pClassEnumerator)
			m_pClassEnumerator->Release();

		m_pClassEnumerator = src.m_pClassEnumerator;

		src.m_pClassEnumerator = nullptr;
	}

	return *this;
}

//#################################################################################################
void CWmiClass::Close(void)
{
	if(m_pClassEnumerator)
	{
		m_pClassEnumerator->Release();
		m_pClassEnumerator = nullptr;
	}
}

//#################################################################################################
bool CWmiClass::IsOpen(void) const noexcept
{
	return (m_pClassEnumerator != nullptr);
}

//#################################################################################################
CWmiInstance CWmiClass::GetFirstInstance(void) const
{
	CWmiInstance wmiInstance;

	if(m_pClassEnumerator)
	{
		m_pClassEnumerator->Reset();

		ULONG nReturned;
		m_pClassEnumerator->Next(WBEM_INFINITE, 1, &wmiInstance.m_pClass, &nReturned);
	}

	return wmiInstance;
}

//#################################################################################################
CWmiInstance CWmiClass::GetNextInstance(void) const
{
	CWmiInstance wmiInstance;

	if(m_pClassEnumerator)
	{
		ULONG nReturned;
		m_pClassEnumerator->Next(WBEM_INFINITE, 1, &wmiInstance.m_pClass, &nReturned);
	}

	return wmiInstance;
}




/////////////////////////////////////////////////
// CWmiService
/////////////////////////////////////////////////

//#################################################################################################
CWmiService::CWmiService(void)
	: m_pLocator(nullptr),
	m_pService(nullptr)
{
}

//#################################################################################################
CWmiService::CWmiService(const CWmiService &src)
	: m_pLocator(nullptr),
	m_pService(nullptr)
{
	*this = src;
}

//#################################################################################################
CWmiService::CWmiService(CWmiService &&src) noexcept
	: m_pLocator(src.m_pLocator),
	m_pService(src.m_pService)
{
	src.m_pLocator = nullptr;
	src.m_pService = nullptr;
}

//#################################################################################################
CWmiService::CWmiService(PCWSTR szNamespace)
	: m_pLocator(nullptr),
	m_pService(nullptr)
{
	Open(szNamespace);
}

//#################################################################################################
CWmiService::~CWmiService(void)
{
	Close();
}

//#################################################################################################
CWmiService &CWmiService::operator=(const CWmiService &src)
{
	if(this != &src)
	{
		Close();
		if(src.m_pLocator)
		{
			m_pLocator = src.m_pLocator;
			m_pLocator->AddRef();
		}

		if(src.m_pService)
		{
			m_pService = src.m_pService;
			m_pService->AddRef();
		}
	}

	return *this;
}

//#################################################################################################
CWmiService &CWmiService::operator=(CWmiService &&src) noexcept
{
	if(this != &src)
	{
		if(m_pService)
			m_pService->Release();
		if(m_pLocator)
			m_pLocator->Release();

		m_pLocator = src.m_pLocator;
		m_pService = src.m_pService;

		src.m_pLocator = nullptr;
		src.m_pService = nullptr;
	}

	return *this;
}

//#################################################################################################
HRESULT CWmiService::Open(PCWSTR szNamespace, const DWORD dwAuthnLevel)
{
	Close();

	HRESULT hr = CoCreateInstance(__uuidof(WbemLocator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWbemLocator), (PVOID*)&m_pLocator);
	if(SUCCEEDED(hr) && m_pLocator)
	{
		BSTR bstrNamespace = SysAllocString(szNamespace);
		if(bstrNamespace)
		{
			hr = m_pLocator->ConnectServer(bstrNamespace, nullptr, nullptr, nullptr, 0, nullptr, nullptr, &m_pService);
			if(SUCCEEDED(hr) && m_pService)
				hr = CoSetProxyBlanket(m_pService, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, dwAuthnLevel, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);
			else
			{
				m_pLocator->Release();
				m_pLocator = nullptr;
			}

			SysFreeString(bstrNamespace);
		}
		else
			hr = E_OUTOFMEMORY;
	}

	return hr;
}

//#################################################################################################
void CWmiService::Close(void)
{
	if(m_pService)
	{
		m_pService->Release();
		m_pService = nullptr;
	}

	if(m_pLocator)
	{
		m_pLocator->Release();
		m_pLocator = nullptr;
	}
}

//#################################################################################################
bool CWmiService::IsOpen(void) const noexcept
{
	return (m_pService != nullptr);
}

//#################################################################################################
CWmiClass CWmiService::GetClass(PCWSTR szClassName) const
{
	CWmiClass wmiClass;

	if(m_pService)
	{
		CStrW strQuery(L"select * from ");
		strQuery += szClassName;

		BSTR bstrWql = SysAllocString(g_szWql);
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

//#################################################################################################
CWmiClass CWmiService::GetClass(PCWSTR szClassName, const std::vector<CStrW> &vecProperties) const
{
	CWmiClass wmiClass;

	if(m_pService)
	{
		CStrW strQuery(L"select ");
		if(vecProperties.empty())
			strQuery += L'*';
		else
		{
			bool bFirst = true;
			for(const auto &property : vecProperties)
			{
				if(!bFirst)
					strQuery += L", ";

				strQuery += property;
				bFirst = false;
			}
		}
		strQuery += L" from ";
		strQuery += szClassName;

		BSTR bstrWql = SysAllocString(g_szWql);
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

//#################################################################################################
CWmiClass CWmiService::GetClassByQuery(PCWSTR szQuery) const
{
	CWmiClass wmiClass;

	if(m_pService)
	{
		BSTR bstrWql = SysAllocString(g_szWql);
		if(bstrWql)
		{
			BSTR bstrQuery = SysAllocString(szQuery);
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
