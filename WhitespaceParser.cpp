#include "WhitespaceParser.h"

static wchar_t g_chDoubleQuote = L'"';
static wchar_t g_chSpace = L' ';
static PCWSTR g_szWhitespace = L" \t";

//#################################################################################################
CWhitespaceParser::CWhitespaceParser(CWhitespaceParser &&src) noexcept
	: m_vecParams(std::move(src.m_vecParams))
{
}

//#################################################################################################
CWhitespaceParser::CWhitespaceParser(PCWSTR sz)
{
	Assign(sz);
}

//#################################################################################################
CWhitespaceParser::CWhitespaceParser(const CStr &str)
{
	Assign(str);
}

//#################################################################################################
size_t CWhitespaceParser::GetCount(void) const
{
	return m_vecParams.size();
}

//#################################################################################################
size_t CWhitespaceParser::GetLength(const bool bIncludeNullTerm) const
{	// Length in characters
	size_t nStrLen = 0;

	if(!m_vecParams.empty())
	{
		for(const auto &param : m_vecParams)
		{
			if(nStrLen)
				++nStrLen;
			nStrLen += param.GetLength();
			if(param.ContainsOneOf(g_szWhitespace))
				nStrLen += 2;
		}

		if(bIncludeNullTerm)
			++nStrLen;
	}

	return nStrLen;
}

//#################################################################################################
size_t CWhitespaceParser::GetSize(const bool bIncludeNullTerm) const
{	// Size in bytes
	return GetLength(bIncludeNullTerm) * sizeof(wchar_t);
}

//#################################################################################################
bool CWhitespaceParser::IsEmpty(void) const noexcept
{
	return m_vecParams.empty();
}

//#################################################################################################
void CWhitespaceParser::Empty(void)
{
	m_vecParams.clear();
}

//#################################################################################################
CStr CWhitespaceParser::Get(void) const
{
	CStr str;

	for(const auto &param : m_vecParams)
	{
		size_t nWhitespaceIndex;

		if(!str.IsEmpty())
			str += g_chSpace;

		if((nWhitespaceIndex = param.FindFirstOf(g_szWhitespace)) != CStr::not_found)
			str += g_chDoubleQuote;
		str += param;
		if(nWhitespaceIndex != CStr::not_found)
			str += g_chDoubleQuote;
	}

	return str;
}

//#################################################################################################
CStr CWhitespaceParser::Get(const size_t nParam) const
{
	CStr strParam;

	if(nParam < m_vecParams.size())
		strParam = m_vecParams[nParam];

	return strParam;
}

//#################################################################################################
CWhitespaceParser &CWhitespaceParser::operator=(CWhitespaceParser &&src) noexcept
{
	if(this != &src)
		m_vecParams = std::move(src.m_vecParams);

	return *this;
}

//#################################################################################################
CWhitespaceParser &CWhitespaceParser::operator=(PCWSTR sz)
{
	Assign(sz);
	return *this;
}

//#################################################################################################
CWhitespaceParser &CWhitespaceParser::operator=(const CStr &str)
{
	Assign(str);
	return *this;
}

//#################################################################################################
void CWhitespaceParser::Assign(PCWSTR sz)
{
	Empty();
	Append(CStr(sz));
}

//#################################################################################################
void CWhitespaceParser::Assign(const CStr &str)
{
	Empty();
	Append(str);
}

//#################################################################################################
void CWhitespaceParser::Assign(const CWhitespaceParser &src)
{
	if(this != &src)
		m_vecParams = src.m_vecParams;
}

//#################################################################################################
CWhitespaceParser &CWhitespaceParser::operator+=(PCWSTR sz)
{
	Append(sz);
	return *this;
}

//#################################################################################################
CWhitespaceParser &CWhitespaceParser::operator+=(const CStr &str)
{
	Append(str);
	return *this;
}

//#################################################################################################
CWhitespaceParser &CWhitespaceParser::operator+=(const CWhitespaceParser &src)
{
	Append(src);
	return *this;
}

//#################################################################################################
void CWhitespaceParser::Append(PCWSTR sz)
{
	Append(CStr(sz));
}

//#################################################################################################
void CWhitespaceParser::Append(const CStr &str)
{
	size_t nStart = str.FindFirstNotOf(g_szWhitespace);
	size_t nEnd;
	if(nStart == CStr::not_found)
		nStart = 0;

	while(nStart < str.GetLength())
	{
		if(str.GetAt(nStart) == g_chDoubleQuote)
		{
			if(++nStart >= str.GetLength())
				break;

			nEnd = str.Find(g_chDoubleQuote, nStart);
			if(nEnd == CStr::not_found)
			{
				m_vecParams.emplace_back(str, nStart);
				break;
			}
			else
			{
				if(nEnd != nStart)
					m_vecParams.emplace_back(str, nStart, nEnd - nStart);
				nStart = nEnd + 1;
			}
		}
		else
		{
			nEnd = str.FindFirstOf(g_szWhitespace, nStart);
			if(nEnd == CStr::not_found)
			{
				m_vecParams.emplace_back(str, nStart);
				break;
			}
			else
			{
				m_vecParams.emplace_back(str, nStart, nEnd - nStart);
				nStart = nEnd;
			}
		}

		nStart = str.FindFirstNotOf(g_szWhitespace, nStart);
		if(nStart == CStr::not_found)
			break;
	}
}

//#################################################################################################
void CWhitespaceParser::Append(const CWhitespaceParser &src)
{
	if(this != &src)
	{
		for(const auto &param : src.m_vecParams)
			m_vecParams.push_back(param);
	}
}

//#################################################################################################
void CWhitespaceParser::Insert(const size_t nParam, const CStr &str)
{
	if(m_vecParams.empty())
		Assign(str);
	else if(!str.IsEmpty())
	{
		size_t nCount = nParam;
		auto itr = m_vecParams.begin();
		while(itr != m_vecParams.end() && nCount-- > 0)
			++itr;

		m_vecParams.insert(itr, str);
	}
}

//#################################################################################################
void CWhitespaceParser::Delete(const size_t nParam, const size_t nParamCount)
{
	if(nParam < m_vecParams.size() && nParamCount > 0)
	{
		if(nParam == 0 && nParamCount >= m_vecParams.size())
			Empty();
		else
		{
			size_t nCount = nParam;
			auto itr = m_vecParams.begin();
			while(itr != m_vecParams.end() && nCount-- > 0)
				++itr;

			nCount = nParamCount;
			while(itr != m_vecParams.end() && nCount-- > 0)
				itr = m_vecParams.erase(itr);
		}
	}
}
