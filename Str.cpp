#include "Str.h"
#include "StrUtils.h"
#include <cstdio>
#include <iostream>

// Static variables
const size_t CStr8::not_found = (size_t)-1;
const size_t CStr8::end = (size_t)-1;
const size_t CStrW::not_found = (size_t)-1;
const size_t CStrW::end = (size_t)-1;
const size_t CStr16::not_found = (size_t)-1;
const size_t CStr16::end = (size_t)-1;

// Swap two identically sized variables
#define SWAP(a, b)			\
	{						\
		static_assert(sizeof(a) == sizeof(b), "Mismatched variable size");\
		auto _tmp = (a);	\
		(a) = (b);			\
		(b) = _tmp;			\
	}

/////////////////////////////////////////////////
// CStr8
/////////////////////////////////////////////////

//#################################################################################################
CStr8::CStr8(void)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CStr8::CStr8(const CStr8 &src)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CStr8::CStr8(CStr8 &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PSTR)g_szNull8;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CStr8::CStr8(const CStrW &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStr8::CStr8(const CStr16 &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStr8::CStr8(const std::string &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStr8::CStr8(const std::wstring &str)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStr8::CStr8(PCSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CStr8::CStr8(PCWSTR sz)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CStr8::CStr8(const char ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CStr8::CStr8(const wchar_t ch)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CStr8::CStr8(const CStr8 &str, const size_t nStart, const size_t nLen)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, nStart, nLen);
}
//#################################################################################################
CStr8::CStr8(PCSTR sz, const size_t nStart, const size_t nLen)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, nStart, nLen);
}

//#################################################################################################
CStr8::CStr8(const EPrintfType eIgnored, PCSTR szFormat, ...)
	: m_sz((PSTR)g_szNull8),
	m_nBufLen(0),
	m_nStrLen(0)
{
	UNUSED(eIgnored);

	va_list vaArgs;
	va_start(vaArgs, szFormat);
	PrintfV(szFormat, vaArgs);
	va_end(vaArgs);
}

//#################################################################################################
CStr8::~CStr8(void)
{
	Dealloc();
}

//#################################################################################################
CStr8 &CStr8::Attach(PSTR sz, const size_t nBufLen)
{
	Dealloc();

	if(sz)
	{
		m_sz = sz;
		m_nBufLen = nBufLen;
		m_nStrLen = StringGetLength(sz);
	}

	return *this;
}

//#################################################################################################
PSTR CStr8::Detach(void) noexcept
{	// Release the buffer, it is up to the caller to dealloc the memory
	PSTR sz = nullptr;

	if(m_nBufLen)
	{
		sz = m_sz;
		m_sz = (PSTR)g_szNull8;
		m_nBufLen = m_nStrLen = 0;
	}

	return sz;
}

//#################################################################################################
void CStr8::SecureErase(void)
{	// Overwrite the memory of the string with zeros, great for sensitive data like passwords which otherwise could be left in memory when the string is freed
	for(size_t n = 0; n < m_nBufLen; ++n)
		m_sz[n] = g_chNull8;
	m_nStrLen = 0;
}

//#################################################################################################
size_t CStr8::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CStr8::GetCount(const bool bIncludeNullTerm) const
{	// Count of characters
	size_t nCount = (bIncludeNullTerm) ? 1 : 0;

	PCSTR szScan = m_sz;
	size_t nRemaining = m_nStrLen;

	while(*szScan != g_chNull8)
	{
		size_t nSize = GetMultiByteCharSize(szScan, nRemaining);
		szScan += nSize;
		--nRemaining;
		++nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
bool CStr8::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
void CStr8::Empty(const bool bFree)
{
	if(bFree)
		Dealloc();
	else
	{
		if(m_nBufLen)
			*m_sz = g_chNull8;
		else
			m_sz = (PSTR)g_szNull8;
		m_nStrLen = 0;
	}
}

//#################################################################################################
char CStr8::GetFirst(void) const
{
	return (m_nStrLen) ? m_sz[0] : g_chNull8;
}

//#################################################################################################
char CStr8::GetLast(void) const
{
	return (m_nStrLen) ? m_sz[m_nStrLen - 1] : g_chNull8;
}

//#################################################################################################
char CStr8::GetAt(const size_t nIndex) const
{
	return (nIndex < m_nStrLen) ? m_sz[nIndex] : g_chNull8;
}

//#################################################################################################
void CStr8::SetAt(const size_t nIndex, const char ch)
{
	if(nIndex < m_nStrLen)
	{
		m_sz[nIndex] = ch;
		if(ch == g_chNull8)
			m_nStrLen = nIndex;
	}
}

//#################################################################################################
CStr8::operator PCSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
CStr8 CStr8::AsUtf8(void) const
{
	return *this;
}

//#################################################################################################
CStrW CStr8::AsWide(void) const
{
	return CStrW(*this);
}

//#################################################################################################
CStrW CStr8::AsNative(void) const
{
	return CStrW(*this);
}

//#################################################################################################
CStr8 &CStr8::operator=(const CStr8 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(CStr8 &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (PSTR)g_szNull8;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(const CStr16 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(const std::string &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(const std::wstring &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
void CStr8::Assign(const CStr8 &str)
{
	if(this != &str)
	{
		if(str.m_nStrLen)
		{	// Alloc and copy
			if(Alloc(str.m_nStrLen + 1))
			{
				std::memcpy(m_sz, str.m_sz, str.m_nStrLen);
				m_nStrLen = str.m_nStrLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
			else
				Empty();
		}
		else
			Empty();
	}
}

//#################################################################################################
void CStr8::Assign(const CStrW &str)
{
	if(str.m_nStrLen)
	{	// Alloc and copy
		if(Alloc(WideToUtf8(str.m_sz, str.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const CStr16 &str)
{
	if(str.m_nStrLen)
	{	// Alloc and copy
		if(Alloc(Utf16ToUtf8(str.m_sz, str.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf16ToUtf8(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const std::string &str)
{
	if(str.length())
	{	// Alloc and copy
		if(Alloc(str.length() + 1))
		{
			std::memcpy(m_sz, str.c_str(), str.length());
			m_nStrLen = str.length();
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const std::wstring &str)
{
	if(str.length())
	{	// Alloc and copy
		if(Alloc(WideToUtf8(str.c_str(), str.length(), nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(str.c_str(), str.length(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(PCSTR sz)
{
	if(sz && *sz != g_chNull8)
	{	// Alloc and copy
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(nStrLen + 1))
		{
			std::memcpy(m_sz, sz, nStrLen);
			m_nStrLen = nStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(PCWSTR sz)
{
	if(sz && *sz != g_chNullW)
	{	// Alloc and copy
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(WideToUtf8(sz, nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(sz, nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const char ch)
{
	if(ch != g_chNull8)
	{	// Alloc and set
		if(Alloc(2))
		{
			*m_sz = ch;
			m_nStrLen = 1;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const wchar_t ch)
{
	if(ch != g_chNullW)
	{	// Alloc and set
		if(Alloc(WideToUtf8(&ch, 1, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf8(&ch, 1, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const CStr8 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{	// Alloc and copy
		size_t nNewLen;
		if(nLen == end)
			nNewLen = str.m_nStrLen - nStart;
		else
			nNewLen = MIN(nLen, str.m_nStrLen - nStart);

		if(Alloc(nNewLen + 1))
		{
			std::memcpy(m_sz, str.m_sz + nStart, nNewLen);
			m_nStrLen = nNewLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(const CStrW &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{	// Alloc and copy
		size_t nNewLen;
		if(nLen == end)
			nNewLen = WideToUtf8(str.m_sz + nStart, str.m_nStrLen - nStart, nullptr, 0);
		else
		{
			size_t nNewLen8 = WideToUtf8(str.m_sz + nStart, MIN(nLen, str.m_nStrLen - nStart), nullptr, 0);
			nNewLen = MIN(nLen, nNewLen8);
		}

		if(Alloc(nNewLen + 1))
		{
			WideToUtf8(str.m_sz + nStart, str.m_nStrLen - nStart, m_sz, m_nBufLen);
			m_nStrLen = nNewLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(PCSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull8 && nLen)
	{	// Alloc and copy
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nNewLen;
			if(nLen == end)
				nNewLen = nStrLen - nStart;
			else
				nNewLen = MIN(nLen, nStrLen - nStart);

			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, sz + nStart, nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
			else
				Empty();
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr8::Assign(PCWSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNullW && nLen)
	{	// Alloc and copy
		size_t nStrLen = (nLen == CStrW::end) ? StringGetLength(sz) : CStrW::GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nNewLen;
			if(nLen == end)
				nNewLen = WideToUtf8(sz + nStart, nStrLen - nStart, nullptr, 0);
			else
			{
				size_t nNewLenW = WideToUtf8(sz + nStart, MIN(nLen, nStrLen - nStart), nullptr, 0);
				nNewLen = MIN(nLen, nNewLenW);
			}

			if(Alloc(nNewLen + 1))
			{
				WideToUtf8(sz + nStart, nStrLen - nStart, m_sz, m_nBufLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
			else
				Empty();
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
CStr8 &CStr8::operator+=(const CStr8 &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator+=(const CStrW &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator+=(PCSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator+=(PCWSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator+=(const char ch)
{
	Append(ch);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::operator+=(const wchar_t ch)
{
	Append(ch);
	return *this;
}

//#################################################################################################
void CStr8::Prepend(const CStr8 &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + str.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + str.m_nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[str.m_nStrLen] = g_chNull8;
			std::memcpy(m_sz, str.m_sz, str.m_nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(const CStrW &str)
{
	if(str.m_nStrLen)
	{
		size_t nStrLen = WideToUtf8(str.m_sz, str.m_nStrLen, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen] = g_chNull8;
			WideToUtf8(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(PCSTR sz)
{
	if(sz && *sz != g_chNull8)
	{
		size_t nStrLen = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen] = g_chNull8;
			std::memcpy(m_sz, sz, nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(PCWSTR sz)
{
	if(sz && *sz != g_chNullW)
	{
		size_t nStrLenW = StringGetLength(sz);
		size_t nStrLen8 = WideToUtf8(sz, nStrLenW, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen8;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen8, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen8] = g_chNull8;
			WideToUtf8(sz, nStrLenW, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(const char ch)
{
	if(ch != g_chNull8)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + 1, m_sz, m_nStrLen + 1);
			else
				m_sz[1] = g_chNull8;
			m_sz[0] = ch;
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(const wchar_t ch)
{
	if(ch != g_chNullW)
	{
		size_t nStrLen = WideToUtf8(&ch, 1, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen] = g_chNull8;
			WideToUtf8(&ch, 1, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(const CStr8 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nPrependLen;
		if(nLen == end)
			nPrependLen = str.m_nStrLen - nStart;
		else
			nPrependLen = MIN(nLen, str.m_nStrLen - nStart);

		size_t nNewStrLen = m_nStrLen + nPrependLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nPrependLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nPrependLen] = g_chNull8;
			std::memcpy(m_sz, str.m_sz + nStart, nPrependLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(const CStrW &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nPrependLenW;
		size_t nPrependLen8;

		if(nLen == end)
		{
			nPrependLenW = str.m_nStrLen - nStart;
			nPrependLen8 = WideToUtf8(str.m_sz + nStart, nPrependLenW, nullptr, 0);
		}
		else
		{
			nPrependLenW = MIN(nLen, str.m_nStrLen - nStart);
			size_t nConvertLen8 = WideToUtf8(str.m_sz + nStart, nPrependLenW, nullptr, 0);
			nPrependLen8 = MIN(nLen, nConvertLen8);
		}

		size_t nNewStrLen = m_nStrLen + nPrependLen8;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nPrependLen8, m_sz, m_nStrLen + 1);
			else
				m_sz[nPrependLen8] = g_chNull8;
			WideToUtf8(str.m_sz + nStart, nPrependLenW, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr8::Prepend(PCSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull8 && nLen)
	{
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nPrependLen;
			if(nLen == end)
				nPrependLen = nStrLen - nStart;
			else
				nPrependLen = MIN(nLen, nStrLen - nStart);

			size_t nNewStrLen = m_nStrLen + nPrependLen;
			if(Alloc(nNewStrLen + 1))
			{
				if(m_nStrLen)
					std::memmove(m_sz + nPrependLen, m_sz, m_nStrLen + 1);
				else
					m_sz[nPrependLen] = g_chNull8;
				std::memcpy(m_sz, sz + nStart, nPrependLen);
				m_nStrLen = nNewStrLen;
			}
		}
	}
}

//#################################################################################################
void CStr8::Prepend(PCWSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNullW && nLen)
	{
		size_t nStrLenW = (nLen == CStrW::end) ? StringGetLength(sz) : CStrW::GetLength(sz, nStart + nLen);
		if(nStart < nStrLenW)
		{
			size_t nPrependLenW;
			size_t nPrependLen8;

			if(nLen == end)
			{
				nPrependLenW = nStrLenW - nStart;
				nPrependLen8 = WideToUtf8(sz + nStart, nPrependLenW, nullptr, 0);
			}
			else
			{
				nPrependLenW = MIN(nLen, nStrLenW - nStart);
				size_t nConvertLen8 = WideToUtf8(sz + nStart, nPrependLenW, nullptr, 0);
				nPrependLen8 = MIN(nLen, nConvertLen8);
			}

			size_t nNewStrLen = m_nStrLen + nPrependLen8;
			if(Alloc(nNewStrLen + 1))
			{
				if(m_nStrLen)
					std::memmove(m_sz + nPrependLen8, m_sz, m_nStrLen + 1);
				else
					m_sz[nPrependLen8] = g_chNull8;
				WideToUtf8(sz + nStart, nPrependLenW, m_sz, m_nBufLen);
				m_nStrLen = nNewStrLen;
			}
		}
	}
}

//#################################################################################################
void CStr8::Append(const CStr8 &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + str.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memcpy(m_sz + m_nStrLen, str.m_sz, str.m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(const CStrW &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + WideToUtf8(str.m_sz, str.m_nStrLen, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			WideToUtf8(str.m_sz, str.m_nStrLen, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(PCSTR sz)
{
	if(sz && *sz != g_chNull8)
	{
		size_t nStrLen = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memcpy(m_sz + m_nStrLen, sz, nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(PCWSTR sz)
{
	if(sz && *sz != g_chNullW)
	{
		size_t nStrLenW = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + WideToUtf8(sz, nStrLenW, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			WideToUtf8(sz, nStrLenW, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(const char ch)
{
	if(ch != g_chNull8)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			m_sz[m_nStrLen] = ch;
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(const wchar_t ch)
{
	if(ch != g_chNullW)
	{
		size_t nNewStrLen = m_nStrLen + WideToUtf8(&ch, 1, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			WideToUtf8(&ch, 1, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(const CStr8 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nAppendLen;
		if(nLen == end)
			nAppendLen = str.m_nStrLen - nStart;
		else
			nAppendLen = MIN(nLen, str.m_nStrLen - nStart);

		size_t nNewStrLen = m_nStrLen + nAppendLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memcpy(m_sz + m_nStrLen, str.m_sz + nStart, nAppendLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(const CStrW &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nAppendLenW;
		size_t nAppendLen8;

		if(nLen == end)
		{
			nAppendLenW = str.m_nStrLen - nStart;
			nAppendLen8 = WideToUtf8(str.m_sz + nStart, nAppendLenW, nullptr, 0);
		}
		else
		{
			nAppendLenW = MIN(nLen, str.m_nStrLen - nStart);
			size_t nConvertLen8 = WideToUtf8(str.m_sz + nStart, nAppendLenW, nullptr, 0);
			nAppendLen8 = MIN(nLen, nConvertLen8);
		}

		size_t nNewStrLen = m_nStrLen + nAppendLen8;
		if(Alloc(nNewStrLen + 1))
		{
			WideToUtf8(str.m_sz + nStart, nAppendLenW, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull8;
		}
	}
}

//#################################################################################################
void CStr8::Append(PCSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull8 && nLen)
	{
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nAppendLen;
			if(nLen == end)
				nAppendLen = nStrLen - nStart;
			else
				nAppendLen = MIN(nLen, nStrLen - nStart);

			size_t nNewStrLen = m_nStrLen + nAppendLen;
			if(Alloc(nNewStrLen + 1))
			{
				std::memcpy(m_sz + m_nStrLen, sz + nStart, nAppendLen);
				m_nStrLen = nNewStrLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
		}
	}
}

//#################################################################################################
void CStr8::Append(PCWSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNullW && nLen)
	{
		size_t nStrLenW = (nLen == CStrW::end) ? StringGetLength(sz) : CStrW::GetLength(sz, nStart + nLen);
		if(nStart < nStrLenW)
		{
			size_t nAppendLenW;
			size_t nAppendLen8;

			if(nLen == end)
			{
				nAppendLenW = nStrLenW - nStart;
				nAppendLen8 = WideToUtf8(sz + nStart, nAppendLenW, nullptr, 0);
			}
			else
			{
				nAppendLenW = MIN(nLen, nStrLenW - nStart);
				size_t nConvertLen8 = WideToUtf8(sz + nStart, nAppendLenW, nullptr, 0);
				nAppendLen8 = MIN(nLen, nConvertLen8);
			}

			size_t nNewStrLen = m_nStrLen + nAppendLen8;
			if(Alloc(nNewStrLen + 1))
			{
				WideToUtf8(sz + nStart, nAppendLenW, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
				m_nStrLen = nNewStrLen;
				m_sz[m_nStrLen] = g_chNull8;
			}
		}
	}
}

//#################################################################################################
std::ostream &operator<<(std::ostream &stream, const CStr8 &str)
{
	stream << str.m_sz;
	return stream;
}

//#################################################################################################
bool CStr8::operator==(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) == 0);
}

//#################################################################################################
bool CStr8::operator==(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) == 0);
}

//#################################################################################################
bool CStr8::operator==(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) == 0);
}

//#################################################################################################
bool CStr8::operator!=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) != 0);
}

//#################################################################################################
bool CStr8::operator!=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) != 0);
}

//#################################################################################################
bool CStr8::operator!=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) != 0);
}

//#################################################################################################
bool CStr8::operator<(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) < 0);
}

//#################################################################################################
bool CStr8::operator<(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) < 0);
}

//#################################################################################################
bool CStr8::operator<(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) < 0);
}

//#################################################################################################
bool CStr8::operator<=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) <= 0);
}

//#################################################################################################
bool CStr8::operator<=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) <= 0);
}

//#################################################################################################
bool CStr8::operator<=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) <= 0);
}

//#################################################################################################
bool CStr8::operator>(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) > 0);
}

//#################################################################################################
bool CStr8::operator>(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) > 0);
}

//#################################################################################################
bool CStr8::operator>(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) > 0);
}

//#################################################################################################
bool CStr8::operator>=(const CStr8 &str) const
{
	return (StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) >= 0);
}

//#################################################################################################
bool CStr8::operator>=(PCSTR sz) const
{
	return (StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz)) >= 0);
}

//#################################################################################################
bool CStr8::operator>=(const char ch) const
{
	return (StringCompare8(m_sz, m_nStrLen, &ch, 1) >= 0);
}

//#################################################################################################
int CStr8::Compare(const CStr8 &str, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CStr8::Compare(PCSTR sz, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CStr8::Compare(const char ch, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
int CStr8::CompareAlphaNum(const CStr8 &str, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	static PCSTR szDigits8 = "0123456789";

	size_t nPos1 = 0;
	size_t nPos2 = 0;
	CStr8 strChunk1;
	CStr8 strChunk2;

	while(nCompareRtn == 0)
	{
		bool bNum1 = false;
		bool bNum2 = false;

		char ch1 = GetAt(nPos1);
		char ch2 = str.GetAt(nPos2);
		if(ch1 == g_chNull8 && ch2 == g_chNull8)
			break;

		if(std::memchr(szDigits8, ch1, 10))
		{
			strChunk1 = SpanIncluding(szDigits8, nPos1);
			bNum1 = true;
		}
		else
			strChunk1 = SpanExcluding(szDigits8, nPos1);
		nPos1 += strChunk1.GetLength();

		if(std::memchr(szDigits8, ch2, 10))
		{
			strChunk2 = str.SpanIncluding(szDigits8, nPos2);
			bNum2 = true;
		}
		else
			strChunk2 = str.SpanExcluding(szDigits8, nPos2);
		nPos2 += strChunk2.GetLength();

		if(bNum1 && bNum2)
		{
			uint64_t n1 = StringToInteger8<uint64_t>(strChunk1);
			uint64_t n2 = StringToInteger8<uint64_t>(strChunk2);

			if(n1 < n2)
				nCompareRtn = -1;
			else if(n1 > n2)
				nCompareRtn = 1;
		}
		else if(bNum1)
			nCompareRtn = -1;
		else if(bNum2)
			nCompareRtn = 1;
		else
			nCompareRtn = StringCompare8(strChunk1.m_sz, strChunk1.m_nStrLen, strChunk2.m_sz, strChunk2.m_nStrLen, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
int CStr8::CompareMid(const CStr8 &str, const size_t nStart, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
	nCompareRtn = StringCompare8(m_sz + nOffset, MIN(m_nStrLen - nOffset, nCount), str.m_sz, MIN(str.m_nStrLen, nCount), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr8::CompareMid(PCSTR sz, const size_t nStart, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = GetLength(sz, nCount);
	size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
	nCompareRtn = StringCompare8(m_sz + nOffset, MIN(m_nStrLen - nOffset, nCount), sz, nStrLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr8::CompareMid(const char ch, const size_t nStart, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNull8)
	{
		size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
		nCompareRtn = StringCompare8(m_sz + nOffset, MIN(m_nStrLen - nOffset, 1), &ch, 1, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
int CStr8::CompareLeft(const CStr8 &str, const size_t nCount, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, MIN(m_nStrLen, nCount), str.m_sz, MIN(str.m_nStrLen, nCount), bCaseInsensitive);
}

//#################################################################################################
int CStr8::CompareLeft(PCSTR sz, const size_t nCount, const bool bCaseInsensitive) const
{
	return StringCompare8(m_sz, MIN(m_nStrLen, nCount), sz, GetLength(sz, nCount), bCaseInsensitive);
}

//#################################################################################################
int CStr8::CompareLeft(const char ch, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNull8)
		nCompareRtn = StringCompare8(m_sz, MIN(m_nStrLen, 1), &ch, 1, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr8::CompareRight(const CStr8 &str, const size_t nCount, const bool bCaseInsensitive) const
{
	size_t nStrLen1 = MIN(m_nStrLen, nCount);
	size_t nStrLen2 = MIN(str.m_nStrLen, nCount);

	return StringCompare8(m_sz + (m_nStrLen - nStrLen1), nStrLen1, str.m_sz + (str.m_nStrLen - nStrLen2), nStrLen2, bCaseInsensitive);
}

//#################################################################################################
int CStr8::CompareRight(PCSTR sz, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(sz);
	size_t nStrLen1 = MIN(m_nStrLen, nCount);
	size_t nStrLen2 = MIN(nStrLen, nCount);
	nCompareRtn = StringCompare8(m_sz + (m_nStrLen - nStrLen1), nStrLen1, sz + (nStrLen - nStrLen2), nStrLen2, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr8::CompareRight(const char ch, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNull8)
	{
		size_t nStrLen1 = MIN(m_nStrLen, 1);

		nCompareRtn = StringCompare8(m_sz + (m_nStrLen - nStrLen1), nStrLen1, &ch, 1, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
size_t CStr8::Count(const char chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(m_nStrLen && nStart < m_nStrLen)
	{
		if(bCaseInsensitive)
		{
			char chLower = CharToLower(chFind);
			for(size_t n = nStart; n < m_nStrLen; ++n)
			{
				if(CharToLower(m_sz[n]) == chLower)
					++nCount;
			}
		}
		else
		{
			for(size_t n = nStart; n < m_nStrLen; ++n)
			{
				if(m_sz[n] == chFind)
					++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::CountCompare(const CStr8 &str, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(nStart < m_nStrLen)
	{
		PCSTR szThis = m_sz + nStart;
		PCSTR szThat = str.m_sz;

		if(bCaseInsensitive)
		{	// This is not UTF8 compliant, but it does not make sense to convert to wchar_t for the check as that may alter the character count
			while(*szThis != g_chNull8 && *szThat != g_chNull8 && CharToLower(*szThis++) == CharToLower(*szThat++))
				++nCount;
		}
		else
		{
			while(*szThis != g_chNull8 && *szThat != g_chNull8 && *szThis++ == *szThat++)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::CountCompare(PCSTR sz, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(nStart < m_nStrLen && sz)
	{
		PCSTR szThis = m_sz + nStart;
		PCSTR szThat = sz;

		if(bCaseInsensitive)
		{	// This is not UTF8 compliant, but it does not make sense to convert to wchar_t for the check as that may alter the character count
			while(*szThis != g_chNull8 && *szThat != g_chNull8 && CharToLower(*szThis++) == CharToLower(*szThat++))
				++nCount;
		}
		else
		{
			while(*szThis != g_chNull8 && *szThat != g_chNull8 && *szThis++ == *szThat++)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::ReverseCountCompare(const CStr8 &str, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(m_nStrLen && str.m_nStrLen)
	{
		PCSTR szThis = (nStart >= m_nStrLen) ? (m_sz + m_nStrLen - 1) : (m_sz + nStart);
		PCSTR szThat = str.m_sz + str.m_nStrLen - 1;

		if(bCaseInsensitive)
		{	// This is not UTF8 compliant, but it does not make sense to convert to wchar_t for the check as that may alter the character count
			while(szThis != m_sz && szThat != str.m_sz && CharToLower(*szThis--) == CharToLower(*szThat--))
				++nCount;
		}
		else
		{
			while(szThis != m_sz && szThat != str.m_sz && *szThis-- == *szThat--)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::ReverseCountCompare(PCSTR sz, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(m_nStrLen)
	{
		PCSTR szThis = (nStart >= m_nStrLen) ? (m_sz + m_nStrLen - 1) : (m_sz + nStart);
		PCSTR szThat = sz + StringGetLength(sz) - 1;

		if(bCaseInsensitive)
		{	// This is not UTF8 compliant, but it does not make sense to convert to wchar_t for the check as that may alter the character count
			while(szThis != m_sz && szThat != sz && CharToLower(*szThis--) == CharToLower(*szThat--))
				++nCount;
		}
		else
		{
			while(szThis != m_sz && szThat != sz && *szThis-- == *szThat--)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
bool CStr8::Contains(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return false;
	else if(strFind.m_nStrLen == 0)
		return true;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);

	return (nPos != not_found);
}

//#################################################################################################
bool CStr8::Contains(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return false;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return true;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);

	return (nPos != not_found);
}

//#################################################################################################
bool CStr8::Contains(const char chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return false;

	PCSTR sz;
	if(bCaseInsensitive)
		sz = memchri(m_sz + nStart, chFind, m_nStrLen - nStart);
	else
		sz = (PCSTR)std::memchr(m_sz + nStart, chFind, m_nStrLen - nStart);

	return (sz != nullptr);
}

//#################################################################################################
bool CStr8::ContainsOneOf(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return false;
	else if(strFind.m_nStrLen == 0)
		return true;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(memchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return true;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::memchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return true;
		}
	}

	return false;
}

//#################################################################################################
bool CStr8::ContainsOneOf(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return false;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return true;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(memchri(szFind, m_sz[nIndex], nFindLen))
				return true;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::memchr(szFind, m_sz[nIndex], nFindLen))
				return true;
		}
	}

	return false;
}

//#################################################################################################
size_t CStr8::Find(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);

	return (nPos == not_found) ? not_found : nPos + nStart;
}

//#################################################################################################
size_t CStr8::Find(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);

	return (nPos == not_found) ? not_found : nPos + nStart;
}

//#################################################################################################
size_t CStr8::Find(const char chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return not_found;

	PCSTR sz;
	if(bCaseInsensitive)
		sz = memchri(m_sz + nStart, chFind, m_nStrLen - nStart);
	else
		sz = (PCSTR)std::memchr(m_sz + nStart, chFind, m_nStrLen - nStart);

	return (sz == nullptr) ? not_found : (size_t)(sz - m_sz);
}

//#################################################################################################
size_t CStr8::ReverseFind(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen && nStart)
	{
		if(strFind.m_nStrLen == 0)
			return nStart;	// Null string always matches

		size_t nIndex;
		if(nStart >= m_nStrLen)
		{
			if(strFind.m_nStrLen > m_nStrLen)
				return not_found;
			nIndex = m_nStrLen - strFind.m_nStrLen;
		}
		else
		{
			if(strFind.m_nStrLen > nStart)
				return not_found;
			nIndex = nStart - strFind.m_nStrLen;
		}

		if(bCaseInsensitive)
		{
			do{
				if(memcmpi(m_sz + nIndex, strFind, strFind.m_nStrLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
		else
		{
			do{
				if(std::memcmp(m_sz + nIndex, strFind, strFind.m_nStrLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::ReverseFind(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen && nStart)
	{
		size_t nFindLen = StringGetLength(szFind);
		if(nFindLen == 0)
			return nStart;	// Null string always matches

		size_t nIndex;
		if(nStart >= m_nStrLen)
		{
			if(nFindLen > m_nStrLen)
				return not_found;
			nIndex = m_nStrLen - nFindLen;
		}
		else
		{
			if(nFindLen > nStart)
				return not_found;
			nIndex = nStart - nFindLen;
		}

		if(bCaseInsensitive)
		{
			do{
				if(memcmpi(m_sz + nIndex, szFind, nFindLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
		else
		{
			do{
				if(std::memcmp(m_sz + nIndex, szFind, nFindLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::ReverseFind(const char chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen && nStart)
	{
		size_t nIndex;
		if(nStart >= m_nStrLen)
			nIndex = m_nStrLen - 1;
		else
			nIndex = nStart - 1;

		if(bCaseInsensitive)
		{
			char chLower = CharToLower(chFind);
			do{
				if(CharToLower(m_sz[nIndex]) == chLower)
					return nIndex;
			}while(nIndex-- != 0);
		}
		else
		{
			do{
				if(m_sz[nIndex] == chFind)
					return nIndex;
			}while(nIndex-- != 0);
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindFirstOf(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(memchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::memchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindFirstOf(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(memchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::memchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindFirstNotOf(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!memchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!std::memchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindFirstNotOf(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!memchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!std::memchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindLastOf(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(memchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(std::memchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindLastOf(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(memchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(std::memchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindLastNotOf(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(!memchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(!std::memchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindLastNotOf(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(!memchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(!std::memchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
CStr8 CStr8::SpanIncluding(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstNotOf(strFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
CStr8 CStr8::SpanIncluding(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstNotOf(szFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
CStr8 CStr8::SpanExcluding(const CStr8 &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstOf(strFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
CStr8 CStr8::SpanExcluding(PCSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstOf(szFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
size_t CStr8::Insert(const size_t nIndex, const CStr8 &strInsert)
{
	if(nIndex >= m_nStrLen)
		Append(strInsert);
	else if(strInsert.m_nStrLen != 0)
	{
		size_t nNewStrLen = m_nStrLen + strInsert.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memmove(m_sz + nIndex + strInsert.m_nStrLen, m_sz + nIndex, m_nStrLen + 1 - nIndex);
			std::memcpy(m_sz + nIndex, strInsert.m_sz, strInsert.m_nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStr8::Insert(const size_t nIndex, PCSTR szInsert)
{
	if(nIndex >= m_nStrLen)
		Append(szInsert);
	else if(szInsert != nullptr && *szInsert != g_chNull8)
	{
		size_t nStrLen = StringGetLength(szInsert);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memmove(m_sz + nIndex + nStrLen, m_sz + nIndex, m_nStrLen + 1 - nIndex);
			std::memcpy(m_sz + nIndex, szInsert, nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStr8::Insert(const size_t nIndex, const char chInsert)
{
	if(nIndex >= m_nStrLen)
		Append(chInsert);
	else if(chInsert != g_chNull8)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			std::memmove(m_sz + nIndex + 1, m_sz + nIndex, m_nStrLen + 1 - nIndex);
			m_sz[nIndex] = chInsert;
			m_nStrLen = nNewStrLen;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStr8::Delete(const size_t nIndex, const size_t nCount)
{
	if(nIndex < m_nStrLen)
	{
		if(nIndex == 0 && (nCount == end || nCount >= m_nStrLen))
			Empty();
		else if(nCount == end || nIndex + nCount >= m_nStrLen)
		{	// If nCount is -1 or nIndex + nCount is too long, delete the last part of the string
			m_nStrLen = nIndex;
			m_sz[m_nStrLen] = g_chNull8;
		}
		else if(nCount)
		{	// Remove chars out of the middle
			std::memmove(m_sz + nIndex, m_sz + nIndex + nCount, m_nStrLen + 1 - nIndex - nCount);
			m_nStrLen -= nCount;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStr8::Remove(const CStr8 &strRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen && strRemove.m_nStrLen)
	{
		if(bCaseInsensitive)
		{
			PSTR szScan = FindStrPtrI(m_sz, m_nStrLen, strRemove.m_sz, strRemove.m_nStrLen);
			while(szScan)
			{
				std::memmove(szScan, szScan + strRemove.m_nStrLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - strRemove.m_nStrLen);
				m_nStrLen -= strRemove.m_nStrLen;
				++nCount;

				szScan = FindStrPtrI(szScan, m_nStrLen - (size_t)(szScan - m_sz), strRemove.m_sz, strRemove.m_nStrLen);
			}
		}
		else
		{
			PSTR szScan = FindStrPtr(m_sz, m_nStrLen, strRemove.m_sz, strRemove.m_nStrLen);
			while(szScan)
			{
				std::memmove(szScan, szScan + strRemove.m_nStrLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - strRemove.m_nStrLen);
				m_nStrLen -= strRemove.m_nStrLen;
				++nCount;

				szScan = FindStrPtr(szScan, m_nStrLen - (size_t)(szScan - m_sz), strRemove.m_sz, strRemove.m_nStrLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::Remove(PCSTR szRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	size_t nRemoveLen = StringGetLength(szRemove);
	if(m_nStrLen && nRemoveLen)
	{
		if(bCaseInsensitive)
		{
			PSTR szScan = FindStrPtrI(m_sz, m_nStrLen, szRemove, nRemoveLen);
			while(szScan)
			{
				std::memmove(szScan, szScan + nRemoveLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - nRemoveLen);
				m_nStrLen -= nRemoveLen;
				++nCount;

				szScan = FindStrPtrI(szScan, m_nStrLen - (size_t)(szScan - m_sz), szRemove, nRemoveLen);
			}
		}
		else
		{
			PSTR szScan = FindStrPtr(m_sz, m_nStrLen, szRemove, nRemoveLen);
			while(szScan)
			{
				std::memmove(szScan, szScan + nRemoveLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - nRemoveLen);
				m_nStrLen -= nRemoveLen;
				++nCount;

				szScan = FindStrPtr(szScan, m_nStrLen - (size_t)(szScan - m_sz), szRemove, nRemoveLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::Remove(const char chRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen && chRemove != g_chNull8)
	{
		PSTR szWrite = m_sz;
		PSTR szScan = m_sz;

		if(bCaseInsensitive)
		{
			char chLower = CharToLower(chRemove);
			while(*szScan != g_chNull8)
			{
				if(CharToLower(*szScan) != chLower)
					*szWrite++ = *szScan;
				else
					++nCount;
				++szScan;
			}
		}
		else
		{
			while(*szScan != g_chNull8)
			{
				if(*szScan != chRemove)
					*szWrite++ = *szScan;
				else
					++nCount;
				++szScan;
			}
		}

		*szWrite = g_chNull8;
		m_nStrLen -= nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::RemoveAllOf(const CStr8 &strRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0 || strRemove.m_nStrLen == 0)
		return nCount;

	PSTR szWrite = m_sz;
	PSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNull8)
		{
			if(!memchri(strRemove.m_sz, *szScan, strRemove.m_nStrLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen -= nCount;
	}
	else
	{
		while(*szScan != g_chNull8)
		{
			if(!std::memchr(strRemove.m_sz, *szScan, strRemove.m_nStrLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen -= nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::RemoveAllOf(PCSTR szRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0 || szRemove == nullptr || *szRemove == g_chNull8)
		return nCount;

	size_t nRemoveLen = StringGetLength(szRemove);
	PSTR szWrite = m_sz;
	PSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNull8)
		{
			if(!memchri(szRemove, *szScan, nRemoveLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen -= nCount;
	}
	else
	{
		while(*szScan != g_chNull8)
		{
			if(!std::memchr(szRemove, *szScan, nRemoveLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen -= nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::KeepAllOf(const CStr8 &strKeep, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0)
		return nCount;

	if(strKeep.m_nStrLen == 0)
	{
		Empty();
		return nCount;
	}

	PSTR szWrite = m_sz;
	PSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNull8)
		{
			if(memchri(strKeep.m_sz, *szScan, strKeep.m_nStrLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen = nCount;
	}
	else
	{
		while(*szScan != g_chNull8)
		{
			if(std::memchr(strKeep.m_sz, *szScan, strKeep.m_nStrLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen = nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::KeepAllOf(PCSTR szKeep, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0)
		return nCount;

	if(szKeep == nullptr || *szKeep == g_chNull8)
	{
		Empty();
		return nCount;
	}

	size_t nKeepLen = StringGetLength(szKeep);
	PSTR szWrite = m_sz;
	PSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNull8)
		{
			if(memchri(szKeep, *szScan, nKeepLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen = nCount;
	}
	else
	{
		while(*szScan != g_chNull8)
		{
			if(std::memchr(szKeep, *szScan, nKeepLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNull8;
		m_nStrLen = nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::Replace(const CStr8 &strOld, const CStr8 &strNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!m_nStrLen || !strOld.m_nStrLen)
		return nCount;

	if(strOld.m_nStrLen != strNew.m_nStrLen)
	{	// The length of the string is changing
		if(bCaseInsensitive)
		{
			size_t nNewStrLen = m_nStrLen;
			PSTR szScan = FindStrPtrI(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				nNewStrLen -= strOld.m_nStrLen;
				nNewStrLen += strNew.m_nStrLen;
				szScan = FindStrPtrI(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz), strOld.m_sz, strOld.m_nStrLen);
			}

			auto szTemp = std::make_unique<char[]>(m_nStrLen);
			std::memcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PSTR szDest = m_sz;
				PCSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtrI(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), strOld.m_sz, strOld.m_nStrLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::memcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::memcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(strNew.m_nStrLen)
						{
							std::memcpy(szDest, strNew.m_sz, strNew.m_nStrLen);
							m_nStrLen += strNew.m_nStrLen;
							szDest += strNew.m_nStrLen;
						}
						++nCount;
						szSrc = szScan + strOld.m_nStrLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNull8;
			}
		}
		else
		{
			size_t nNewStrLen = m_nStrLen;
			PSTR szScan = FindStrPtr(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				nNewStrLen -= strOld.m_nStrLen;
				nNewStrLen += strNew.m_nStrLen;
				szScan = FindStrPtr(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz), strOld.m_sz, strOld.m_nStrLen);
			}

			auto szTemp = std::make_unique<char[]>(m_nStrLen);
			std::memcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PSTR szDest = m_sz;
				PCSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtr(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), strOld.m_sz, strOld.m_nStrLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::memcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::memcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(strNew.m_nStrLen)
						{
							std::memcpy(szDest, strNew.m_sz, strNew.m_nStrLen);
							m_nStrLen += strNew.m_nStrLen;
							szDest += strNew.m_nStrLen;
						}
						++nCount;
						szSrc = szScan + strOld.m_nStrLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNull8;
			}
		}
	}
	else
	{	// No change in length, perform the replace in the current memory
		if(bCaseInsensitive)
		{
			PSTR szScan = FindStrPtrI(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				std::memcpy(szScan, strNew.m_sz, strNew.m_nStrLen);
				++nCount;
				szScan = FindStrPtrI(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz) - strOld.m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			}
		}
		else
		{
			PSTR szScan = FindStrPtr(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				std::memcpy(szScan, strNew.m_sz, strNew.m_nStrLen);
				++nCount;
				szScan = FindStrPtr(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz) - strOld.m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::Replace(PCSTR szOld, PCSTR szNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!m_nStrLen)
		return nCount;

	size_t nOldLen = StringGetLength(szOld);
	size_t nNewLen = StringGetLength(szNew);
	if(nOldLen != nNewLen)
	{	// The length of the string is changing
		if(bCaseInsensitive)
		{
			size_t nNewStrLen = m_nStrLen;
			PSTR szScan = FindStrPtrI(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				nNewStrLen -= nOldLen;
				nNewStrLen += nNewLen;
				szScan = FindStrPtrI(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz), szOld, nOldLen);
			}

			auto szTemp = std::make_unique<char[]>(m_nStrLen);
			std::memcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PSTR szDest = m_sz;
				PCSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtrI(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), szOld, nOldLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::memcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::memcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(nNewLen)
						{
							std::memcpy(szDest, szNew, nNewLen);
							m_nStrLen += nNewLen;
							szDest += nNewLen;
						}
						++nCount;
						szSrc = szScan + nOldLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNull8;
			}
		}
		else
		{
			size_t nNewStrLen = m_nStrLen;
			PSTR szScan = FindStrPtr(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				nNewStrLen -= nOldLen;
				nNewStrLen += nNewLen;
				szScan = FindStrPtr(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz), szOld, nOldLen);
			}

			auto szTemp = std::make_unique<char[]>(m_nStrLen);
			std::memcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PSTR szDest = m_sz;
				PCSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtr(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), szOld, nOldLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::memcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::memcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(nNewLen)
						{
							std::memcpy(szDest, szNew, nNewLen);
							m_nStrLen += nNewLen;
							szDest += nNewLen;
						}
						++nCount;
						szSrc = szScan + nOldLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNull8;
			}
		}
	}
	else
	{	// No change in length, perform the replace in the current memory
		if(bCaseInsensitive)
		{
			PSTR szScan = FindStrPtrI(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				std::memcpy(szScan, szNew, nNewLen);
				++nCount;
				szScan = FindStrPtrI(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz) - nOldLen, szOld, nOldLen);
			}
		}
		else
		{
			PSTR szScan = FindStrPtr(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				std::memcpy(szScan, szNew, nNewLen);
				++nCount;
				szScan = FindStrPtr(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz) - nOldLen, szOld, nOldLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStr8::Replace(const char chOld, const char chNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen && chOld != chNew && chNew != g_chNull8)
	{
		PSTR szScan = m_sz;

		if(bCaseInsensitive)
		{
			char chLower = CharToLower(chOld);
			while(*szScan != g_chNull8)
			{
				if(CharToLower(*szScan) == chLower)
				{
					*szScan = chNew;
					++nCount;
				}
				++szScan;
			}
		}
		else
		{
			while(*szScan != g_chNull8)
			{
				if(*szScan == chOld)
				{
					*szScan = chNew;
					++nCount;
				}
				++szScan;
			}
		}
	}

	return nCount;
}

//#################################################################################################
void CStr8::Swap(CStr8 &src) noexcept
{
	SWAP(m_sz, src.m_sz);
	SWAP(m_nBufLen, src.m_nBufLen);
	SWAP(m_nStrLen, src.m_nStrLen);
}

//#################################################################################################
CStr8 CStr8::GetMid(const size_t nStart, const size_t nCount) const
{
	CStr8 str;

	if(nStart < m_nStrLen)
	{	// Copy nCount chars, unless nCount is zero or nStart + nCount is too much
		size_t nStrLen = (nCount == end || nStart + nCount > m_nStrLen) ? m_nStrLen - nStart : nCount;
		if(str.Alloc(nStrLen + 1))
		{
			std::memcpy(str.m_sz, m_sz + nStart, nStrLen);
			str.m_nStrLen = nStrLen;
			str.m_sz[str.m_nStrLen] = g_chNull8;
		}
	}

	return str;
}

//#################################################################################################
CStr8 CStr8::GetLeft(const size_t nCount) const
{
	CStr8 str;

	if(nCount >= m_nStrLen)
		str.Assign(*this);
	else
	{
		if(str.Alloc(nCount + 1))
		{
			std::memcpy(str.m_sz, m_sz, nCount);
			str.m_nStrLen = nCount;
			str.m_sz[str.m_nStrLen] = g_chNull8;
		}
	}

	return str;
}

//#################################################################################################
CStr8 CStr8::GetRight(const size_t nCount) const
{
	CStr8 str;

	if(nCount >= m_nStrLen)
		str.Assign(*this);
	else
	{
		if(str.Alloc(nCount + 1))
		{
			std::memcpy(str.m_sz, m_sz + m_nStrLen - nCount, nCount);
			str.m_nStrLen = nCount;
			str.m_sz[str.m_nStrLen] = g_chNull8;
		}
	}

	return str;
}

//#################################################################################################
CStr8 &CStr8::Trim(const CStr8 &strTrim, const bool bCaseInsensitive)
{
	TrimLeft(strTrim, bCaseInsensitive);
	TrimRight(strTrim, bCaseInsensitive);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::Trim(PCSTR szTrim, const bool bCaseInsensitive)
{
	TrimLeft(szTrim, bCaseInsensitive);
	TrimRight(szTrim, bCaseInsensitive);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::Trim(const char chTrim, const bool bCaseInsensitive)
{
	TrimLeft(chTrim, bCaseInsensitive);
	TrimRight(chTrim, bCaseInsensitive);
	return *this;
}

//#################################################################################################
CStr8 &CStr8::Trim(void)
{
	TrimLeft();
	TrimRight();
	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimLeft(const CStr8 &strTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || strTrim.m_nStrLen == 0)
		return *this;

	// Find first non-matching character
	PCSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNull8 && memchri(strTrim.m_sz, *szScan, strTrim.m_nStrLen))
			++szScan;
	}
	else
	{
		while(*szScan != g_chNull8 && std::memchr(strTrim.m_sz, *szScan, strTrim.m_nStrLen))
			++szScan;
	}

	if(*szScan == g_chNull8)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::memmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimLeft(PCSTR szTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || szTrim == nullptr || *szTrim == g_chNull8)
		return *this;

	size_t nTrimLen = StringGetLength(szTrim);

	// Find first non-matching character
	PCSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNull8 && memchri(szTrim, *szScan, nTrimLen))
			++szScan;
	}
	else
	{
		while(*szScan != g_chNull8 && std::memchr(szTrim, *szScan, nTrimLen))
			++szScan;
	}

	if(*szScan == g_chNull8)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::memmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimLeft(const char chTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || chTrim == g_chNull8)
		return *this;

	// Find first non-matching character
	PCSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		char chLower = CharToLower(chTrim);
		while(*szScan != g_chNull8 && CharToLower(*szScan) == chLower)
			++szScan;
	}
	else
	{
		while(*szScan != g_chNull8 && *szScan == chTrim)
			++szScan;
	}

	if(*szScan == g_chNull8)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::memmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimLeft(void)
{
	if(m_nStrLen == 0)
		return *this;

	// Find first non-space character
	PCSTR szScan = m_sz;
	while(*szScan != g_chNull8 && IsSpace8(*szScan))
		++szScan;

	if(*szScan == g_chNull8)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::memmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimRight(const CStr8 &strTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || strTrim.m_nStrLen == 0)
		return *this;

	// Find last non-matching starting at the end
	PSTR szScan = m_sz + m_nStrLen;
	if(bCaseInsensitive)
	{
		while(szScan != m_sz && memchri(strTrim.m_sz, *(szScan - 1), strTrim.m_nStrLen))
			--szScan;
	}
	else
	{
		while(szScan != m_sz && std::memchr(strTrim.m_sz, *(szScan - 1), strTrim.m_nStrLen))
			--szScan;
	}

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNull8;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimRight(PCSTR szTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || szTrim == nullptr || *szTrim == g_chNull8)
		return *this;

	size_t nTrimLen = StringGetLength(szTrim);

	// Find last non-matching starting at the end
	PSTR szScan = m_sz + m_nStrLen;
	if(bCaseInsensitive)
	{
		while(szScan != m_sz && memchri(szTrim, *(szScan - 1), nTrimLen))
			--szScan;
	}
	else
	{
		while(szScan != m_sz && std::memchr(szTrim, *(szScan - 1), nTrimLen))
			--szScan;
	}

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNull8;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimRight(const char chTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || chTrim == g_chNull8)
		return *this;

	// Find last non-matching starting at the end
	PSTR szScan = m_sz + m_nStrLen;
	if(bCaseInsensitive)
	{
		char chLower = CharToLower(chTrim);
		while(szScan != m_sz && CharToLower(*(szScan - 1)) == chLower)
			--szScan;
	}
	else
	{
		while(szScan != m_sz && *(szScan - 1) == chTrim)
			--szScan;
	}

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNull8;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::TrimRight(void)
{
	if(m_nStrLen == 0)
		return *this;

	// Find last non-space starting at the end
	PSTR szScan = m_sz + m_nStrLen;
	while(szScan != m_sz && IsSpace8(*(szScan - 1)))
		--szScan;

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNull8;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::CropMid(const size_t nStart, const size_t nCount)
{
	if(nStart >= m_nStrLen || nCount == 0)
		Empty();
	else
	{
		m_nStrLen = MIN(nCount, m_nStrLen - nStart);
		std::memmove(m_sz, m_sz + nStart, m_nStrLen);
		m_sz[m_nStrLen] = g_chNull8;
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::CropLeft(const size_t nCount)
{
	if(nCount == 0)
		Empty();
	else if(nCount < m_nStrLen)
	{
		m_nStrLen = nCount;
		m_sz[m_nStrLen] = g_chNull8;
	}

	return *this;
}

//#################################################################################################
CStr8 &CStr8::CropRight(const size_t nCount)
{
	if(nCount == 0)
		Empty();
	else if(nCount < m_nStrLen)
	{
		std::memmove(m_sz, m_sz + m_nStrLen - nCount, nCount + 1);
		m_nStrLen = nCount;
	}

	return *this;
}

//#################################################################################################
CStr8 CStr8::GetUpper(void) const
{
	CStr8 str(*this);
	StringToUpper(str.m_sz, str.m_nStrLen);

	return str;
}

//#################################################################################################
CStr8 CStr8::GetLower(void) const
{
	CStr8 str(*this);
	StringToLower(str.m_sz, str.m_nStrLen);

	return str;
}

//#################################################################################################
CStr8 &CStr8::MakeUpper(void)
{
	StringToUpper(m_sz, m_nStrLen);

	return *this;
}

//#################################################################################################
CStr8 &CStr8::MakeLower(void)
{
	StringToLower(m_sz, m_nStrLen);

	return *this;
}

//#################################################################################################
CStr8 &CStr8::MakeReverse(void)
{
	if(m_nStrLen > 1)
	{
		PSTR szFront = m_sz;
		PSTR szBack = m_sz + m_nStrLen - 1;
		do{
			SWAP(*szFront, *szBack);
		}while(++szFront < --szBack);
	}

	return *this;
}

//#################################################################################################
CStr8 CStr8::Tokenize(const CStr8 &strTokens, size_t &nStart, const bool bCaseInsensitive) const
{
	if(strTokens.m_nStrLen == 0)
		return *this;

	if(nStart < m_nStrLen)
	{
		size_t nBegin = nStart;
		if(bCaseInsensitive)
		{
			while(nBegin < m_nStrLen)
			{
				if(!memchri(strTokens.m_sz, m_sz[nBegin], strTokens.m_nStrLen))
					break;
				++nBegin;
			}
		}
		else
		{
			while(nBegin < m_nStrLen)
			{
				if(!std::memchr(strTokens.m_sz, m_sz[nBegin], strTokens.m_nStrLen))
					break;
				++nBegin;
			}
		}

		if(nBegin < m_nStrLen)
		{
			size_t nEnd = nBegin + 1;
			if(bCaseInsensitive)
			{
				while(nEnd < m_nStrLen)
				{
					if(memchri(strTokens.m_sz, m_sz[nEnd], strTokens.m_nStrLen))
						break;
					++nEnd;
				}
			}
			else
			{
				while(nEnd < m_nStrLen)
				{
					if(std::memchr(strTokens.m_sz, m_sz[nEnd], strTokens.m_nStrLen))
						break;
					++nEnd;
				}
			}

			nStart = nEnd + 1;
			return GetMid(nBegin, nEnd - nBegin);
		}
	}

	// Nothing left to tokenize
	nStart = not_found;
	return CStr8();
}

//#################################################################################################
CStr8 CStr8::Tokenize(PCSTR szTokens, size_t &nStart, const bool bCaseInsensitive) const
{
	if(szTokens == nullptr || *szTokens == g_chNull8)
		return *this;

	if(nStart < m_nStrLen)
	{
		size_t nTokenLen = StringGetLength(szTokens);
		size_t nBegin = nStart;
		if(bCaseInsensitive)
		{
			while(nBegin < m_nStrLen)
			{
				if(!memchri(szTokens, m_sz[nBegin], nTokenLen))
					break;
				++nBegin;
			}
		}
		else
		{
			while(nBegin < m_nStrLen)
			{
				if(!std::memchr(szTokens, m_sz[nBegin], nTokenLen))
					break;
				++nBegin;
			}
		}

		if(nBegin < m_nStrLen)
		{
			size_t nEnd = nBegin + 1;
			if(bCaseInsensitive)
			{
				while(nEnd < m_nStrLen)
				{
					if(memchri(szTokens, m_sz[nEnd], nTokenLen))
						break;
					++nEnd;
				}
			}
			else
			{
				while(nEnd < m_nStrLen)
				{
					if(std::memchr(szTokens, m_sz[nEnd], nTokenLen))
						break;
					++nEnd;
				}
			}

			nStart = nEnd + 1;
			return GetMid(nBegin, nEnd - nBegin);
		}
	}

	// Nothing left to tokenize
	nStart = not_found;
	return CStr8();
}

//#################################################################################################
CStr8 CStr8::Tokenize(const char chToken, size_t &nStart, const bool bCaseInsensitive) const
{
	if(chToken == g_chNull8)
		return *this;

	if(nStart < m_nStrLen)
	{
		size_t nBegin = nStart;
		if(bCaseInsensitive)
		{
			char chLower = CharToLower(chToken);
			while(nBegin < m_nStrLen)
			{
				if(CharToLower(m_sz[nBegin]) != chLower)
					break;
				++nBegin;
			}
		}
		else
		{
			while(nBegin < m_nStrLen)
			{
				if(chToken != m_sz[nBegin])
					break;
				++nBegin;
			}
		}

		if(nBegin < m_nStrLen)
		{
			size_t nEnd = nBegin + 1;
			if(bCaseInsensitive)
			{
				char chLower = CharToLower(chToken);
				while(nEnd < m_nStrLen)
				{
					if(CharToLower(m_sz[nEnd]) == chLower)
						break;
					++nEnd;
				}
			}
			else
			{
				while(nEnd < m_nStrLen)
				{
					if(chToken == m_sz[nEnd])
						break;
					++nEnd;
				}
			}

			nStart = nEnd + 1;
			return GetMid(nBegin, nEnd - nBegin);
		}
	}

	// Nothing left to tokenize
	nStart = not_found;
	return CStr8();
}

//#################################################################################################
bool CStr8::Printf(PCSTR szFormat, ...)
{
	bool bFormatted;

	va_list vaArgs;
	va_start(vaArgs, szFormat);
	bFormatted = PrintfV(szFormat, vaArgs);
	va_end(vaArgs);

	return bFormatted;
}

//#################################################################################################
bool CStr8::PrintfV(PCSTR szFormat, va_list vaArgs)
{
	bool bFormatted = true;

	if(szFormat)
	{
		size_t nStrLen = 32;
		va_list vaBegin;
		va_copy(vaBegin, vaArgs);

		int nResult = std::vsnprintf(GetBuffer(nStrLen), nStrLen, szFormat, vaArgs);
		if(nResult < 0)
		{
			Empty();
			bFormatted = false;
		}
		else
		{
			if((size_t)nResult > nStrLen - 1)
			{
				va_end(vaArgs);
				va_copy(vaArgs, vaBegin);
				nStrLen = (size_t)nResult + 1;
				std::vsnprintf(GetBuffer(nStrLen), nStrLen, szFormat, vaArgs);
			}

			ReleaseBuffer();
		}

		va_end(vaBegin);
	}
	else
	{
		Empty();
		bFormatted = false;
	}

	return bFormatted;
}

//#################################################################################################
bool CStr8::ReserveBuffer(const size_t nBufLen)
{
	bool bAllocated;

	bool bPrevAllocated = (m_nBufLen != 0);
	bAllocated = Alloc(nBufLen);
	if(!bPrevAllocated && bAllocated)
	{
		*m_sz = g_chNull8;
		m_nStrLen = 0;
	}

	return bAllocated;
}

//#################################################################################################
size_t CStr8::GetBufferLength(void) const noexcept
{
	return m_nBufLen;
}

//#################################################################################################
size_t CStr8::GetBufferSize(void) const noexcept
{
	return m_nBufLen;
}

//#################################################################################################
PSTR CStr8::GetBuffer(const size_t nMinBufLen)
{
	Alloc(nMinBufLen);
	return m_sz;
}

//#################################################################################################
void CStr8::ReleaseBuffer(const size_t nNewStrLen)
{	// Calculate string length if new string length was not provided
	size_t nStrLen;
	if(nNewStrLen == end)
		nStrLen = StringGetLength(m_sz);
	else
		nStrLen = nNewStrLen;
	m_nStrLen = (nStrLen < m_nBufLen) ? nStrLen : m_nBufLen - 1;
	m_sz[m_nStrLen] = g_chNull8;
}

//#################################################################################################
void CStr8::FreeExtra(void)
{
	if(m_nStrLen == 0)
		Dealloc();
	else if(m_nStrLen + 1 < m_nBufLen)
	{
		PSTR sz = (PSTR)std::malloc(m_nStrLen + 1);
		if(sz)
		{
			std::memcpy(sz, m_sz, m_nStrLen + 1);
			std::free(m_sz);
			m_sz = sz;
			m_nBufLen = m_nStrLen + 1;
		}
	}
}

//#################################################################################################
void CStr8::DebugPrint(const bool bAppendNewline, const bool bConsolePrint) const
{
	if(m_nStrLen)
	{
		if(bConsolePrint)
		{
			std::cout << m_sz;
			if(bAppendNewline)
				std::cout << std::endl;
		}
		else
		{
			if(!bAppendNewline)
				OutputDebugStringA(m_sz);
			else
			{
				auto szTemp = std::make_unique<char[]>(m_nStrLen + g_nEolLen + 1);
				std::memcpy(szTemp.get(), m_sz, m_nStrLen);
				std::memcpy(szTemp.get() + m_nStrLen, g_szEol8, g_nEolLen);
				szTemp[m_nStrLen + g_nEolLen] = g_chNull8;

				OutputDebugStringA(szTemp.get());
			}
		}
	}
}

//#################################################################################################
bool CStr8::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		PSTR sz;
		if(m_nBufLen)
			sz = (PSTR)std::realloc(m_sz, nBufLen);
		else
			sz = (PSTR)std::malloc(nBufLen);

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CStr8::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PSTR)g_szNull8;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
size_t CStr8::GetLength(PCSTR sz, const size_t nMaxLen)
{
	size_t n = 0;

	if(sz)
	{
		size_t nLen = nMaxLen;
		while(nLen-- && *sz++ != g_chNull8)
			++n;
	}

	return n;
}

//#################################################################################################
PSTR CStr8::FindStrPtr(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCSTR szScan1 = sz;
		PSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PSTR)std::memchr(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(std::memcmp(szScan2, szFind, nFindLen) == 0)
				return szScan2;

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return nullptr;
}

//#################################################################################################
PSTR CStr8::FindStrPtrI(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCSTR szScan1 = sz;
		PSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PSTR)memchri(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(memcmpi(szScan2, szFind, nFindLen) == 0)
				return szScan2;

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return nullptr;
}

//#################################################################################################
size_t CStr8::FindStrIndex(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCSTR szScan1 = sz;
		PCSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PCSTR)std::memchr(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(std::memcmp(szScan2, szFind, nFindLen) == 0)
				return (size_t)(szScan2 - sz);

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStr8::FindStrIndexI(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCSTR szScan1 = sz;
		PCSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PCSTR)memchri(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(memcmpi(szScan2, szFind, nFindLen) == 0)
				return (size_t)(szScan2 - sz);

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return not_found;
}

//#################################################################################################
PCSTR CStr8::memchri(PCSTR sz, const char ch, const size_t nLen)
{
	if(nLen)
	{
		PCSTR szScan = sz;
		size_t nCount = 0;

		char chLower = CharToLower(ch);
		do{
			if(CharToLower(*szScan) == chLower)
				return szScan;
			++szScan;
		}while(++nCount < nLen);
	}

	return nullptr;
}

//#################################################################################################
int CStr8::memcmpi(PCSTR sz1, PCSTR sz2, const size_t nLen)
{
	int nResult = 0;

	if(nLen)
	{
		PCSTR szScan1 = sz1;
		PCSTR szScan2 = sz2;
		size_t nCount = 0;

		do{
			nResult = (int)CharToLower(*szScan1++) - (int)CharToLower(*szScan2++);
		}while(nResult == 0 && ++nCount < nLen);
	}

	return nResult;
}

//#################################################################################################
size_t CStr8::str2int(PCSTR sz)
{
	return StringToInteger8<size_t>(sz);
}




/////////////////////////////////////////////////
// CStrW
/////////////////////////////////////////////////

//#################################################################################################
CStrW::CStrW(void)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CStrW::CStrW(const CStrW &src)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CStrW::CStrW(CStrW &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (PWSTR)g_szNullW;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CStrW::CStrW(const CStr8 &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStrW::CStrW(const CStr16 &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStrW::CStrW(const std::wstring &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStrW::CStrW(const std::string &str)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStrW::CStrW(PCWSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CStrW::CStrW(PCSTR sz)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CStrW::CStrW(const wchar_t ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CStrW::CStrW(const char ch)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CStrW::CStrW(const CStrW &str, const size_t nStart, const size_t nLen)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, nStart, nLen);
}
//#################################################################################################
CStrW::CStrW(PCWSTR sz, const size_t nStart, const size_t nLen)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, nStart, nLen);
}

//#################################################################################################
CStrW::CStrW(const EPrintfType eIgnored, PCWSTR szFormat, ...)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	UNUSED(eIgnored);

	va_list vaArgs;
	va_start(vaArgs, szFormat);
	PrintfV(szFormat, vaArgs);
	va_end(vaArgs);
}

//#################################################################################################
CStrW::CStrW(const ELoadType eIgnored, const int nId)
	: m_sz((PWSTR)g_szNullW),
	m_nBufLen(0),
	m_nStrLen(0)
{
	UNUSED(eIgnored);

	LoadString(nId);
}

//#################################################################################################
CStrW::~CStrW(void)
{
	Dealloc();
}

//#################################################################################################
CStrW &CStrW::Attach(PWSTR sz, const size_t nBufLen)
{
	Dealloc();

	if(sz)
	{
		m_sz = sz;
		m_nBufLen = nBufLen;
		m_nStrLen = StringGetLength(sz);
	}

	return *this;
}

//#################################################################################################
PWSTR CStrW::Detach(void) noexcept
{	// Release the buffer, it is up to the caller to dealloc the memory
	PWSTR sz = nullptr;

	if(m_nBufLen)
	{
		sz = m_sz;
		m_sz = (PWSTR)g_szNullW;
		m_nBufLen = m_nStrLen = 0;
	}

	return sz;
}

//#################################################################################################
void CStrW::SecureErase(void)
{	// Overwrite the memory of the string with zeros, great for sensitive data like passwords which otherwise could be left in memory when the string is freed
	for(size_t n = 0; n < m_nBufLen; ++n)
		m_sz[n] = g_chNullW;
	m_nStrLen = 0;
}

//#################################################################################################
size_t CStrW::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CStrW::GetCount(const bool bIncludeNullTerm) const noexcept
{	// Count of characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CStrW::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen) * sizeof(wchar_t);
}

//#################################################################################################
bool CStrW::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
void CStrW::Empty(const bool bFree)
{
	if(bFree)
		Dealloc();
	else
	{
		if(m_nBufLen)
			*m_sz = g_chNullW;
		else
			m_sz = (PWSTR)g_szNullW;
		m_nStrLen = 0;
	}
}

//#################################################################################################
wchar_t CStrW::GetFirst(void) const
{
	return (m_nStrLen) ? m_sz[0] : g_chNullW;
}

//#################################################################################################
wchar_t CStrW::GetLast(void) const
{
	return (m_nStrLen) ? m_sz[m_nStrLen - 1] : g_chNullW;
}

//#################################################################################################
wchar_t CStrW::GetAt(const size_t nIndex) const
{
	return (nIndex < m_nStrLen) ? m_sz[nIndex] : g_chNullW;
}

//#################################################################################################
void CStrW::SetAt(const size_t nIndex, const wchar_t ch)
{
	if(nIndex < m_nStrLen)
	{
		m_sz[nIndex] = ch;
		if(ch == g_chNullW)
			m_nStrLen = nIndex;
	}
}

//#################################################################################################
CStrW::operator PCWSTR(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
CStr8 CStrW::AsUtf8(void) const
{
	return CStr8(*this);
}

//#################################################################################################
CStrW CStrW::AsWide(void) const
{
	return *this;
}

//#################################################################################################
CStrW CStrW::AsNative(void) const
{
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const CStrW &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(CStrW &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (PWSTR)g_szNullW;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const CStr16 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const std::wstring &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const std::string &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(PCSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const wchar_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator=(const char ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
void CStrW::Assign(const CStrW &str)
{
	if(this != &str)
	{
		if(str.m_nStrLen)
		{	// Alloc and copy
			if(Alloc(str.m_nStrLen + 1))
			{
				std::wmemcpy(m_sz, str.m_sz, str.m_nStrLen);
				m_nStrLen = str.m_nStrLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
			else
				Empty();
		}
		else
			Empty();
	}
}

//#################################################################################################
void CStrW::Assign(const CStr8 &str)
{
	if(str.m_nStrLen)
	{	// Alloc and copy
		if(Alloc(Utf8ToWide(str.m_sz, str.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const CStr16 &str)
{
	if(str.m_nStrLen)
	{	// Alloc and copy
		if(Alloc(Utf16ToWide(str.m_sz, str.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf16ToWide(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const std::wstring &str)
{
	if(str.length())
	{	// Alloc and copy
		if(Alloc(str.length() + 1))
		{
			std::wmemcpy(m_sz, str.c_str(), str.length());
			m_nStrLen = str.length();
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const std::string &str)
{
	if(str.length())
	{	// Alloc and copy
		if(Alloc(Utf8ToWide(str.c_str(), str.length(), nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(str.c_str(), str.length(), m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(PCWSTR sz)
{
	if(sz && *sz != g_chNullW)
	{	// Alloc and copy
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(nStrLen + 1))
		{
			std::wmemcpy(m_sz, sz, nStrLen);
			m_nStrLen = nStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(PCSTR sz)
{
	if(sz && *sz != g_chNull8)
	{	// Alloc and copy
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(Utf8ToWide(sz, nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(sz, nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const wchar_t ch)
{
	if(ch != g_chNullW)
	{	// Alloc and set
		if(Alloc(2))
		{
			*m_sz = ch;
			m_nStrLen = 1;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const char ch)
{
	if(ch != g_chNull8)
	{	// Alloc and set
		if(Alloc(Utf8ToWide(&ch, 1, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToWide(&ch, 1, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const CStrW &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{	// Alloc and copy
		size_t nNewLen;
		if(nLen == end)
			nNewLen = str.m_nStrLen - nStart;
		else
			nNewLen = MIN(nLen, str.m_nStrLen - nStart);

		if(Alloc(nNewLen + 1))
		{
			std::wmemcpy(m_sz, str.m_sz + nStart, nNewLen);
			m_nStrLen = nNewLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(const CStr8 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{	// Alloc and copy
		size_t nNewLen;
		if(nLen == end)
			nNewLen = Utf8ToWide(str.m_sz + nStart, str.m_nStrLen - nStart, nullptr, 0);
		else
		{
			size_t nNewLenW = Utf8ToWide(str.m_sz + nStart, MIN(nLen, str.m_nStrLen - nStart), nullptr, 0);
			nNewLen = MIN(nLen, nNewLenW);
		}

		if(Alloc(nNewLen + 1))
		{
			Utf8ToWide(str.m_sz + nStart, str.m_nStrLen - nStart, m_sz, m_nBufLen);
			m_nStrLen = nNewLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(PCWSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNullW && nLen)
	{	// Alloc and copy
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nNewLen;
			if(nLen == end)
				nNewLen = nStrLen - nStart;
			else
				nNewLen = MIN(nLen, nStrLen - nStart);

			if(Alloc(nNewLen + 1))
			{
				std::wmemcpy(m_sz, sz + nStart, nNewLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
			else
				Empty();
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStrW::Assign(PCSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull8 && nLen)
	{	// Alloc and copy
		size_t nStrLen = (nLen == CStr8::end) ? StringGetLength(sz) : CStr8::GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nNewLen;
			if(nLen == end)
				nNewLen = Utf8ToWide(sz + nStart, nStrLen - nStart, nullptr, 0);
			else
			{
				size_t nNewLen8 = Utf8ToWide(sz + nStart, MIN(nLen, nStrLen - nStart), nullptr, 0);
				nNewLen = MIN(nLen, nNewLen8);
			}

			if(Alloc(nNewLen + 1))
			{
				Utf8ToWide(sz + nStart, nStrLen - nStart, m_sz, m_nBufLen);
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
			else
				Empty();
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
CStrW &CStrW::operator+=(const CStrW &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator+=(const CStr8 &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator+=(PCWSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator+=(PCSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator+=(const wchar_t ch)
{
	Append(ch);
	return *this;
}

//#################################################################################################
CStrW &CStrW::operator+=(const char ch)
{
	Append(ch);
	return *this;
}

//#################################################################################################
void CStrW::Prepend(const CStrW &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + str.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + str.m_nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[str.m_nStrLen] = g_chNullW;
			std::wmemcpy(m_sz, str.m_sz, str.m_nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(const CStr8 &str)
{
	if(str.m_nStrLen)
	{
		size_t nStrLen = Utf8ToWide(str.m_sz, str.m_nStrLen, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen] = g_chNullW;
			Utf8ToWide(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(PCWSTR sz)
{
	if(sz && *sz != g_chNullW)
	{
		size_t nStrLen = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen] = g_chNullW;
			std::wmemcpy(m_sz, sz, nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(PCSTR sz)
{
	if(sz && *sz != g_chNull8)
	{
		size_t nStrLen8 = StringGetLength(sz);
		size_t nStrLenW = Utf8ToWide(sz, nStrLen8, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLenW;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + nStrLenW, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLenW] = g_chNullW;
			Utf8ToWide(sz, nStrLen8, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(const wchar_t ch)
{
	if(ch != g_chNullW)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + 1, m_sz, m_nStrLen + 1);
			else
				m_sz[1] = g_chNullW;
			m_sz[0] = ch;
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(const char ch)
{
	if(ch != g_chNull8)
	{
		size_t nStrLen = Utf8ToWide(&ch, 1, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + nStrLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nStrLen] = g_chNullW;
			Utf8ToWide(&ch, 1, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(const CStrW &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nPrependLen;
		if(nLen == end)
			nPrependLen = str.m_nStrLen - nStart;
		else
			nPrependLen = MIN(nLen, str.m_nStrLen - nStart);

		size_t nNewStrLen = m_nStrLen + nPrependLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + nPrependLen, m_sz, m_nStrLen + 1);
			else
				m_sz[nPrependLen] = g_chNullW;
			std::wmemcpy(m_sz, str.m_sz + nStart, nPrependLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(const CStr8 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nPrependLen8;
		size_t nPrependLenW;

		if(nLen == end)
		{
			nPrependLen8 = str.m_nStrLen - nStart;
			nPrependLenW = Utf8ToWide(str.m_sz + nStart, nPrependLen8, nullptr, 0);
		}
		else
		{
			nPrependLen8 = MIN(nLen, str.m_nStrLen - nStart);
			size_t nConvertLenW = Utf8ToWide(str.m_sz + nStart, nPrependLen8, nullptr, 0);
			nPrependLenW = MIN(nLen, nConvertLenW);
		}

		size_t nNewStrLen = m_nStrLen + nPrependLenW;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::wmemmove(m_sz + nPrependLenW, m_sz, m_nStrLen + 1);
			else
				m_sz[nPrependLenW] = g_chNullW;
			Utf8ToWide(str.m_sz + nStart, nPrependLen8, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStrW::Prepend(PCWSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNullW && nLen)
	{
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nPrependLen;
			if(nLen == end)
				nPrependLen = nStrLen - nStart;
			else
				nPrependLen = MIN(nLen, nStrLen - nStart);

			size_t nNewStrLen = m_nStrLen + nPrependLen;
			if(Alloc(nNewStrLen + 1))
			{
				if(m_nStrLen)
					std::wmemmove(m_sz + nPrependLen, m_sz, m_nStrLen + 1);
				else
					m_sz[nPrependLen] = g_chNullW;
				std::wmemcpy(m_sz, sz + nStart, nPrependLen);
				m_nStrLen = nNewStrLen;
			}
		}
	}
}

//#################################################################################################
void CStrW::Prepend(PCSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull8 && nLen)
	{
		size_t nStrLen8 = (nLen == CStr8::end) ? StringGetLength(sz) : CStr8::GetLength(sz, nStart + nLen);
		if(nStart < nStrLen8)
		{
			size_t nPrependLen8;
			size_t nPrependLenW;

			if(nLen == end)
			{
				nPrependLen8 = nStrLen8 - nStart;
				nPrependLenW = Utf8ToWide(sz + nStart, nPrependLen8, nullptr, 0);
			}
			else
			{
				nPrependLen8 = MIN(nLen, nStrLen8 - nStart);
				size_t nConvertLenW = Utf8ToWide(sz + nStart, nPrependLen8, nullptr, 0);
				nPrependLenW = MIN(nLen, nConvertLenW);
			}

			size_t nNewStrLen = m_nStrLen + nPrependLenW;
			if(Alloc(nNewStrLen + 1))
			{
				if(m_nStrLen)
					std::wmemmove(m_sz + nPrependLenW, m_sz, m_nStrLen + 1);
				else
					m_sz[nPrependLenW] = g_chNullW;
				Utf8ToWide(sz + nStart, nPrependLen8, m_sz, m_nBufLen);
				m_nStrLen = nNewStrLen;
			}
		}
	}
}

//#################################################################################################
void CStrW::Append(const CStrW &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + str.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::wmemcpy(m_sz + m_nStrLen, str.m_sz, str.m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(const CStr8 &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + Utf8ToWide(str.m_sz, str.m_nStrLen, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			Utf8ToWide(str.m_sz, str.m_nStrLen, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(PCWSTR sz)
{
	if(sz && *sz != g_chNullW)
	{
		size_t nStrLen = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::wmemcpy(m_sz + m_nStrLen, sz, nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(PCSTR sz)
{
	if(sz && *sz != g_chNull8)
	{
		size_t nStrLen8 = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + Utf8ToWide(sz, nStrLen8, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			Utf8ToWide(sz, nStrLen8, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(const wchar_t ch)
{
	if(ch != g_chNullW)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			m_sz[m_nStrLen] = ch;
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(const char ch)
{
	if(ch != g_chNull8)
	{
		size_t nNewStrLen = m_nStrLen + Utf8ToWide(&ch, 1, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			Utf8ToWide(&ch, 1, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(const CStrW &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nAppendLen;
		if(nLen == end)
			nAppendLen = str.m_nStrLen - nStart;
		else
			nAppendLen = MIN(nLen, str.m_nStrLen - nStart);

		size_t nNewStrLen = m_nStrLen + nAppendLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::wmemcpy(m_sz + m_nStrLen, str.m_sz + nStart, nAppendLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(const CStr8 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nAppendLen8;
		size_t nAppendLenW;

		if(nLen == end)
		{
			nAppendLen8 = str.m_nStrLen - nStart;
			nAppendLenW = Utf8ToWide(str.m_sz + nStart, nAppendLen8, nullptr, 0);
		}
		else
		{
			nAppendLen8 = MIN(nLen, str.m_nStrLen - nStart);
			size_t nConvertLenW = Utf8ToWide(str.m_sz + nStart, nAppendLen8, nullptr, 0);
			nAppendLenW = MIN(nLen, nConvertLenW);
		}

		size_t nNewStrLen = m_nStrLen + nAppendLenW;
		if(Alloc(nNewStrLen + 1))
		{
			Utf8ToWide(str.m_sz + nStart, nAppendLen8, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNullW;
		}
	}
}

//#################################################################################################
void CStrW::Append(PCWSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNullW && nLen)
	{
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nAppendLen;
			if(nLen == end)
				nAppendLen = nStrLen - nStart;
			else
				nAppendLen = MIN(nLen, nStrLen - nStart);

			size_t nNewStrLen = m_nStrLen + nAppendLen;
			if(Alloc(nNewStrLen + 1))
			{
				std::wmemcpy(m_sz + m_nStrLen, sz + nStart, nAppendLen);
				m_nStrLen = nNewStrLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
		}
	}
}

//#################################################################################################
void CStrW::Append(PCSTR sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull8 && nLen)
	{
		size_t nStrLen8 = (nLen == CStr8::end) ? StringGetLength(sz) : CStr8::GetLength(sz, nStart + nLen);
		if(nStart < nStrLen8)
		{
			size_t nAppendLen8;
			size_t nAppendLenW;

			if(nLen == end)
			{
				nAppendLen8 = nStrLen8 - nStart;
				nAppendLenW = Utf8ToWide(sz + nStart, nAppendLen8, nullptr, 0);
			}
			else
			{
				nAppendLen8 = MIN(nLen, nStrLen8 - nStart);
				size_t nConvertLenW = Utf8ToWide(sz + nStart, nAppendLen8, nullptr, 0);
				nAppendLenW = MIN(nLen, nConvertLenW);
			}

			size_t nNewStrLen = m_nStrLen + nAppendLenW;
			if(Alloc(nNewStrLen + 1))
			{
				Utf8ToWide(sz + nStart, nAppendLen8, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
				m_nStrLen = nNewStrLen;
				m_sz[m_nStrLen] = g_chNullW;
			}
		}
	}
}

//#################################################################################################
CStrW operator+(const CStrW &str1, const CStrW &str2)
{
	CStrW strRtn(str1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStrW &str1, const CStr8 &str2)
{
	CStrW strRtn(str1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStr8 &str1, const CStrW &str2)
{
	CStrW strRtn(str1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStr8 &str1, const CStr8 &str2)
{
	CStrW strRtn(str1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStrW &str1, PCWSTR sz2)
{
	CStrW strRtn(str1);
	strRtn.Append(sz2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStr8 &str1, PCWSTR sz2)
{
	CStrW strRtn(str1);
	strRtn.Append(sz2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStrW &str1, PCSTR sz2)
{
	CStrW strRtn(str1);
	strRtn.Append(sz2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStr8 &str1, PCSTR sz2)
{
	CStrW strRtn(str1);
	strRtn.Append(sz2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStrW &str1, const wchar_t ch2)
{
	CStrW strRtn(str1);
	strRtn.Append(ch2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStr8 &str1, const wchar_t ch2)
{
	CStrW strRtn(str1);
	strRtn.Append(ch2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStrW &str1, const char ch2)
{
	CStrW strRtn(str1);
	strRtn.Append(ch2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const CStr8 &str1, const char ch2)
{
	CStrW strRtn(str1);
	strRtn.Append(ch2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(PCWSTR sz1, const CStrW &str2)
{
	CStrW strRtn(sz1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(PCWSTR sz1, const CStr8 &str2)
{
	CStrW strRtn(sz1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(PCSTR sz1, const CStrW &str2)
{
	CStrW strRtn(sz1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(PCSTR sz1, const CStr8 &str2)
{
	CStrW strRtn(sz1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const wchar_t ch1, const CStrW &str2)
{
	CStrW strRtn(ch1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const wchar_t ch1, const CStr8 &str2)
{
	CStrW strRtn(ch1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const char ch1, const CStrW &str2)
{
	CStrW strRtn(ch1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
CStrW operator+(const char ch1, const CStr8 &str2)
{
	CStrW strRtn(ch1);
	strRtn.Append(str2);
	return strRtn;
}

//#################################################################################################
std::wostream &operator<<(std::wostream &stream, const CStrW &str)
{
	stream << str.m_sz;
	return stream;
}

//#################################################################################################
bool CStrW::operator==(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) == 0);
}

//#################################################################################################
bool CStrW::operator==(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) == 0);
}

//#################################################################################################
bool CStrW::operator==(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) == 0);
}

//#################################################################################################
bool CStrW::operator!=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) != 0);
}

//#################################################################################################
bool CStrW::operator!=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) != 0);
}

//#################################################################################################
bool CStrW::operator!=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) != 0);
}

//#################################################################################################
bool CStrW::operator<(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) < 0);
}

//#################################################################################################
bool CStrW::operator<(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) < 0);
}

//#################################################################################################
bool CStrW::operator<(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) < 0);
}

//#################################################################################################
bool CStrW::operator<=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) <= 0);
}

//#################################################################################################
bool CStrW::operator<=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) <= 0);
}

//#################################################################################################
bool CStrW::operator<=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) <= 0);
}

//#################################################################################################
bool CStrW::operator>(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) > 0);
}

//#################################################################################################
bool CStrW::operator>(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) > 0);
}

//#################################################################################################
bool CStrW::operator>(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) > 0);
}

//#################################################################################################
bool CStrW::operator>=(const CStrW &str) const
{
	return (StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) >= 0);
}

//#################################################################################################
bool CStrW::operator>=(PCWSTR sz) const
{
	return (StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz)) >= 0);
}

//#################################################################################################
bool CStrW::operator>=(const wchar_t ch) const
{
	return (StringCompareW(m_sz, m_nStrLen, &ch, 1) >= 0);
}

//#################################################################################################
int CStrW::Compare(const CStrW &str, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CStrW::Compare(PCWSTR sz, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CStrW::Compare(const wchar_t ch, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
int CStrW::CompareAlphaNum(const CStrW &str, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	static PCWSTR szDigitsW = L"0123456789";

	size_t nPos1 = 0;
	size_t nPos2 = 0;
	CStrW strChunk1;
	CStrW strChunk2;

	while(nCompareRtn == 0)
	{
		bool bNum1 = false;
		bool bNum2 = false;

		wchar_t ch1 = GetAt(nPos1);
		wchar_t ch2 = str.GetAt(nPos2);
		if(ch1 == g_chNullW && ch2 == g_chNullW)
			break;

		if(std::wmemchr(szDigitsW, ch1, 10))
		{
			strChunk1 = SpanIncluding(szDigitsW, nPos1);
			bNum1 = true;
		}
		else
			strChunk1 = SpanExcluding(szDigitsW, nPos1);
		nPos1 += strChunk1.GetLength();

		if(std::wmemchr(szDigitsW, ch2, 10))
		{
			strChunk2 = str.SpanIncluding(szDigitsW, nPos2);
			bNum2 = true;
		}
		else
			strChunk2 = str.SpanExcluding(szDigitsW, nPos2);
		nPos2 += strChunk2.GetLength();

		if(bNum1 && bNum2)
		{
			uint64_t n1 = StringToIntegerW<uint64_t>(strChunk1);
			uint64_t n2 = StringToIntegerW<uint64_t>(strChunk2);

			if(n1 < n2)
				nCompareRtn = -1;
			else if(n1 > n2)
				nCompareRtn = 1;
		}
		else if(bNum1)
			nCompareRtn = -1;
		else if(bNum2)
			nCompareRtn = 1;
		else
			nCompareRtn = StringCompareW(strChunk1.m_sz, strChunk1.m_nStrLen, strChunk2.m_sz, strChunk2.m_nStrLen, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
int CStrW::CompareMid(const CStrW &str, const size_t nStart, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
	nCompareRtn = StringCompareW(m_sz + nOffset, MIN(m_nStrLen - nOffset, nCount), str.m_sz, MIN(str.m_nStrLen, nCount), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStrW::CompareMid(PCWSTR sz, const size_t nStart, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = GetLength(sz, nCount);
	size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
	nCompareRtn = StringCompareW(m_sz + nOffset, MIN(m_nStrLen - nOffset, nCount), sz, nStrLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStrW::CompareMid(const wchar_t ch, const size_t nStart, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNullW)
	{
		size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
		nCompareRtn = StringCompareW(m_sz + nOffset, MIN(m_nStrLen - nOffset, 1), &ch, 1, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
int CStrW::CompareLeft(const CStrW &str, const size_t nCount, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, MIN(m_nStrLen, nCount), str.m_sz, MIN(str.m_nStrLen, nCount), bCaseInsensitive);
}

//#################################################################################################
int CStrW::CompareLeft(PCWSTR sz, const size_t nCount, const bool bCaseInsensitive) const
{
	return StringCompareW(m_sz, MIN(m_nStrLen, nCount), sz, GetLength(sz, nCount), bCaseInsensitive);
}

//#################################################################################################
int CStrW::CompareLeft(const wchar_t ch, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNullW)
		nCompareRtn = StringCompareW(m_sz, MIN(m_nStrLen, 1), &ch, 1, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStrW::CompareRight(const CStrW &str, const size_t nCount, const bool bCaseInsensitive) const
{
	size_t nStrLen1 = MIN(m_nStrLen, nCount);
	size_t nStrLen2 = MIN(str.m_nStrLen, nCount);

	return StringCompareW(m_sz + (m_nStrLen - nStrLen1), nStrLen1, str.m_sz + (str.m_nStrLen - nStrLen2), nStrLen2, bCaseInsensitive);
}

//#################################################################################################
int CStrW::CompareRight(PCWSTR sz, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(sz);
	size_t nStrLen1 = MIN(m_nStrLen, nCount);
	size_t nStrLen2 = MIN(nStrLen, nCount);
	nCompareRtn = StringCompareW(m_sz + (m_nStrLen - nStrLen1), nStrLen1, sz + (nStrLen - nStrLen2), nStrLen2, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStrW::CompareRight(const wchar_t ch, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNullW)
	{
		size_t nStrLen1 = MIN(m_nStrLen, 1);

		nCompareRtn = StringCompareW(m_sz + (m_nStrLen - nStrLen1), nStrLen1, &ch, 1, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
size_t CStrW::Count(const wchar_t chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(m_nStrLen && nStart < m_nStrLen)
	{
		if(bCaseInsensitive)
		{
			wchar_t chLower = CharToLower(chFind);
			for(size_t n = nStart; n < m_nStrLen; ++n)
			{
				if(CharToLower(m_sz[n]) == chLower)
					++nCount;
			}
		}
		else
		{
			for(size_t n = nStart; n < m_nStrLen; ++n)
			{
				if(m_sz[n] == chFind)
					++nCount;
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::CountCompare(const CStrW &str, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(nStart < m_nStrLen)
	{
		PCWSTR szThis = m_sz + nStart;
		PCWSTR szThat = str.m_sz;

		if(bCaseInsensitive)
		{
			while(*szThis != g_chNullW && *szThat != g_chNullW && CharToLower(*szThis++) == CharToLower(*szThat++))
				++nCount;
		}
		else
		{
			while(*szThis != g_chNullW && *szThat != g_chNullW && *szThis++ == *szThat++)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::CountCompare(PCWSTR sz, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(nStart < m_nStrLen && sz)
	{
		PCWSTR szThis = m_sz + nStart;
		PCWSTR szThat = sz;

		if(bCaseInsensitive)
		{
			while(*szThis != g_chNullW && *szThat != g_chNullW && CharToLower(*szThis++) == CharToLower(*szThat++))
				++nCount;
		}
		else
		{
			while(*szThis != g_chNullW && *szThat != g_chNullW && *szThis++ == *szThat++)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::ReverseCountCompare(const CStrW &str, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(m_nStrLen && str.m_nStrLen)
	{
		PCWSTR szThis = (nStart >= m_nStrLen) ? (m_sz + m_nStrLen - 1) : (m_sz + nStart);
		PCWSTR szThat = str.m_sz + str.m_nStrLen - 1;

		if(bCaseInsensitive)
		{
			while(szThis != m_sz && szThat != str.m_sz && CharToLower(*szThis--) == CharToLower(*szThat--))
				++nCount;
		}
		else
		{
			while(szThis != m_sz && szThat != str.m_sz && *szThis-- == *szThat--)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::ReverseCountCompare(PCWSTR sz, const size_t nStart, const bool bCaseInsensitive) const
{
	size_t nCount = 0;

	if(m_nStrLen)
	{
		PCWSTR szThis = (nStart >= m_nStrLen) ? (m_sz + m_nStrLen - 1) : (m_sz + nStart);
		PCWSTR szThat = sz + StringGetLength(sz) - 1;

		if(bCaseInsensitive)
		{
			while(szThis != m_sz && szThat != sz && CharToLower(*szThis--) == CharToLower(*szThat--))
				++nCount;
		}
		else
		{
			while(szThis != m_sz && szThat != sz && *szThis-- == *szThat--)
				++nCount;
		}
	}

	return nCount;
}

//#################################################################################################
bool CStrW::Contains(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return false;
	else if(strFind.m_nStrLen == 0)
		return true;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);

	return (nPos != not_found);
}

//#################################################################################################
bool CStrW::Contains(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return false;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return true;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);

	return (nPos != not_found);
}

//#################################################################################################
bool CStrW::Contains(const wchar_t chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return false;

	PCWSTR sz;
	if(bCaseInsensitive)
		sz = wmemchri(m_sz + nStart, chFind, m_nStrLen - nStart);
	else
		sz = std::wmemchr(m_sz + nStart, chFind, m_nStrLen - nStart);

	return (sz != nullptr);
}

//#################################################################################################
bool CStrW::ContainsOneOf(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return false;
	else if(strFind.m_nStrLen == 0)
		return true;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(wmemchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return true;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::wmemchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return true;
		}
	}

	return false;
}

//#################################################################################################
bool CStrW::ContainsOneOf(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return false;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return true;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(wmemchri(szFind, m_sz[nIndex], nFindLen))
				return true;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::wmemchr(szFind, m_sz[nIndex], nFindLen))
				return true;
		}
	}

	return false;
}

//#################################################################################################
size_t CStrW::Find(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, strFind.m_sz, strFind.m_nStrLen);

	return (nPos == not_found) ? not_found : nPos + nStart;
}

//#################################################################################################
size_t CStrW::Find(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	size_t nPos;
	if(bCaseInsensitive)
		nPos = FindStrIndexI(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);
	else
		nPos = FindStrIndex(m_sz + nStart, m_nStrLen - nStart, szFind, nFindLen);

	return (nPos == not_found) ? not_found : nPos + nStart;
}

//#################################################################################################
size_t CStrW::Find(const wchar_t chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(nStart >= m_nStrLen)
		return not_found;

	PCWSTR sz;
	if(bCaseInsensitive)
		sz = wmemchri(m_sz + nStart, chFind, m_nStrLen - nStart);
	else
		sz = std::wmemchr(m_sz + nStart, chFind, m_nStrLen - nStart);

	return (sz == nullptr) ? not_found : (size_t)(sz - m_sz);
}

//#################################################################################################
size_t CStrW::ReverseFind(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen && nStart)
	{
		if(strFind.m_nStrLen == 0)
			return nStart;	// Null string always matches

		size_t nIndex;
		if(nStart >= m_nStrLen)
		{
			if(strFind.m_nStrLen > m_nStrLen)
				return not_found;
			nIndex = m_nStrLen - strFind.m_nStrLen;
		}
		else
		{
			if(strFind.m_nStrLen > nStart)
				return not_found;
			nIndex = nStart - strFind.m_nStrLen;
		}

		if(bCaseInsensitive)
		{
			do{
				if(wmemcmpi(m_sz + nIndex, strFind, strFind.m_nStrLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
		else
		{
			do{
				if(std::wmemcmp(m_sz + nIndex, strFind, strFind.m_nStrLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::ReverseFind(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen && nStart)
	{
		size_t nFindLen = StringGetLength(szFind);
		if(nFindLen == 0)
			return nStart;	// Null string always matches

		size_t nIndex;
		if(nStart >= m_nStrLen)
		{
			if(nFindLen > m_nStrLen)
				return not_found;
			nIndex = m_nStrLen - nFindLen;
		}
		else
		{
			if(nFindLen > nStart)
				return not_found;
			nIndex = nStart - nFindLen;
		}

		if(bCaseInsensitive)
		{
			do{
				if(wmemcmpi(m_sz + nIndex, szFind, nFindLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
		else
		{
			do{
				if(std::wmemcmp(m_sz + nIndex, szFind, nFindLen) == 0)
					return nIndex;
			}while(nIndex-- != 0);
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::ReverseFind(const wchar_t chFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen && nStart)
	{
		size_t nIndex;
		if(nStart >= m_nStrLen)
			nIndex = m_nStrLen - 1;
		else
			nIndex = nStart - 1;

		if(bCaseInsensitive)
		{
			wchar_t chLower = CharToLower(chFind);
			do{
				if(CharToLower(m_sz[nIndex]) == chLower)
					return nIndex;
			}while(nIndex-- != 0);
		}
		else
		{
			do{
				if(m_sz[nIndex] == chFind)
					return nIndex;
			}while(nIndex-- != 0);
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindFirstOf(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(wmemchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::wmemchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindFirstOf(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(wmemchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(std::wmemchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindFirstNotOf(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!wmemchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!std::wmemchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindFirstNotOf(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart >= m_nStrLen)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	if(bCaseInsensitive)
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!wmemchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}
	else
	{
		for(size_t nIndex = nStart; nIndex < m_nStrLen; ++nIndex)
		{
			if(!std::wmemchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindLastOf(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(wmemchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(std::wmemchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindLastOf(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(wmemchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(std::wmemchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindLastNotOf(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;
	else if(strFind.m_nStrLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(!wmemchri(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(!std::wmemchr(strFind.m_sz, m_sz[nIndex], strFind.m_nStrLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindLastNotOf(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	if(m_nStrLen == 0 || nStart == 0)
		return not_found;

	size_t nFindLen = StringGetLength(szFind);
	if(nFindLen == 0)
		return nStart;	// Null string always matches

	size_t nIndex;
	if(nStart >= m_nStrLen)
		nIndex = m_nStrLen - 1;
	else
		nIndex = nStart - 1;

	if(bCaseInsensitive)
	{
		do{
			if(!wmemchri(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}
	else
	{
		do{
			if(!std::wmemchr(szFind, m_sz[nIndex], nFindLen))
				return nIndex;
		}while(nIndex-- != 0);
	}

	return not_found;
}

//#################################################################################################
CStrW CStrW::SpanIncluding(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstNotOf(strFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
CStrW CStrW::SpanIncluding(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstNotOf(szFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
CStrW CStrW::SpanExcluding(const CStrW &strFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstOf(strFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
CStrW CStrW::SpanExcluding(PCWSTR szFind, const size_t nStart, const bool bCaseInsensitive) const
{
	return GetMid(nStart, FindFirstOf(szFind, nStart, bCaseInsensitive) - nStart);
}

//#################################################################################################
size_t CStrW::Insert(const size_t nIndex, const CStrW &strInsert)
{
	if(nIndex >= m_nStrLen)
		Append(strInsert);
	else if(strInsert.m_nStrLen != 0)
	{
		size_t nNewStrLen = m_nStrLen + strInsert.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::wmemmove(m_sz + nIndex + strInsert.m_nStrLen, m_sz + nIndex, m_nStrLen + 1 - nIndex);
			std::wmemcpy(m_sz + nIndex, strInsert.m_sz, strInsert.m_nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStrW::Insert(const size_t nIndex, PCWSTR szInsert)
{
	if(nIndex >= m_nStrLen)
		Append(szInsert);
	else if(szInsert != nullptr && *szInsert != g_chNullW)
	{
		size_t nStrLen = StringGetLength(szInsert);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::wmemmove(m_sz + nIndex + nStrLen, m_sz + nIndex, m_nStrLen + 1 - nIndex);
			std::wmemcpy(m_sz + nIndex, szInsert, nStrLen);
			m_nStrLen = nNewStrLen;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStrW::Insert(const size_t nIndex, const wchar_t chInsert)
{
	if(nIndex >= m_nStrLen)
		Append(chInsert);
	else if(chInsert != g_chNullW)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			std::wmemmove(m_sz + nIndex + 1, m_sz + nIndex, m_nStrLen + 1 - nIndex);
			m_sz[nIndex] = chInsert;
			m_nStrLen = nNewStrLen;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStrW::Delete(const size_t nIndex, const size_t nCount)
{
	if(nIndex < m_nStrLen)
	{
		if(nIndex == 0 && (nCount == end || nCount >= m_nStrLen))
			Empty();
		else if(nCount == end || nIndex + nCount >= m_nStrLen)
		{	// If nCount is -1 or nIndex + nCount is too long, delete the last part of the string
			m_nStrLen = nIndex;
			m_sz[m_nStrLen] = g_chNullW;
		}
		else if(nCount)
		{	// Remove chars out of the middle
			std::wmemmove(m_sz + nIndex, m_sz + nIndex + nCount, m_nStrLen + 1 - nIndex - nCount);
			m_nStrLen -= nCount;
		}
	}

	return m_nStrLen;
}

//#################################################################################################
size_t CStrW::Remove(const CStrW &strRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen && strRemove.m_nStrLen)
	{
		if(bCaseInsensitive)
		{
			PWSTR szScan = FindStrPtrI(m_sz, m_nStrLen, strRemove.m_sz, strRemove.m_nStrLen);
			while(szScan)
			{
				std::wmemmove(szScan, szScan + strRemove.m_nStrLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - strRemove.m_nStrLen);
				m_nStrLen -= strRemove.m_nStrLen;
				++nCount;

				szScan = FindStrPtrI(szScan, m_nStrLen - (size_t)(szScan - m_sz), strRemove.m_sz, strRemove.m_nStrLen);
			}
		}
		else
		{
			PWSTR szScan = FindStrPtr(m_sz, m_nStrLen, strRemove.m_sz, strRemove.m_nStrLen);
			while(szScan)
			{
				std::wmemmove(szScan, szScan + strRemove.m_nStrLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - strRemove.m_nStrLen);
				m_nStrLen -= strRemove.m_nStrLen;
				++nCount;

				szScan = FindStrPtr(szScan, m_nStrLen - (size_t)(szScan - m_sz), strRemove.m_sz, strRemove.m_nStrLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::Remove(PCWSTR szRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	size_t nRemoveLen = StringGetLength(szRemove);
	if(m_nStrLen && nRemoveLen)
	{
		if(bCaseInsensitive)
		{
			PWSTR szScan = FindStrPtrI(m_sz, m_nStrLen, szRemove, nRemoveLen);
			while(szScan)
			{
				std::wmemmove(szScan, szScan + nRemoveLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - nRemoveLen);
				m_nStrLen -= nRemoveLen;
				++nCount;

				szScan = FindStrPtrI(szScan, m_nStrLen - (size_t)(szScan - m_sz), szRemove, nRemoveLen);
			}
		}
		else
		{
			PWSTR szScan = FindStrPtr(m_sz, m_nStrLen, szRemove, nRemoveLen);
			while(szScan)
			{
				std::wmemmove(szScan, szScan + nRemoveLen, m_nStrLen + 1 - (size_t)(szScan - m_sz) - nRemoveLen);
				m_nStrLen -= nRemoveLen;
				++nCount;

				szScan = FindStrPtr(szScan, m_nStrLen - (size_t)(szScan - m_sz), szRemove, nRemoveLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::Remove(const wchar_t chRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen && chRemove != g_chNullW)
	{
		PWSTR szWrite = m_sz;
		PWSTR szScan = m_sz;

		if(bCaseInsensitive)
		{
			wchar_t chLower = CharToLower(chRemove);
			while(*szScan != g_chNullW)
			{
				if(CharToLower(*szScan) != chLower)
					*szWrite++ = *szScan;
				else
					++nCount;
				++szScan;
			}
		}
		else
		{
			while(*szScan != g_chNullW)
			{
				if(*szScan != chRemove)
					*szWrite++ = *szScan;
				else
					++nCount;
				++szScan;
			}
		}

		*szWrite = g_chNullW;
		m_nStrLen -= nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::RemoveAllOf(const CStrW &strRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0 || strRemove.m_nStrLen == 0)
		return nCount;

	PWSTR szWrite = m_sz;
	PWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNullW)
		{
			if(!wmemchri(strRemove.m_sz, *szScan, strRemove.m_nStrLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen -= nCount;
	}
	else
	{
		while(*szScan != g_chNullW)
		{
			if(!std::wmemchr(strRemove.m_sz, *szScan, strRemove.m_nStrLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen -= nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::RemoveAllOf(PCWSTR szRemove, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0 || szRemove == nullptr || *szRemove == g_chNullW)
		return nCount;

	size_t nRemoveLen = StringGetLength(szRemove);
	PWSTR szWrite = m_sz;
	PWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNullW)
		{
			if(!wmemchri(szRemove, *szScan, nRemoveLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen -= nCount;
	}
	else
	{
		while(*szScan != g_chNullW)
		{
			if(!std::wmemchr(szRemove, *szScan, nRemoveLen))
				*szWrite++ = *szScan;
			else
				++nCount;
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen -= nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::KeepAllOf(const CStrW &strKeep, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0)
		return nCount;

	if(strKeep.m_nStrLen == 0)
	{
		Empty();
		return nCount;
	}

	PWSTR szWrite = m_sz;
	PWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNullW)
		{
			if(wmemchri(strKeep.m_sz, *szScan, strKeep.m_nStrLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen = nCount;
	}
	else
	{
		while(*szScan != g_chNullW)
		{
			if(std::wmemchr(strKeep.m_sz, *szScan, strKeep.m_nStrLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen = nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::KeepAllOf(PCWSTR szKeep, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen == 0)
		return nCount;

	if(szKeep == nullptr || *szKeep == g_chNullW)
	{
		Empty();
		return nCount;
	}

	size_t nKeepLen = StringGetLength(szKeep);
	PWSTR szWrite = m_sz;
	PWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNullW)
		{
			if(wmemchri(szKeep, *szScan, nKeepLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen = nCount;
	}
	else
	{
		while(*szScan != g_chNullW)
		{
			if(std::wmemchr(szKeep, *szScan, nKeepLen))
			{
				*szWrite++ = *szScan;
				++nCount;
			}
			++szScan;
		}

		*szWrite = g_chNullW;
		m_nStrLen = nCount;
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::Replace(const CStrW &strOld, const CStrW &strNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!m_nStrLen || !strOld.m_nStrLen)
		return nCount;

	if(strOld.m_nStrLen != strNew.m_nStrLen)
	{	// The length of the string is changing
		if(bCaseInsensitive)
		{
			size_t nNewStrLen = m_nStrLen;
			PWSTR szScan = FindStrPtrI(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				nNewStrLen -= strOld.m_nStrLen;
				nNewStrLen += strNew.m_nStrLen;
				szScan = FindStrPtrI(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz), strOld.m_sz, strOld.m_nStrLen);
			}

			auto szTemp = std::make_unique<wchar_t[]>(m_nStrLen);
			std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PWSTR szDest = m_sz;
				PCWSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtrI(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), strOld.m_sz, strOld.m_nStrLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::wmemcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::wmemcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(strNew.m_nStrLen)
						{
							std::wmemcpy(szDest, strNew.m_sz, strNew.m_nStrLen);
							m_nStrLen += strNew.m_nStrLen;
							szDest += strNew.m_nStrLen;
						}
						++nCount;
						szSrc = szScan + strOld.m_nStrLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNullW;
			}
		}
		else
		{
			size_t nNewStrLen = m_nStrLen;
			PWSTR szScan = FindStrPtr(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				nNewStrLen -= strOld.m_nStrLen;
				nNewStrLen += strNew.m_nStrLen;
				szScan = FindStrPtr(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz), strOld.m_sz, strOld.m_nStrLen);
			}

			auto szTemp = std::make_unique<wchar_t[]>(m_nStrLen);
			std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PWSTR szDest = m_sz;
				PCWSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtr(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), strOld.m_sz, strOld.m_nStrLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::wmemcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::wmemcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(strNew.m_nStrLen)
						{
							std::wmemcpy(szDest, strNew.m_sz, strNew.m_nStrLen);
							m_nStrLen += strNew.m_nStrLen;
							szDest += strNew.m_nStrLen;
						}
						++nCount;
						szSrc = szScan + strOld.m_nStrLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNullW;
			}
		}
	}
	else
	{	// No change in length, perform the replace in the current memory
		if(bCaseInsensitive)
		{
			PWSTR szScan = FindStrPtrI(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				std::wmemcpy(szScan, strNew.m_sz, strNew.m_nStrLen);
				++nCount;
				szScan = FindStrPtrI(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz) - strOld.m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			}
		}
		else
		{
			PWSTR szScan = FindStrPtr(m_sz, m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			while(szScan)
			{
				std::wmemcpy(szScan, strNew.m_sz, strNew.m_nStrLen);
				++nCount;
				szScan = FindStrPtr(szScan + strOld.m_nStrLen, m_nStrLen - (size_t)(szScan - m_sz) - strOld.m_nStrLen, strOld.m_sz, strOld.m_nStrLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::Replace(PCWSTR szOld, PCWSTR szNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(!m_nStrLen)
		return nCount;

	size_t nOldLen = StringGetLength(szOld);
	size_t nNewLen = StringGetLength(szNew);
	if(nOldLen != nNewLen)
	{	// The length of the string is changing
		if(bCaseInsensitive)
		{
			size_t nNewStrLen = m_nStrLen;
			PWSTR szScan = FindStrPtrI(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				nNewStrLen -= nOldLen;
				nNewStrLen += nNewLen;
				szScan = FindStrPtrI(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz), szOld, nOldLen);
			}

			auto szTemp = std::make_unique<wchar_t[]>(m_nStrLen);
			std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PWSTR szDest = m_sz;
				PCWSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtrI(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), szOld, nOldLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::wmemcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::wmemcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(nNewLen)
						{
							std::wmemcpy(szDest, szNew, nNewLen);
							m_nStrLen += nNewLen;
							szDest += nNewLen;
						}
						++nCount;
						szSrc = szScan + nOldLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNullW;
			}
		}
		else
		{
			size_t nNewStrLen = m_nStrLen;
			PWSTR szScan = FindStrPtr(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				nNewStrLen -= nOldLen;
				nNewStrLen += nNewLen;
				szScan = FindStrPtr(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz), szOld, nOldLen);
			}

			auto szTemp = std::make_unique<wchar_t[]>(m_nStrLen);
			std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
			size_t nStrLen = m_nStrLen;

			if(Alloc(nNewStrLen + 1))
			{
				m_nStrLen = 0;
				PWSTR szDest = m_sz;
				PCWSTR szSrc = szTemp.get();
				do{
					szScan = FindStrPtr(szSrc, nStrLen - (size_t)(szSrc - szTemp.get()), szOld, nOldLen);
					if(szScan == nullptr)
					{
						if(nStrLen != (size_t)(szSrc - szTemp.get()))
						{
							std::wmemcpy(szDest, szSrc, nStrLen - (size_t)(szSrc - szTemp.get()));
							m_nStrLen += (nStrLen - (size_t)(szSrc - szTemp.get()));
						}
					}
					else
					{
						if(szScan - szSrc)
						{
							std::wmemcpy(szDest, szSrc, (size_t)(szScan - szSrc));
							m_nStrLen += (size_t)(szScan - szSrc);
							szDest += (szScan - szSrc);
						}
						if(nNewLen)
						{
							std::wmemcpy(szDest, szNew, nNewLen);
							m_nStrLen += nNewLen;
							szDest += nNewLen;
						}
						++nCount;
						szSrc = szScan + nOldLen;
					}
				}while(szScan);
				m_sz[m_nStrLen] = g_chNullW;
			}
		}
	}
	else
	{	// No change in length, perform the replace in the current memory
		if(bCaseInsensitive)
		{
			PWSTR szScan = FindStrPtrI(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				std::wmemcpy(szScan, szNew, nNewLen);
				++nCount;
				szScan = FindStrPtrI(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz) - nOldLen, szOld, nOldLen);
			}
		}
		else
		{
			PWSTR szScan = FindStrPtr(m_sz, m_nStrLen, szOld, nOldLen);
			while(szScan)
			{
				std::wmemcpy(szScan, szNew, nNewLen);
				++nCount;
				szScan = FindStrPtr(szScan + nOldLen, m_nStrLen - (size_t)(szScan - m_sz) - nOldLen, szOld, nOldLen);
			}
		}
	}

	return nCount;
}

//#################################################################################################
size_t CStrW::Replace(const wchar_t chOld, const wchar_t chNew, const bool bCaseInsensitive)
{
	size_t nCount = 0;

	if(m_nStrLen && chOld != chNew && chNew != g_chNullW)
	{
		PWSTR szScan = m_sz;

		if(bCaseInsensitive)
		{
			wchar_t chLower = CharToLower(chOld);
			while(*szScan != g_chNullW)
			{
				if(CharToLower(*szScan) == chLower)
				{
					*szScan = chNew;
					++nCount;
				}
				++szScan;
			}
		}
		else
		{
			while(*szScan != g_chNullW)
			{
				if(*szScan == chOld)
				{
					*szScan = chNew;
					++nCount;
				}
				++szScan;
			}
		}
	}

	return nCount;
}

//#################################################################################################
void CStrW::Swap(CStrW &src) noexcept
{
	SWAP(m_sz, src.m_sz);
	SWAP(m_nBufLen, src.m_nBufLen);
	SWAP(m_nStrLen, src.m_nStrLen);
}

//#################################################################################################
CStrW CStrW::GetMid(const size_t nStart, const size_t nCount) const
{
	CStrW str;

	if(nStart < m_nStrLen)
	{	// Copy nCount chars, unless nCount is zero or nStart + nCount is too much
		size_t nStrLen = (nCount == end || nStart + nCount > m_nStrLen) ? m_nStrLen - nStart : nCount;
		if(str.Alloc(nStrLen + 1))
		{
			std::wmemcpy(str.m_sz, m_sz + nStart, nStrLen);
			str.m_nStrLen = nStrLen;
			str.m_sz[str.m_nStrLen] = g_chNullW;
		}
	}

	return str;
}

//#################################################################################################
CStrW CStrW::GetLeft(const size_t nCount) const
{
	CStrW str;

	if(nCount >= m_nStrLen)
		str.Assign(*this);
	else
	{
		if(str.Alloc(nCount + 1))
		{
			std::wmemcpy(str.m_sz, m_sz, nCount);
			str.m_nStrLen = nCount;
			str.m_sz[str.m_nStrLen] = g_chNullW;
		}
	}

	return str;
}

//#################################################################################################
CStrW CStrW::GetRight(const size_t nCount) const
{
	CStrW str;

	if(nCount >= m_nStrLen)
		str.Assign(*this);
	else
	{
		if(str.Alloc(nCount + 1))
		{
			std::wmemcpy(str.m_sz, m_sz + m_nStrLen - nCount, nCount);
			str.m_nStrLen = nCount;
			str.m_sz[str.m_nStrLen] = g_chNullW;
		}
	}

	return str;
}

//#################################################################################################
CStrW &CStrW::Trim(const CStrW &strTrim, const bool bCaseInsensitive)
{
	TrimLeft(strTrim, bCaseInsensitive);
	TrimRight(strTrim, bCaseInsensitive);
	return *this;
}

//#################################################################################################
CStrW &CStrW::Trim(PCWSTR szTrim, const bool bCaseInsensitive)
{
	TrimLeft(szTrim, bCaseInsensitive);
	TrimRight(szTrim, bCaseInsensitive);
	return *this;
}

//#################################################################################################
CStrW &CStrW::Trim(const wchar_t chTrim, const bool bCaseInsensitive)
{
	TrimLeft(chTrim, bCaseInsensitive);
	TrimRight(chTrim, bCaseInsensitive);
	return *this;
}

//#################################################################################################
CStrW &CStrW::Trim(void)
{
	TrimLeft();
	TrimRight();
	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimLeft(const CStrW &strTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || strTrim.m_nStrLen == 0)
		return *this;

	// Find first non-matching character
	PCWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNullW && wmemchri(strTrim.m_sz, *szScan, strTrim.m_nStrLen))
			++szScan;
	}
	else
	{
		while(*szScan != g_chNullW && std::wmemchr(strTrim.m_sz, *szScan, strTrim.m_nStrLen))
			++szScan;
	}

	if(*szScan == g_chNullW)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::wmemmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimLeft(PCWSTR szTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || szTrim == nullptr || *szTrim == g_chNullW)
		return *this;

	size_t nTrimLen = StringGetLength(szTrim);

	// Find first non-matching character
	PCWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		while(*szScan != g_chNullW && wmemchri(szTrim, *szScan, nTrimLen))
			++szScan;
	}
	else
	{
		while(*szScan != g_chNullW && std::wmemchr(szTrim, *szScan, nTrimLen))
			++szScan;
	}

	if(*szScan == g_chNullW)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::wmemmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimLeft(const wchar_t chTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || chTrim == g_chNullW)
		return *this;

	// Find first non-matching character
	PCWSTR szScan = m_sz;
	if(bCaseInsensitive)
	{
		wchar_t chLower = CharToLower(chTrim);
		while(*szScan != g_chNullW && CharToLower(*szScan) == chLower)
			++szScan;
	}
	else
	{
		while(*szScan != g_chNullW && *szScan == chTrim)
			++szScan;
	}

	if(*szScan == g_chNullW)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::wmemmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimLeft(void)
{
	if(m_nStrLen == 0)
		return *this;

	// Find first non-space character
	PCWSTR szScan = m_sz;
	while(*szScan != g_chNullW && IsSpaceW(*szScan))
		++szScan;

	if(*szScan == g_chNullW)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::wmemmove(m_sz, szScan, m_nStrLen + 1);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimRight(const CStrW &strTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || strTrim.m_nStrLen == 0)
		return *this;

	// Find last non-matching starting at the end
	PWSTR szScan = m_sz + m_nStrLen;
	if(bCaseInsensitive)
	{
		while(szScan != m_sz && wmemchri(strTrim.m_sz, *(szScan - 1), strTrim.m_nStrLen))
			--szScan;
	}
	else
	{
		while(szScan != m_sz && std::wmemchr(strTrim.m_sz, *(szScan - 1), strTrim.m_nStrLen))
			--szScan;
	}

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNullW;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimRight(PCWSTR szTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || szTrim == nullptr || *szTrim == g_chNullW)
		return *this;

	size_t nTrimLen = StringGetLength(szTrim);

	// Find last non-matching starting at the end
	PWSTR szScan = m_sz + m_nStrLen;
	if(bCaseInsensitive)
	{
		while(szScan != m_sz && wmemchri(szTrim, *(szScan - 1), nTrimLen))
			--szScan;
	}
	else
	{
		while(szScan != m_sz && std::wmemchr(szTrim, *(szScan - 1), nTrimLen))
			--szScan;
	}

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNullW;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimRight(const wchar_t chTrim, const bool bCaseInsensitive)
{
	if(m_nStrLen == 0 || chTrim == g_chNullW)
		return *this;

	// Find last non-matching starting at the end
	PWSTR szScan = m_sz + m_nStrLen;
	if(bCaseInsensitive)
	{
		wchar_t chLower = CharToLower(chTrim);
		while(szScan != m_sz && CharToLower(*(szScan - 1)) == chLower)
			--szScan;
	}
	else
	{
		while(szScan != m_sz && *(szScan - 1) == chTrim)
			--szScan;
	}

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNullW;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::TrimRight(void)
{
	if(m_nStrLen == 0)
		return *this;

	// Find last non-space starting at the end
	PWSTR szScan = m_sz + m_nStrLen;
	while(szScan != m_sz && IsSpaceW(*(szScan - 1)))
		--szScan;

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNullW;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::CropMid(const size_t nStart, const size_t nCount)
{
	if(nStart >= m_nStrLen || nCount == 0)
		Empty();
	else
	{
		m_nStrLen = MIN(nCount, m_nStrLen - nStart);
		std::wmemmove(m_sz, m_sz + nStart, m_nStrLen);
		m_sz[m_nStrLen] = g_chNullW;
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::CropLeft(const size_t nCount)
{
	if(nCount == 0)
		Empty();
	else if(nCount < m_nStrLen)
	{
		m_nStrLen = nCount;
		m_sz[m_nStrLen] = g_chNullW;
	}

	return *this;
}

//#################################################################################################
CStrW &CStrW::CropRight(const size_t nCount)
{
	if(nCount == 0)
		Empty();
	else if(nCount < m_nStrLen)
	{
		std::wmemmove(m_sz, m_sz + m_nStrLen - nCount, nCount + 1);
		m_nStrLen = nCount;
	}

	return *this;
}

//#################################################################################################
CStrW CStrW::GetUpper(void) const
{
	CStrW str(*this);
	StringToUpper(str.m_sz, str.m_nStrLen);

	return str;
}

//#################################################################################################
CStrW CStrW::GetLower(void) const
{
	CStrW str(*this);
	StringToLower(str.m_sz, str.m_nStrLen);

	return str;
}

//#################################################################################################
CStrW &CStrW::MakeUpper(void)
{
	StringToUpper(m_sz, m_nStrLen);

	return *this;
}

//#################################################################################################
CStrW &CStrW::MakeLower(void)
{
	StringToLower(m_sz, m_nStrLen);

	return *this;
}

//#################################################################################################
CStrW &CStrW::MakeReverse(void)
{
	if(m_nStrLen > 1)
	{
		PWSTR szFront = m_sz;
		PWSTR szBack = m_sz + m_nStrLen - 1;
		do{
			SWAP(*szFront, *szBack);
		}while(++szFront < --szBack);
	}

	return *this;
}

//#################################################################################################
CStrW CStrW::Tokenize(const CStrW &strTokens, size_t &nStart, const bool bCaseInsensitive) const
{
	if(strTokens.m_nStrLen == 0)
		return *this;

	if(nStart < m_nStrLen)
	{
		size_t nBegin = nStart;
		if(bCaseInsensitive)
		{
			while(nBegin < m_nStrLen)
			{
				if(!wmemchri(strTokens.m_sz, m_sz[nBegin], strTokens.m_nStrLen))
					break;
				++nBegin;
			}
		}
		else
		{
			while(nBegin < m_nStrLen)
			{
				if(!std::wmemchr(strTokens.m_sz, m_sz[nBegin], strTokens.m_nStrLen))
					break;
				++nBegin;
			}
		}

		if(nBegin < m_nStrLen)
		{
			size_t nEnd = nBegin + 1;
			if(bCaseInsensitive)
			{
				while(nEnd < m_nStrLen)
				{
					if(wmemchri(strTokens.m_sz, m_sz[nEnd], strTokens.m_nStrLen))
						break;
					++nEnd;
				}
			}
			else
			{
				while(nEnd < m_nStrLen)
				{
					if(std::wmemchr(strTokens.m_sz, m_sz[nEnd], strTokens.m_nStrLen))
						break;
					++nEnd;
				}
			}

			nStart = nEnd + 1;
			return GetMid(nBegin, nEnd - nBegin);
		}
	}

	// Nothing left to tokenize
	nStart = not_found;
	return CStrW();
}

//#################################################################################################
CStrW CStrW::Tokenize(PCWSTR szTokens, size_t &nStart, const bool bCaseInsensitive) const
{
	if(szTokens == nullptr || *szTokens == g_chNullW)
		return *this;

	if(nStart < m_nStrLen)
	{
		size_t nTokenLen = StringGetLength(szTokens);
		size_t nBegin = nStart;
		if(bCaseInsensitive)
		{
			while(nBegin < m_nStrLen)
			{
				if(!wmemchri(szTokens, m_sz[nBegin], nTokenLen))
					break;
				++nBegin;
			}
		}
		else
		{
			while(nBegin < m_nStrLen)
			{
				if(!std::wmemchr(szTokens, m_sz[nBegin], nTokenLen))
					break;
				++nBegin;
			}
		}

		if(nBegin < m_nStrLen)
		{
			size_t nEnd = nBegin + 1;
			if(bCaseInsensitive)
			{
				while(nEnd < m_nStrLen)
				{
					if(wmemchri(szTokens, m_sz[nEnd], nTokenLen))
						break;
					++nEnd;
				}
			}
			else
			{
				while(nEnd < m_nStrLen)
				{
					if(std::wmemchr(szTokens, m_sz[nEnd], nTokenLen))
						break;
					++nEnd;
				}
			}

			nStart = nEnd + 1;
			return GetMid(nBegin, nEnd - nBegin);
		}
	}

	// Nothing left to tokenize
	nStart = not_found;
	return CStrW();
}

//#################################################################################################
CStrW CStrW::Tokenize(const wchar_t chToken, size_t &nStart, const bool bCaseInsensitive) const
{
	if(chToken == g_chNullW)
		return *this;

	if(nStart < m_nStrLen)
	{
		size_t nBegin = nStart;
		if(bCaseInsensitive)
		{
			wchar_t chLower = CharToLower(chToken);
			while(nBegin < m_nStrLen)
			{
				if(CharToLower(m_sz[nBegin]) != chLower)
					break;
				++nBegin;
			}
		}
		else
		{
			while(nBegin < m_nStrLen)
			{
				if(chToken != m_sz[nBegin])
					break;
				++nBegin;
			}
		}

		if(nBegin < m_nStrLen)
		{
			size_t nEnd = nBegin + 1;
			if(bCaseInsensitive)
			{
				wchar_t chLower = CharToLower(chToken);
				while(nEnd < m_nStrLen)
				{
					if(CharToLower(m_sz[nEnd]) == chLower)
						break;
					++nEnd;
				}
			}
			else
			{
				while(nEnd < m_nStrLen)
				{
					if(chToken == m_sz[nEnd])
						break;
					++nEnd;
				}
			}

			nStart = nEnd + 1;
			return GetMid(nBegin, nEnd - nBegin);
		}
	}

	// Nothing left to tokenize
	nStart = not_found;
	return CStrW();
}

//#################################################################################################
bool CStrW::LoadString(const UINT nId)
{
	bool bResult = false;

	PWSTR sz = nullptr;
	int nResult = LoadStringW(GetModuleHandleW(nullptr), nId, (PWSTR)&sz, 0);
	if(nResult)
	{
		Assign(sz, 0, nResult);
		bResult = true;
	}

	return bResult;
}

//#################################################################################################
bool CStrW::Printf(PCWSTR szFormat, ...)
{
	bool bFormatted;

	va_list vaArgs;
	va_start(vaArgs, szFormat);
	bFormatted = PrintfV(szFormat, vaArgs);
	va_end(vaArgs);

	return bFormatted;
}

//#################################################################################################
bool CStrW::PrintfV(PCWSTR szFormat, va_list vaArgs)
{
	bool bFormatted = true;

	if(szFormat)
	{
		size_t nStrLen = 256;
		int nResult = std::vswprintf(GetBuffer(nStrLen), nStrLen, szFormat, vaArgs);
		if(nResult < 0)
		{
			Empty();
			bFormatted = false;
		}
		else
			ReleaseBuffer();
	}
	else
	{
		Empty();
		bFormatted = false;
	}

	return bFormatted;
}

//#################################################################################################
bool CStrW::ReserveBuffer(const size_t nBufLen)
{
	bool bAllocated;

	bool bPrevAllocated = (m_nBufLen != 0);
	bAllocated = Alloc(nBufLen);
	if(!bPrevAllocated && bAllocated)
	{
		*m_sz = g_chNullW;
		m_nStrLen = 0;
	}

	return bAllocated;
}

//#################################################################################################
size_t CStrW::GetBufferLength(void) const noexcept
{
	return m_nBufLen;
}

//#################################################################################################
size_t CStrW::GetBufferSize(void) const noexcept
{
	return m_nBufLen * sizeof(wchar_t);
}

//#################################################################################################
PWSTR CStrW::GetBuffer(const size_t nMinBufLen)
{
	Alloc(nMinBufLen);
	return m_sz;
}

//#################################################################################################
void CStrW::ReleaseBuffer(const size_t nNewStrLen)
{	// Calculate string length if new string length was not provided
	size_t nStrLen;
	if(nNewStrLen == end)
		nStrLen = StringGetLength(m_sz);
	else
		nStrLen = nNewStrLen;
	m_nStrLen = (nStrLen < m_nBufLen) ? nStrLen : m_nBufLen - 1;
	m_sz[m_nStrLen] = g_chNullW;
}

//#################################################################################################
void CStrW::FreeExtra(void)
{
	if(m_nStrLen == 0)
		Dealloc();
	else if(m_nStrLen + 1 < m_nBufLen)
	{
		PWSTR sz = (PWSTR)std::malloc((m_nStrLen + 1) * sizeof(wchar_t));
		if(sz)
		{
			std::wmemcpy(sz, m_sz, m_nStrLen + 1);
			std::free(m_sz);
			m_sz = sz;
			m_nBufLen = m_nStrLen + 1;
		}
	}
}

//#################################################################################################
void CStrW::DebugPrint(const bool bAppendNewline, const bool bConsolePrint) const
{
	if(m_nStrLen)
	{
		if(bConsolePrint)
		{
			std::wcout << m_sz;
			if(bAppendNewline)
				std::wcout << std::endl;
		}
		else
		{
			if(!bAppendNewline)
				OutputDebugStringW(m_sz);
			else
			{
				auto szTemp = std::make_unique<wchar_t[]>(m_nStrLen + g_nEolLen + 1);
				std::wmemcpy(szTemp.get(), m_sz, m_nStrLen);
				std::wmemcpy(szTemp.get() + m_nStrLen, g_szEolW, g_nEolLen);
				szTemp[m_nStrLen + g_nEolLen] = g_chNullW;

				OutputDebugStringW(szTemp.get());
			}
		}
	}
}

//#################################################################################################
bool CStrW::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		PWSTR sz;
		if(m_nBufLen)
			sz = (PWSTR)std::realloc(m_sz, nBufLen * sizeof(wchar_t));
		else
			sz = (PWSTR)std::malloc(nBufLen * sizeof(wchar_t));

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CStrW::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (PWSTR)g_szNullW;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
size_t CStrW::GetLength(PCWSTR sz, const size_t nMaxLen)
{
	size_t n = 0;

	if(sz)
	{
		size_t nLen = nMaxLen;
		while(nLen-- && *sz++ != g_chNullW)
			++n;
	}

	return n;
}

//#################################################################################################
PWSTR CStrW::FindStrPtr(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCWSTR szScan1 = sz;
		PWSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PWSTR)std::wmemchr(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(std::wmemcmp(szScan2, szFind, nFindLen) == 0)
				return szScan2;

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return nullptr;
}

//#################################################################################################
PWSTR CStrW::FindStrPtrI(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCWSTR szScan1 = sz;
		PWSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PWSTR)wmemchri(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(wmemcmpi(szScan2, szFind, nFindLen) == 0)
				return szScan2;

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return nullptr;
}

//#################################################################################################
size_t CStrW::FindStrIndex(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCWSTR szScan1 = sz;
		PCWSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PCWSTR)std::wmemchr(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(std::wmemcmp(szScan2, szFind, nFindLen) == 0)
				return (size_t)(szScan2 - sz);

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return not_found;
}

//#################################################################################################
size_t CStrW::FindStrIndexI(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen)
{
	size_t nRemaining = nStrLen;
	if(nFindLen <= nRemaining)
	{
		PCWSTR szScan1 = sz;
		PCWSTR szScan2;
		nRemaining -= nFindLen - 1;
		while((szScan2 = (PCWSTR)wmemchri(szScan1, *szFind, nRemaining)) != nullptr)
		{
			if(wmemcmpi(szScan2, szFind, nFindLen) == 0)
				return (size_t)(szScan2 - sz);

			nRemaining -= (size_t)(szScan2 - szScan1) + 1;
			szScan1 = szScan2 + 1;
		}
	}

	return not_found;
}

//#################################################################################################
PCWSTR CStrW::wmemchri(PCWSTR sz, const wchar_t ch, const size_t nLen)
{
	if(nLen)
	{
		PCWSTR szScan = sz;
		size_t nCount = 0;

		wchar_t chLower = CharToLower(ch);
		do{
			if(CharToLower(*szScan) == chLower)
				return szScan;
			++szScan;
		}while(++nCount < nLen);
	}

	return nullptr;
}

//#################################################################################################
int CStrW::wmemcmpi(PCWSTR sz1, PCWSTR sz2, const size_t nLen)
{
	int nResult = 0;

	if(nLen)
	{
		PCWSTR szScan1 = sz1;
		PCWSTR szScan2 = sz2;
		size_t nCount = 0;

		do{
			nResult = (int)CharToLower(*szScan1++) - (int)CharToLower(*szScan2++);
		}while(nResult == 0 && ++nCount < nLen);
	}

	return nResult;
}

//#################################################################################################
size_t CStrW::str2int(PCWSTR sz)
{
	return StringToIntegerW<size_t>(sz);
}




/////////////////////////////////////////////////
// CStr16
/////////////////////////////////////////////////

//#################################################################################################
CStr16::CStr16(void)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
}

//#################################################################################################
CStr16::CStr16(const CStr16 &src)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(src);
}

//#################################################################################################
CStr16::CStr16(CStr16 &&src) noexcept
	: m_sz(src.m_sz),
	m_nBufLen(src.m_nBufLen),
	m_nStrLen(src.m_nStrLen)
{
	src.m_sz = (char16_t*)g_szNull16;
	src.m_nBufLen = 0;
	src.m_nStrLen = 0;
}

//#################################################################################################
CStr16::CStr16(const CStr8 &str)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStr16::CStr16(const CStrW &str)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str);
}

//#################################################################################################
CStr16::CStr16(const char16_t *sz)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz);
}

//#################################################################################################
CStr16::CStr16(const char16_t ch)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(ch);
}

//#################################################################################################
CStr16::CStr16(const CStr16 &str, const size_t nStart, const size_t nLen)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(str, nStart, nLen);
}
//#################################################################################################
CStr16::CStr16(const char16_t *sz, const size_t nStart, const size_t nLen)
	: m_sz((char16_t*)g_szNull16),
	m_nBufLen(0),
	m_nStrLen(0)
{
	Assign(sz, nStart, nLen);
}

//#################################################################################################
CStr16::~CStr16(void)
{
	Dealloc();
}

//#################################################################################################
CStr16 &CStr16::Attach(char16_t *sz, const size_t nBufLen)
{
	Dealloc();

	if(sz)
	{
		m_sz = sz;
		m_nBufLen = nBufLen;
		m_nStrLen = StringGetLength(sz);
	}

	return *this;
}

//#################################################################################################
char16_t *CStr16::Detach(void) noexcept
{	// Release the buffer, it is up to the caller to dealloc the memory
	char16_t *sz = nullptr;

	if(m_nBufLen)
	{
		sz = m_sz;
		m_sz = (char16_t*)g_szNull16;
		m_nBufLen = m_nStrLen = 0;
	}

	return sz;
}

//#################################################################################################
void CStr16::SecureErase(void)
{	// Overwrite the memory of the string with zeros, great for sensitive data like passwords which otherwise could be left in memory when the string is freed
	for(size_t n = 0; n < m_nBufLen; ++n)
		m_sz[n] = g_chNull16;
	m_nStrLen = 0;
}

//#################################################################################################
size_t CStr16::GetLength(const bool bIncludeNullTerm) const noexcept
{	// Length in characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CStr16::GetCount(const bool bIncludeNullTerm) const noexcept
{	// Count of characters
	return (bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen;
}

//#################################################################################################
size_t CStr16::GetSize(const bool bIncludeNullTerm) const noexcept
{	// Size in bytes
	return ((bIncludeNullTerm) ? m_nStrLen + 1 : m_nStrLen) * sizeof(char16_t);
}

//#################################################################################################
bool CStr16::IsEmpty(void) const noexcept
{
	return (m_nStrLen == 0);
}

//#################################################################################################
void CStr16::Empty(const bool bFree)
{
	if(bFree)
		Dealloc();
	else
	{
		if(m_nBufLen)
			*m_sz = g_chNull16;
		else
			m_sz = (char16_t*)g_szNull16;
		m_nStrLen = 0;
	}
}

//#################################################################################################
char16_t CStr16::GetFirst(void) const
{
	return (m_nStrLen) ? m_sz[0] : g_chNull16;
}

//#################################################################################################
char16_t CStr16::GetLast(void) const
{
	return (m_nStrLen) ? m_sz[m_nStrLen - 1] : g_chNull16;
}

//#################################################################################################
char16_t CStr16::GetAt(const size_t nIndex) const
{
	return (nIndex < m_nStrLen) ? m_sz[nIndex] : g_chNull16;
}

//#################################################################################################
void CStr16::SetAt(const size_t nIndex, const char16_t ch)
{
	if(nIndex < m_nStrLen)
	{
		m_sz[nIndex] = ch;
		if(ch == g_chNull16)
			m_nStrLen = nIndex;
	}
}

//#################################################################################################
CStr16::operator const char16_t*(void) const noexcept
{
	return m_sz;
}

//#################################################################################################
CStr8 CStr16::AsUtf8(void) const
{
	return CStr8(*this);
}

//#################################################################################################
CStrW CStr16::AsWide(void) const
{
	return CStrW(*this);
}

//#################################################################################################
CStrW CStr16::AsNative(void) const
{
	return CStrW(*this);
}

//#################################################################################################
CStr16 &CStr16::operator=(const CStr16 &src)
{
	Assign(src);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator=(CStr16 &&src) noexcept
{
	if(this != &src)
	{
		if(m_nBufLen)
			std::free(m_sz);

		m_sz = src.m_sz;
		m_nBufLen = src.m_nBufLen;
		m_nStrLen = src.m_nStrLen;

		src.m_sz = (char16_t*)g_szNull16;
		src.m_nBufLen = 0;
		src.m_nStrLen = 0;
	}

	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator=(const CStr8 &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator=(const CStrW &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator=(const char16_t *sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator=(const char16_t ch)
{
	Assign(ch);
	return *this;
}

//#################################################################################################
void CStr16::Assign(const CStr16 &str)
{
	if(this != &str)
	{
		if(str.m_nStrLen)
		{	// Alloc and copy
			if(Alloc(str.m_nStrLen + 1))
			{
				std::memcpy(m_sz, str.m_sz, str.m_nStrLen * sizeof(char16_t));
				m_nStrLen = str.m_nStrLen;
				m_sz[m_nStrLen] = g_chNull16;
			}
			else
				Empty();
		}
		else
			Empty();
	}
}

//#################################################################################################
void CStr16::Assign(const CStr8 &str)
{
	if(str.m_nStrLen)
	{	// Alloc and copy
		if(Alloc(Utf8ToUtf16(str.m_sz, str.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = Utf8ToUtf16(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull16;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr16::Assign(const CStrW &str)
{
	if(str.m_nStrLen)
	{	// Alloc and copy
		if(Alloc(WideToUtf16(str.m_sz, str.m_nStrLen, nullptr, 0) + 1))
		{
			m_nStrLen = WideToUtf16(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_sz[m_nStrLen] = g_chNull16;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr16::Assign(const char16_t *sz)
{
	if(sz && *sz != g_chNull16)
	{	// Alloc and copy
		size_t nStrLen = StringGetLength(sz);
		if(Alloc(nStrLen + 1))
		{
			std::memcpy(m_sz, sz, nStrLen * sizeof(char16_t));
			m_nStrLen = nStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr16::Assign(const char16_t ch)
{
	if(ch != g_chNull16)
	{	// Alloc and set
		if(Alloc(2))
		{
			*m_sz = ch;
			m_nStrLen = 1;
			m_sz[m_nStrLen] = g_chNull16;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr16::Assign(const CStr16 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{	// Alloc and copy
		size_t nNewLen;
		if(nLen == end)
			nNewLen = str.m_nStrLen - nStart;
		else
			nNewLen = MIN(nLen, str.m_nStrLen - nStart);

		if(Alloc(nNewLen + 1))
		{
			std::memcpy(m_sz, str.m_sz + nStart, nNewLen * sizeof(char16_t));
			m_nStrLen = nNewLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
void CStr16::Assign(const char16_t *sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull16 && nLen)
	{	// Alloc and copy
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nNewLen;
			if(nLen == end)
				nNewLen = nStrLen - nStart;
			else
				nNewLen = MIN(nLen, nStrLen - nStart);

			if(Alloc(nNewLen + 1))
			{
				std::memcpy(m_sz, sz + nStart, nNewLen * sizeof(char16_t));
				m_nStrLen = nNewLen;
				m_sz[m_nStrLen] = g_chNull16;
			}
			else
				Empty();
		}
		else
			Empty();
	}
	else
		Empty();
}

//#################################################################################################
CStr16 &CStr16::operator+=(const CStr16 &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator+=(const CStr8 &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator+=(const CStrW &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator+=(const char16_t *sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
CStr16 &CStr16::operator+=(const char16_t ch)
{
	Append(ch);
	return *this;
}

//#################################################################################################
void CStr16::Prepend(const CStr16 &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + str.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + str.m_nStrLen, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			else
				m_sz[str.m_nStrLen] = g_chNull16;
			std::memcpy(m_sz, str.m_sz, str.m_nStrLen * sizeof(char16_t));
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr16::Prepend(const CStr8 &str)
{
	if(str.m_nStrLen)
	{
		size_t nStrLen = Utf8ToUtf16(str.m_sz, str.m_nStrLen, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			else
				m_sz[nStrLen] = g_chNull16;
			Utf8ToUtf16(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr16::Prepend(const CStrW &str)
{
	if(str.m_nStrLen)
	{
		size_t nStrLen = WideToUtf16(str.m_sz, str.m_nStrLen, nullptr, 0);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			else
				m_sz[nStrLen] = g_chNull16;
			WideToUtf16(str.m_sz, str.m_nStrLen, m_sz, m_nBufLen);
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr16::Prepend(const char16_t *sz)
{
	if(sz && *sz != g_chNull16)
	{
		size_t nStrLen = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nStrLen, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			else
				m_sz[nStrLen] = g_chNull16;
			std::memcpy(m_sz, sz, nStrLen * sizeof(char16_t));
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr16::Prepend(const char16_t ch)
{
	if(ch != g_chNull16)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + 1, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			else
				m_sz[1] = g_chNull16;
			m_sz[0] = ch;
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr16::Prepend(const CStr16 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nPrependLen;
		if(nLen == end)
			nPrependLen = str.m_nStrLen - nStart;
		else
			nPrependLen = MIN(nLen, str.m_nStrLen - nStart);

		size_t nNewStrLen = m_nStrLen + nPrependLen;
		if(Alloc(nNewStrLen + 1))
		{
			if(m_nStrLen)
				std::memmove(m_sz + nPrependLen, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			else
				m_sz[nPrependLen] = g_chNull16;
			std::memcpy(m_sz, str.m_sz + nStart, nPrependLen * sizeof(char16_t));
			m_nStrLen = nNewStrLen;
		}
	}
}

//#################################################################################################
void CStr16::Prepend(const char16_t *sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull16 && nLen)
	{
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nPrependLen;
			if(nLen == end)
				nPrependLen = nStrLen - nStart;
			else
				nPrependLen = MIN(nLen, nStrLen - nStart);

			size_t nNewStrLen = m_nStrLen + nPrependLen;
			if(Alloc(nNewStrLen + 1))
			{
				if(m_nStrLen)
					std::memmove(m_sz + nPrependLen, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
				else
					m_sz[nPrependLen] = g_chNull16;
				std::memcpy(m_sz, sz + nStart, nPrependLen * sizeof(char16_t));
				m_nStrLen = nNewStrLen;
			}
		}
	}
}

//#################################################################################################
void CStr16::Append(const CStr16 &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + str.m_nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memcpy(m_sz + m_nStrLen, str.m_sz, str.m_nStrLen * sizeof(char16_t));
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
	}
}

//#################################################################################################
void CStr16::Append(const CStr8 &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + Utf8ToUtf16(str.m_sz, str.m_nStrLen, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			Utf8ToUtf16(str.m_sz, str.m_nStrLen, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
	}
}

//#################################################################################################
void CStr16::Append(const CStrW &str)
{
	if(str.m_nStrLen)
	{
		size_t nNewStrLen = m_nStrLen + WideToUtf16(str.m_sz, str.m_nStrLen, nullptr, 0);
		if(Alloc(nNewStrLen + 1))
		{
			WideToUtf16(str.m_sz, str.m_nStrLen, m_sz + m_nStrLen, m_nBufLen - m_nStrLen);
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
	}
}

//#################################################################################################
void CStr16::Append(const char16_t *sz)
{
	if(sz && *sz != g_chNull16)
	{
		size_t nStrLen = StringGetLength(sz);
		size_t nNewStrLen = m_nStrLen + nStrLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memcpy(m_sz + m_nStrLen, sz, nStrLen * sizeof(char16_t));
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
	}
}

//#################################################################################################
void CStr16::Append(const char16_t ch)
{
	if(ch != g_chNull16)
	{
		size_t nNewStrLen = m_nStrLen + 1;
		if(Alloc(nNewStrLen + 1))
		{
			m_sz[m_nStrLen] = ch;
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
	}
}

//#################################################################################################
void CStr16::Append(const CStr16 &str, const size_t nStart, const size_t nLen)
{
	if(str.m_nStrLen && nStart < str.m_nStrLen && nLen)
	{
		size_t nAppendLen;
		if(nLen == end)
			nAppendLen = str.m_nStrLen - nStart;
		else
			nAppendLen = MIN(nLen, str.m_nStrLen - nStart);

		size_t nNewStrLen = m_nStrLen + nAppendLen;
		if(Alloc(nNewStrLen + 1))
		{
			std::memcpy(m_sz + m_nStrLen, str.m_sz + nStart, nAppendLen * sizeof(char16_t));
			m_nStrLen = nNewStrLen;
			m_sz[m_nStrLen] = g_chNull16;
		}
	}
}

//#################################################################################################
void CStr16::Append(const char16_t *sz, const size_t nStart, const size_t nLen)
{
	if(sz && *sz != g_chNull16 && nLen)
	{
		size_t nStrLen = (nLen == end) ? StringGetLength(sz) : GetLength(sz, nStart + nLen);
		if(nStart < nStrLen)
		{
			size_t nAppendLen;
			if(nLen == end)
				nAppendLen = nStrLen - nStart;
			else
				nAppendLen = MIN(nLen, nStrLen - nStart);

			size_t nNewStrLen = m_nStrLen + nAppendLen;
			if(Alloc(nNewStrLen + 1))
			{
				std::memcpy(m_sz + m_nStrLen, sz + nStart, nAppendLen * sizeof(char16_t));
				m_nStrLen = nNewStrLen;
				m_sz[m_nStrLen] = g_chNull16;
			}
		}
	}
}

//#################################################################################################
bool CStr16::operator==(const CStr16 &str) const
{
	return (StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) == 0);
}

//#################################################################################################
bool CStr16::operator==(const char16_t *sz) const
{
	return (StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz)) == 0);
}

//#################################################################################################
bool CStr16::operator==(const char16_t ch) const
{
	return (StringCompare16(m_sz, m_nStrLen, &ch, 1) == 0);
}

//#################################################################################################
bool CStr16::operator!=(const CStr16 &str) const
{
	return (StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) != 0);
}

//#################################################################################################
bool CStr16::operator!=(const char16_t *sz) const
{
	return (StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz)) != 0);
}

//#################################################################################################
bool CStr16::operator!=(const char16_t ch) const
{
	return (StringCompare16(m_sz, m_nStrLen, &ch, 1) != 0);
}

//#################################################################################################
bool CStr16::operator<(const CStr16 &str) const
{
	return (StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) < 0);
}

//#################################################################################################
bool CStr16::operator<(const char16_t *sz) const
{
	return (StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz)) < 0);
}

//#################################################################################################
bool CStr16::operator<(const char16_t ch) const
{
	return (StringCompare16(m_sz, m_nStrLen, &ch, 1) < 0);
}

//#################################################################################################
bool CStr16::operator<=(const CStr16 &str) const
{
	return (StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) <= 0);
}

//#################################################################################################
bool CStr16::operator<=(const char16_t *sz) const
{
	return (StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz)) <= 0);
}

//#################################################################################################
bool CStr16::operator<=(const char16_t ch) const
{
	return (StringCompare16(m_sz, m_nStrLen, &ch, 1) <= 0);
}

//#################################################################################################
bool CStr16::operator>(const CStr16 &str) const
{
	return (StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) > 0);
}

//#################################################################################################
bool CStr16::operator>(const char16_t *sz) const
{
	return (StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz)) > 0);
}

//#################################################################################################
bool CStr16::operator>(const char16_t ch) const
{
	return (StringCompare16(m_sz, m_nStrLen, &ch, 1) > 0);
}

//#################################################################################################
bool CStr16::operator>=(const CStr16 &str) const
{
	return (StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen) >= 0);
}

//#################################################################################################
bool CStr16::operator>=(const char16_t *sz) const
{
	return (StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz)) >= 0);
}

//#################################################################################################
bool CStr16::operator>=(const char16_t ch) const
{
	return (StringCompare16(m_sz, m_nStrLen, &ch, 1) >= 0);
}

//#################################################################################################
int CStr16::Compare(const CStr16 &str, const bool bCaseInsensitive) const
{
	return StringCompare16(m_sz, m_nStrLen, str.m_sz, str.m_nStrLen, bCaseInsensitive);
}

//#################################################################################################
int CStr16::Compare(const char16_t *sz, const bool bCaseInsensitive) const
{
	return StringCompare16(m_sz, m_nStrLen, sz, StringGetLength(sz), bCaseInsensitive);
}

//#################################################################################################
int CStr16::Compare(const char16_t ch, const bool bCaseInsensitive) const
{
	return StringCompare16(m_sz, m_nStrLen, &ch, 1, bCaseInsensitive);
}

//#################################################################################################
int CStr16::CompareMid(const CStr16 &str, const size_t nStart, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
	nCompareRtn = StringCompare16(m_sz + nOffset, MIN(m_nStrLen - nOffset, nCount), str.m_sz, MIN(str.m_nStrLen, nCount), bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr16::CompareMid(const char16_t *sz, const size_t nStart, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = GetLength(sz, nCount);
	size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
	nCompareRtn = StringCompare16(m_sz + nOffset, MIN(m_nStrLen - nOffset, nCount), sz, nStrLen, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr16::CompareMid(const char16_t ch, const size_t nStart, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNull16)
	{
		size_t nOffset = (nStart > m_nStrLen) ? m_nStrLen : nStart;
		nCompareRtn = StringCompare16(m_sz + nOffset, MIN(m_nStrLen - nOffset, 1), &ch, 1, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
int CStr16::CompareLeft(const CStr16 &str, const size_t nCount, const bool bCaseInsensitive) const
{
	return StringCompare16(m_sz, MIN(m_nStrLen, nCount), str.m_sz, MIN(str.m_nStrLen, nCount), bCaseInsensitive);
}

//#################################################################################################
int CStr16::CompareLeft(const char16_t *sz, const size_t nCount, const bool bCaseInsensitive) const
{
	return StringCompare16(m_sz, MIN(m_nStrLen, nCount), sz, GetLength(sz, nCount), bCaseInsensitive);
}

//#################################################################################################
int CStr16::CompareLeft(const char16_t ch, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNull16)
		nCompareRtn = StringCompare16(m_sz, MIN(m_nStrLen, 1), &ch, 1, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr16::CompareRight(const CStr16 &str, const size_t nCount, const bool bCaseInsensitive) const
{
	size_t nStrLen1 = MIN(m_nStrLen, nCount);
	size_t nStrLen2 = MIN(str.m_nStrLen, nCount);

	return StringCompare16(m_sz + (m_nStrLen - nStrLen1), nStrLen1, str.m_sz + (str.m_nStrLen - nStrLen2), nStrLen2, bCaseInsensitive);
}

//#################################################################################################
int CStr16::CompareRight(const char16_t *sz, const size_t nCount, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	size_t nStrLen = StringGetLength(sz);
	size_t nStrLen1 = MIN(m_nStrLen, nCount);
	size_t nStrLen2 = MIN(nStrLen, nCount);
	nCompareRtn = StringCompare16(m_sz + (m_nStrLen - nStrLen1), nStrLen1, sz + (nStrLen - nStrLen2), nStrLen2, bCaseInsensitive);

	return nCompareRtn;
}

//#################################################################################################
int CStr16::CompareRight(const char16_t ch, const bool bCaseInsensitive) const
{
	int nCompareRtn = 0;

	if(ch != g_chNull16)
	{
		size_t nStrLen1 = MIN(m_nStrLen, 1);

		nCompareRtn = StringCompare16(m_sz + (m_nStrLen - nStrLen1), nStrLen1, &ch, 1, bCaseInsensitive);
	}

	return nCompareRtn;
}

//#################################################################################################
void CStr16::Swap(CStr16 &src) noexcept
{
	SWAP(m_sz, src.m_sz);
	SWAP(m_nBufLen, src.m_nBufLen);
	SWAP(m_nStrLen, src.m_nStrLen);
}

//#################################################################################################
CStr16 CStr16::GetMid(const size_t nStart, const size_t nCount) const
{
	CStr16 str;

	if(nStart < m_nStrLen)
	{	// Copy nCount chars, unless nCount is zero or nStart + nCount is too much
		size_t nStrLen = (nCount == end || nStart + nCount > m_nStrLen) ? m_nStrLen - nStart : nCount;
		if(str.Alloc(nStrLen + 1))
		{
			std::memcpy(str.m_sz, m_sz + nStart, nStrLen * sizeof(char16_t));
			str.m_nStrLen = nStrLen;
			str.m_sz[str.m_nStrLen] = g_chNull16;
		}
	}

	return str;
}

//#################################################################################################
CStr16 CStr16::GetLeft(const size_t nCount) const
{
	CStr16 str;

	if(nCount >= m_nStrLen)
		str.Assign(*this);
	else
	{
		if(str.Alloc(nCount + 1))
		{
			std::memcpy(str.m_sz, m_sz, nCount * sizeof(char16_t));
			str.m_nStrLen = nCount;
			str.m_sz[str.m_nStrLen] = g_chNull16;
		}
	}

	return str;
}

//#################################################################################################
CStr16 CStr16::GetRight(const size_t nCount) const
{
	CStr16 str;

	if(nCount >= m_nStrLen)
		str.Assign(*this);
	else
	{
		if(str.Alloc(nCount + 1))
		{
			std::memcpy(str.m_sz, m_sz + m_nStrLen - nCount, nCount * sizeof(char16_t));
			str.m_nStrLen = nCount;
			str.m_sz[str.m_nStrLen] = g_chNull16;
		}
	}

	return str;
}

//#################################################################################################
CStr16 &CStr16::Trim(void)
{
	TrimLeft();
	TrimRight();
	return *this;
}

//#################################################################################################
CStr16 &CStr16::TrimLeft(void)
{
	if(m_nStrLen == 0)
		return *this;

	// Find first non-space character
	const char16_t *szScan = m_sz;
	while(*szScan != g_chNull16 && IsSpace16(*szScan))
		++szScan;

	if(*szScan == g_chNull16)
		Empty();
	else if(szScan != m_sz)
	{
		m_nStrLen -= (size_t)(szScan - m_sz);
		std::memmove(m_sz, szScan, (m_nStrLen + 1) * sizeof(char16_t));
	}

	return *this;
}

//#################################################################################################
CStr16 &CStr16::TrimRight(void)
{
	if(m_nStrLen == 0)
		return *this;

	// Find last non-space starting at the end
	char16_t *szScan = m_sz + m_nStrLen;
	while(szScan != m_sz && IsSpace16(*(szScan - 1)))
		--szScan;

	if(szScan == m_sz)
		Empty();
	else if(szScan != m_sz + m_nStrLen)
	{	// Truncate at current scan location
		*szScan = g_chNull16;
		m_nStrLen = (size_t)(szScan - m_sz);
	}

	return *this;
}

//#################################################################################################
CStr16 &CStr16::CropMid(const size_t nStart, const size_t nCount)
{
	if(nStart >= m_nStrLen || nCount == 0)
		Empty();
	else
	{
		m_nStrLen = MIN(nCount, m_nStrLen - nStart);
		std::memmove(m_sz, m_sz + nStart, m_nStrLen * sizeof(char16_t));
		m_sz[m_nStrLen] = g_chNull16;
	}

	return *this;
}

//#################################################################################################
CStr16 &CStr16::CropLeft(const size_t nCount)
{
	if(nCount == 0)
		Empty();
	else if(nCount < m_nStrLen)
	{
		m_nStrLen = nCount;
		m_sz[m_nStrLen] = g_chNull16;
	}

	return *this;
}

//#################################################################################################
CStr16 &CStr16::CropRight(const size_t nCount)
{
	if(nCount == 0)
		Empty();
	else if(nCount < m_nStrLen)
	{
		std::memmove(m_sz, m_sz + m_nStrLen - nCount, (nCount + 1) * sizeof(char16_t));
		m_nStrLen = nCount;
	}

	return *this;
}

//#################################################################################################
CStr16 CStr16::GetUpper(void) const
{
	CStr16 str(*this);
	StringToUpper(str.m_sz, str.m_nStrLen);

	return str;
}

//#################################################################################################
CStr16 CStr16::GetLower(void) const
{
	CStr16 str(*this);
	StringToLower(str.m_sz, str.m_nStrLen);

	return str;
}

//#################################################################################################
CStr16 &CStr16::MakeUpper(void)
{
	StringToUpper(m_sz, m_nStrLen);

	return *this;
}

//#################################################################################################
CStr16 &CStr16::MakeLower(void)
{
	StringToLower(m_sz, m_nStrLen);

	return *this;
}

//#################################################################################################
CStr16 &CStr16::MakeReverse(void)
{
	if(m_nStrLen > 1)
	{
		char16_t *szFront = m_sz;
		char16_t *szBack = m_sz + m_nStrLen - 1;
		do{
			SWAP(*szFront, *szBack);
		}while(++szFront < --szBack);
	}

	return *this;
}

//#################################################################################################
bool CStr16::ReserveBuffer(const size_t nBufLen)
{
	bool bAllocated;

	bool bPrevAllocated = (m_nBufLen != 0);
	bAllocated = Alloc(nBufLen);
	if(!bPrevAllocated && bAllocated)
	{
		*m_sz = g_chNull16;
		m_nStrLen = 0;
	}

	return bAllocated;
}

//#################################################################################################
size_t CStr16::GetBufferLength(void) const noexcept
{
	return m_nBufLen;
}

//#################################################################################################
size_t CStr16::GetBufferSize(void) const noexcept
{
	return m_nBufLen * sizeof(char16_t);
}

//#################################################################################################
char16_t *CStr16::GetBuffer(const size_t nMinBufLen)
{
	Alloc(nMinBufLen);
	return m_sz;
}

//#################################################################################################
void CStr16::ReleaseBuffer(const size_t nNewStrLen)
{	// Calculate string length if new string length was not provided
	size_t nStrLen;
	if(nNewStrLen == end)
		nStrLen = StringGetLength(m_sz);
	else
		nStrLen = nNewStrLen;
	m_nStrLen = (nStrLen < m_nBufLen) ? nStrLen : m_nBufLen - 1;
	m_sz[m_nStrLen] = g_chNull16;
}

//#################################################################################################
void CStr16::FreeExtra(void)
{
	if(m_nStrLen == 0)
		Dealloc();
	else if(m_nStrLen + 1 < m_nBufLen)
	{
		char16_t *sz = (char16_t*)std::malloc((m_nStrLen + 1) * sizeof(char16_t));
		if(sz)
		{
			std::memcpy(sz, m_sz, (m_nStrLen + 1) * sizeof(char16_t));
			std::free(m_sz);
			m_sz = sz;
			m_nBufLen = m_nStrLen + 1;
		}
	}
}

//#################################################################################################
void CStr16::DebugPrint(const bool bAppendNewline, const bool bConsolePrint) const
{
	if(m_nStrLen)
	{
		if(bConsolePrint)
		{
			std::wcout << (PCWSTR)m_sz;
			if(bAppendNewline)
				std::wcout << std::endl;
		}
		else
		{
			if(!bAppendNewline)
				OutputDebugStringW((PCWSTR)m_sz);
			else
			{
				auto szTemp = std::make_unique<wchar_t[]>(m_nStrLen + g_nEolLen + 1);
				std::wmemcpy(szTemp.get(), (PCWSTR)m_sz, m_nStrLen);
				std::wmemcpy(szTemp.get() + m_nStrLen, g_szEolW, g_nEolLen);
				szTemp[m_nStrLen + g_nEolLen] = g_chNull16;

				OutputDebugStringW(szTemp.get());
			}
		}
	}
}

//#################################################################################################
bool CStr16::Alloc(const size_t nBufLen)
{
	bool bAllocated = true;

	if(nBufLen > m_nBufLen)
	{	// The buffer is not large enough, realloc it
		char16_t *sz;
		if(m_nBufLen)
			sz = (char16_t*)std::realloc(m_sz, nBufLen * sizeof(char16_t));
		else
			sz = (char16_t*)std::malloc(nBufLen * sizeof(char16_t));

		if(sz)
		{
			m_sz = sz;
			m_nBufLen = nBufLen;
		}
		else
			bAllocated = false;
	}

	return bAllocated;
}

//#################################################################################################
void CStr16::Dealloc(void)
{
	if(m_nBufLen)
		std::free(m_sz);
	m_sz = (char16_t*)g_szNull16;
	m_nStrLen = m_nBufLen = 0;
}

//#################################################################################################
size_t CStr16::GetLength(const char16_t *sz, const size_t nMaxLen)
{
	size_t n = 0;

	if(sz)
	{
		size_t nLen = nMaxLen;
		while(nLen-- && *sz++ != g_chNull16)
			++n;
	}

	return n;
}
