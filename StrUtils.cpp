#include "StrUtils.h"

// Macros for multibyte characters
#define IS_MB_1BYTE(x)	(((x) & 0x80) == 0x00)
#define IS_MB_2BYTE(x)	(((x) & 0xE0) == 0xC0)
#define IS_MB_3BYTE(x)	(((x) & 0xF0) == 0xE0)
#define IS_MB_4BYTE(x)	(((x) & 0xF8) == 0xF0)
#define IS_MB_XBYTE(x)	(((x) & 0xC0) == 0x80)

//#################################################################################################
size_t Utf8ToWide(PCSTR sz8, const size_t nStrLen8, PWSTR szW, const size_t nStrLenW)
{
	size_t nLenW = 0;

	if(nStrLen8)
		nLenW = MultiByteToWideChar(CP_UTF8, 0, sz8, (int)nStrLen8, szW, (int)nStrLenW);

	return nLenW;
}

//#################################################################################################
size_t Utf8ToWide(const char ch8, PWSTR szW, const size_t nStrLenW)
{
	size_t nLenW = 0;

	if(ch8 != '\0')
		nLenW = MultiByteToWideChar(CP_UTF8, 0, &ch8, 1, szW, (int)nStrLenW);

	return nLenW;
}

//#################################################################################################
size_t Utf8ToUtf16(PCSTR sz8, const size_t nStrLen8, char16_t *sz16, const size_t nStrLen16)
{
	size_t nLen16 = 0;

	if(nStrLen8)
		nLen16 = MultiByteToWideChar(CP_UTF8, 0, sz8, (int)nStrLen8, (PWSTR)sz16, (int)nStrLen16);

	return nLen16;
}

//#################################################################################################
size_t Utf8ToUtf16(const char ch8, char16_t *sz16, const size_t nStrLen16)
{
	size_t nLen16 = 0;

	if(ch8 != '\0')
		nLen16 = MultiByteToWideChar(CP_UTF8, 0, &ch8, 1, (PWSTR)sz16, (int)nStrLen16);

	return nLen16;
}

//#################################################################################################
size_t WideToUtf8(PCWSTR szW, const size_t nStrLenW, PSTR sz8, const size_t nStrLen8)
{
	size_t nLen8 = 0;

	if(nStrLenW)
		nLen8 = WideCharToMultiByte(CP_UTF8, 0, szW, (int)nStrLenW, sz8, (int)nStrLen8, nullptr, nullptr);

	return nLen8;
}

//#################################################################################################
size_t WideToUtf8(const wchar_t chW, PSTR sz8, const size_t nStrLen8)
{
	size_t nLen8 = 0;

	if(chW != L'\0')
		nLen8 = WideCharToMultiByte(CP_UTF8, 0, &chW, 1, sz8, (int)nStrLen8, nullptr, nullptr);

	return nLen8;
}

//#################################################################################################
size_t WideToUtf16(PCWSTR szW, const size_t nStrLenW, char16_t *sz16, const size_t nStrLen16)
{
	size_t nLen16 = 0;

	if(nStrLenW)
	{
		nLen16 = nStrLenW;
		size_t nLen = MIN(nStrLenW, nStrLen16);
		for(size_t n = 0; n < nLen; ++n)
			sz16[n] = szW[n];
	}

	return nLen16;
}

//#################################################################################################
size_t WideToUtf16(const wchar_t chW, char16_t *sz16, const size_t nStrLen16)
{
	size_t nLen16 = 0;

	if(chW != L'\0')
	{
		nLen16 = 1;
		if(nStrLen16)
			sz16[0] = chW;
	}

	return nLen16;
}

//#################################################################################################
size_t Utf16ToUtf8(const char16_t *sz16, const size_t nStrLen16, PSTR sz8, const size_t nStrLen8)
{
	size_t nLen8 = 0;

	if(nStrLen16)
		nLen8 = WideCharToMultiByte(CP_UTF8, 0, (PCWSTR)sz16, (int)nStrLen16, sz8, (int)nStrLen8, nullptr, nullptr);

	return nLen8;
}

//#################################################################################################
size_t Utf16ToUtf8(const char16_t ch16, PSTR sz8, const size_t nStrLen8)
{
	size_t nLen8 = 0;

	if(ch16 != u'\0')
		nLen8 = WideCharToMultiByte(CP_UTF8, 0, (PCWSTR)&ch16, 1, sz8, (int)nStrLen8, nullptr, nullptr);

	return nLen8;
}

//#################################################################################################
size_t Utf16ToWide(const char16_t *sz16, const size_t nStrLen16, PWSTR szW, const size_t nStrLenW)
{
	size_t nLenW = 0;

	if(nStrLen16)
	{
		nLenW = nStrLen16;
		size_t nLen = MIN(nStrLenW, nStrLen16);
		for(size_t n = 0; n < nLen; ++n)
			szW[n] = sz16[n];
	}

	return nLenW;
}

//#################################################################################################
size_t Utf16ToWide(const char16_t ch16, PWSTR szW, const size_t nStrLenW)
{
	size_t nLenW = 0;

	if(ch16 != u'\0')
	{
		nLenW = 1;
		if(nStrLenW)
			szW[0] = ch16;
	}

	return nLenW;
}

//#################################################################################################
void StringToLower(PSTR sz, const size_t nStrLen)
{	// It is not possible to lowercase UTF8 strings one byte at a time, so convert to wide characters for processing
	auto szW = std::make_unique<wchar_t[]>(nStrLen);
	size_t nStrLenW = Utf8ToWide(sz, nStrLen, szW.get(), nStrLen);
	StringToLower(szW.get(), nStrLenW);
	WideToUtf8(szW.get(), nStrLenW, sz, nStrLen);
}

//#################################################################################################
void StringToUpper(PSTR sz, const size_t nStrLen)
{	// It is not possible to uppercase UTF8 strings one byte at a time, so convert to wide characters for processing
	auto szW = std::make_unique<wchar_t[]>(nStrLen);
	size_t nStrLenW = Utf8ToWide(sz, nStrLen, szW.get(), nStrLen);
	StringToUpper(szW.get(), nStrLenW);
	WideToUtf8(szW.get(), nStrLenW, sz, nStrLen);
}

//#################################################################################################
void StringToLower(char16_t *sz, const size_t nStrLen)
{	// It is not possible to lowercase UTF16 strings one byte at a time, so convert to wide characters for processing
	auto szW = std::make_unique<wchar_t[]>(nStrLen);
	size_t nStrLenW = Utf16ToWide(sz, nStrLen, szW.get(), nStrLen);
	StringToLower(szW.get(), nStrLenW);
	WideToUtf16(szW.get(), nStrLenW, sz, nStrLen);
}

//#################################################################################################
void StringToUpper(char16_t *sz, const size_t nStrLen)
{	// It is not possible to uppercase UTF16 strings one byte at a time, so convert to wide characters for processing
	auto szW = std::make_unique<wchar_t[]>(nStrLen);
	size_t nStrLenW = Utf16ToWide(sz, nStrLen, szW.get(), nStrLen);
	StringToUpper(szW.get(), nStrLenW);
	WideToUtf16(szW.get(), nStrLenW, sz, nStrLen);
}

//#################################################################################################
void CharAdvance(PCSTR &sz)
{
	if(IS_MB_4BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)) && IS_MB_XBYTE(*(sz + 3)))
		sz += 4;
	else if(IS_MB_3BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)))
		sz += 3;
	else if(IS_MB_2BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)))
		sz += 2;
	else
		++sz;
}

//#################################################################################################
size_t CharAdvance(PCSTR &sz, const size_t nStrSize)
{
	size_t nAdvanced = 0;

	if(nStrSize)
	{
		if(nStrSize >= 4 && IS_MB_4BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)) && IS_MB_XBYTE(*(sz + 3)))
		{
			sz += 4;
			nAdvanced = 4;
		}
		else if(nStrSize >= 3 && IS_MB_3BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)))
		{
			sz += 3;
			nAdvanced = 3;
		}
		else if(nStrSize >= 2 && IS_MB_2BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)))
		{
			sz += 2;
			nAdvanced = 2;
		}
		else
		{
			++sz;
			nAdvanced = 1;
		}
	}

	return nAdvanced;
}

//#################################################################################################
uint8_t StringGetCharSize(PCSTR sz)
{	// Use with caution, this function assumes the string is at least 4 characters long and/or null terminated
	uint8_t nCharSize;

	if(IS_MB_4BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)) && IS_MB_XBYTE(*(sz + 3)))
		nCharSize = 4;
	else if(IS_MB_3BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)))
		nCharSize = 3;
	else if(IS_MB_2BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)))
		nCharSize = 2;
	else
		nCharSize = 1;

	return nCharSize;
}

//#################################################################################################
uint8_t StringGetCharSize(PCSTR sz, const size_t nStrSize)
{
	if(nStrSize == 0)
		return 0;

	uint8_t nCharSize;

	if(nStrSize >= 4 && IS_MB_4BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)) && IS_MB_XBYTE(*(sz + 3)))
		nCharSize = 4;
	else if(nStrSize >= 3 && IS_MB_3BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)) && IS_MB_XBYTE(*(sz + 2)))
		nCharSize = 3;
	else if(nStrSize >= 2 && IS_MB_2BYTE(*sz) && IS_MB_XBYTE(*(sz + 1)))
		nCharSize = 2;
	else
		nCharSize = 1;

	return nCharSize;
}

//#################################################################################################
uint8_t GetMultiByteCharSize(PCSTR szSrc, const size_t nSrcSize)
{
	if(szSrc == nullptr || nSrcSize == 0)
		return 0;

	uint8_t nCharSize;

	if(nSrcSize >= 4 && IS_MB_4BYTE(*szSrc) && IS_MB_XBYTE(*(szSrc + 1)) && IS_MB_XBYTE(*(szSrc + 2)) && IS_MB_XBYTE(*(szSrc + 3)))
		nCharSize = 4;
	else if(nSrcSize >= 3 && IS_MB_3BYTE(*szSrc) && IS_MB_XBYTE(*(szSrc + 1)) && IS_MB_XBYTE(*(szSrc + 2)))
		nCharSize = 3;
	else if(nSrcSize >= 2 && IS_MB_2BYTE(*szSrc) && IS_MB_XBYTE(*(szSrc + 1)))
		nCharSize = 2;
	else
		nCharSize = 1;

	return nCharSize;
}

//#################################################################################################
int StringCompare8(PCSTR sz1, const size_t nStrLen1, PCSTR sz2, const size_t nStrLen2, const bool bCaseInsensitive)
{
	int nCompareRtn = 0;

	// CompareString does not like nullptr pointers, so check for those first
	bool bStr1Null = (nStrLen1 == 0 || (sz1 == nullptr || *sz1 == g_chNull8));
	bool bStr2Null = (nStrLen2 == 0 || (sz2 == nullptr || *sz2 == g_chNull8));

	if(bStr1Null && !bStr2Null)
		nCompareRtn = -1;
	else if(bStr1Null && bStr2Null)
		nCompareRtn = 0;
	else if(!bStr1Null && bStr2Null)
		nCompareRtn = 1;
	else
	{
		if(bCaseInsensitive)
		{
			auto szW1 = std::make_unique<wchar_t[]>(nStrLen1);
			size_t nStrLenW1 = Utf8ToWide(sz1, nStrLen1, szW1.get(), nStrLen1);

			auto szW2 = std::make_unique<wchar_t[]>(nStrLen2);
			size_t nStrLenW2 = Utf8ToWide(sz2, nStrLen2, szW2.get(), nStrLen2);

			PWSTR szScan1 = szW1.get();
			PWSTR szScan2 = szW2.get();

			while((size_t)(szScan1 - szW1.get()) < nStrLenW1 && (size_t)(szScan2 - szW2.get()) < nStrLenW2)
			{
				wchar_t ch1 = CharToLower(*szScan1);
				wchar_t ch2 = CharToLower(*szScan2);

				if(ch1 < ch2)
				{
					nCompareRtn = -1;
					break;
				}
				else if(ch1 > ch2)
				{
					nCompareRtn = 1;
					break;
				}

				++szScan1;
				++szScan2;
			}

			if(nCompareRtn == 0 && (size_t)(szScan2 - szW2.get()) != nStrLenW2)
				nCompareRtn = -1;
			else if(nCompareRtn == 0 && (size_t)(szScan1 - szW1.get()) != nStrLenW1)
					nCompareRtn = 1;
		}
		else
		{
			PCSTR szScan1 = sz1;
			PCSTR szScan2 = sz2;

			while((size_t)(szScan1 - sz1) < nStrLen1 && (size_t)(szScan2 - sz2) < nStrLen2)
			{
				if(*szScan1 < *szScan2)
				{
					nCompareRtn = -1;
					break;
				}
				else if(*szScan1 > *szScan2)
				{
					nCompareRtn = 1;
					break;
				}

				++szScan1;
				++szScan2;
			}

			if(nCompareRtn == 0 && (size_t)(szScan2 - sz2) != nStrLen2)
				nCompareRtn = -1;
			else if(nCompareRtn == 0 && (size_t)(szScan1 - sz1) != nStrLen1)
				nCompareRtn = 1;
		}
	}

	return nCompareRtn;
}

//#################################################################################################
int StringCompareW(PCWSTR sz1, const size_t nStrLen1, PCWSTR sz2, const size_t nStrLen2, const bool bCaseInsensitive)
{
	int nCompareRtn = 0;

	// CompareString does not like nullptr pointers, so check for those first
	bool bStr1Null = (nStrLen1 == 0 || (sz1 == nullptr || *sz1 == g_chNullW));
	bool bStr2Null = (nStrLen2 == 0 || (sz2 == nullptr || *sz2 == g_chNullW));

	if(bStr1Null && !bStr2Null)
		nCompareRtn = -1;
	else if(bStr1Null && bStr2Null)
		nCompareRtn = 0;
	else if(!bStr1Null && bStr2Null)
		nCompareRtn = 1;
	else
	{
		if(bCaseInsensitive)
		{
			PCWSTR szScan1 = sz1;
			PCWSTR szScan2 = sz2;

			while((size_t)(szScan1 - sz1) < nStrLen1 && (size_t)(szScan2 - sz2) < nStrLen2)
			{
				wchar_t ch1 = CharToLower(*szScan1);
				wchar_t ch2 = CharToLower(*szScan2);

				if(ch1 < ch2)
				{
					nCompareRtn = -1;
					break;
				}
				else if(ch1 > ch2)
				{
					nCompareRtn = 1;
					break;
				}

				++szScan1;
				++szScan2;
			}

			if(nCompareRtn == 0 && (size_t)(szScan2 - sz2) != nStrLen2)
				nCompareRtn = -1;
			else if(nCompareRtn == 0 && (size_t)(szScan1 - sz1) != nStrLen1)
				nCompareRtn = 1;
		}
		else
		{
			PCWSTR szScan1 = sz1;
			PCWSTR szScan2 = sz2;

			while((size_t)(szScan1 - sz1) < nStrLen1 && (size_t)(szScan2 - sz2) < nStrLen2)
			{
				if(*szScan1 < *szScan2)
				{
					nCompareRtn = -1;
					break;
				}
				else if(*szScan1 > *szScan2)
				{
					nCompareRtn = 1;
					break;
				}

				++szScan1;
				++szScan2;
			}

			if(nCompareRtn == 0 && (size_t)(szScan2 - sz2) != nStrLen2)
				nCompareRtn = -1;
			else if(nCompareRtn == 0 && (size_t)(szScan1 - sz1) != nStrLen1)
				nCompareRtn = 1;
		}
	}

	return nCompareRtn;
}

//#################################################################################################
int StringCompare16(const char16_t *sz1, const size_t nStrLen1, const char16_t *sz2, const size_t nStrLen2, const bool bCaseInsensitive)
{
	int nCompareRtn = 0;

	// CompareString does not like nullptr pointers, so check for those first
	bool bStr1Null = (nStrLen1 == 0 || (sz1 == nullptr || *sz1 == g_chNull16));
	bool bStr2Null = (nStrLen2 == 0 || (sz2 == nullptr || *sz2 == g_chNull16));

	if(bStr1Null && !bStr2Null)
		nCompareRtn = -1;
	else if(bStr1Null && bStr2Null)
		nCompareRtn = 0;
	else if(!bStr1Null && bStr2Null)
		nCompareRtn = 1;
	else
	{
		if(bCaseInsensitive)
		{
			auto szW1 = std::make_unique<wchar_t[]>(nStrLen1);
			size_t nStrLenW1 = Utf16ToWide(sz1, nStrLen1, szW1.get(), nStrLen1);

			auto szW2 = std::make_unique<wchar_t[]>(nStrLen2);
			size_t nStrLenW2 = Utf16ToWide(sz2, nStrLen2, szW2.get(), nStrLen2);

			PWSTR szScan1 = szW1.get();
			PWSTR szScan2 = szW2.get();

			while((size_t)(szScan1 - szW1.get()) < nStrLenW1 && (size_t)(szScan2 - szW2.get()) < nStrLenW2)
			{
				wchar_t ch1 = CharToLower(*szScan1);
				wchar_t ch2 = CharToLower(*szScan2);

				if(ch1 < ch2)
				{
					nCompareRtn = -1;
					break;
				}
				else if(ch1 > ch2)
				{
					nCompareRtn = 1;
					break;
				}

				++szScan1;
				++szScan2;
			}

			if(nCompareRtn == 0 && (size_t)(szScan2 - szW2.get()) != nStrLenW2)
				nCompareRtn = -1;
			else if(nCompareRtn == 0 && (size_t)(szScan1 - szW1.get()) != nStrLenW1)
				nCompareRtn = 1;
		}
		else
		{
			const char16_t *szScan1 = sz1;
			const char16_t *szScan2 = sz2;

			while((size_t)(szScan1 - sz1) < nStrLen1 && (size_t)(szScan2 - sz2) < nStrLen2)
			{
				if(*szScan1 < *szScan2)
				{
					nCompareRtn = -1;
					break;
				}
				else if(*szScan1 > *szScan2)
				{
					nCompareRtn = 1;
					break;
				}

				++szScan1;
				++szScan2;
			}

			if(nCompareRtn == 0 && (size_t)(szScan2 - sz2) != nStrLen2)
				nCompareRtn = -1;
			else if(nCompareRtn == 0 && (size_t)(szScan1 - sz1) != nStrLen1)
				nCompareRtn = 1;
		}
	}

	return nCompareRtn;
}

//#################################################################################################
bool IsAlphaString8(PCSTR sz)
{
	bool bAlphaStr = true;

	if(sz == nullptr || *sz == g_chNull8)
		return false;

	PCSTR szScan = sz;
	while(*szScan != g_chNull8)
	{
		if(*szScan < 'A' || (*szScan > 'Z' && *szScan < 'a') || *szScan > 'z')
		{
			bAlphaStr = false;
			break;
		}
		++szScan;
	}

	return bAlphaStr;
}

//#################################################################################################
bool IsAlphaStringW(PCWSTR sz)
{
	bool bAlphaStr = true;

	if(sz == nullptr || *sz == g_chNullW)
		return false;

	PCWSTR szScan = sz;
	while(*szScan != g_chNullW)
	{
		if(*szScan < L'A' || (*szScan > L'Z' && *szScan < L'a') || *szScan > L'z')
		{
			bAlphaStr = false;
			break;
		}
		++szScan;
	}

	return bAlphaStr;
}

//#################################################################################################
bool IsDigitString8(PCSTR sz)
{
	bool bDigitStr = true;

	if(sz == nullptr || *sz == g_chNull8)
		return false;

	PCSTR szScan = sz;
	while(*szScan != g_chNull8)
	{
		if(*szScan < '0' || *szScan > '9')
		{
			bDigitStr = false;
			break;
		}
		++szScan;
	}

	return bDigitStr;
}

//#################################################################################################
bool IsDigitStringW(PCWSTR sz)
{
	bool bDigitStr = true;

	if(sz == nullptr || *sz == g_chNullW)
		return false;

	PCWSTR szScan = sz;
	while(*szScan != g_chNullW)
	{
		if(*szScan < L'0' || *szScan > L'9')
		{
			bDigitStr = false;
			break;
		}
		++szScan;
	}

	return bDigitStr;
}

//#################################################################################################
bool IsNumberString8(PCSTR sz)
{	// Similar to IsDigitString except it allows for negative numbers
	bool bNumberStr = true;

	if(sz == nullptr || *sz == g_chNull8)
		return false;

	PCSTR szScan = sz;
	if(*szScan == '-')
	{
		++szScan;
		if(*szScan == g_chNull8)
			return false;
	}

	while(*szScan != g_chNull8)
	{
		if(*szScan < '0' || *szScan > '9')
		{
			if(*szScan != '-' || sz != szScan)
			{
				bNumberStr = false;
				break;
			}
		}
		++szScan;
	}

	return bNumberStr;
}

//#################################################################################################
bool IsNumberStringW(PCWSTR sz)
{	// Similar to IsDigitString except it allows for negative numbers
	bool bNumberStr = true;

	if(sz == nullptr || *sz == g_chNullW)
		return false;

	PCWSTR szScan = sz;
	if(*szScan == L'-')
	{
		++szScan;
		if(*szScan == g_chNullW)
			return false;
	}

	while(*szScan != g_chNullW)
	{
		if(*szScan < L'0' || *szScan > L'9')
		{
			if(*szScan != L'-' || sz != szScan)
			{
				bNumberStr = false;
				break;
			}
		}
		++szScan;
	}

	return bNumberStr;
}

//#################################################################################################
bool IsAlphaDigitString8(PCSTR sz)
{
	bool bAlphaDigitStr = true;

	if(sz == nullptr || *sz == g_chNull8)
		return false;

	PCSTR szScan = sz;
	while(*szScan != g_chNull8)
	{
		if(*szScan < '0' || (*szScan > '9' && *szScan < 'A') || (*szScan > 'Z' && *szScan < 'a') || *szScan > 'z')
		{
			bAlphaDigitStr = false;
			break;
		}
		++szScan;
	}

	return bAlphaDigitStr;
}

//#################################################################################################
bool IsAlphaDigitStringW(PCWSTR sz)
{
	bool bAlphaDigitStr = true;

	if(sz == nullptr || *sz == g_chNullW)
		return false;

	PCWSTR szScan = sz;
	while(*szScan != g_chNullW)
	{
		if(*szScan < L'0' || (*szScan > L'9' && *szScan < L'A') || (*szScan > L'Z' && *szScan < L'a') || *szScan > L'z')
		{
			bAlphaDigitStr = false;
			break;
		}
		++szScan;
	}

	return bAlphaDigitStr;
}

//#################################################################################################
bool IsHexString8(PCSTR sz)
{
	bool bHexStr = true;

	if(sz == nullptr || *sz == g_chNull8)
		return false;

	PCSTR szScan = sz;
	while(*szScan != g_chNull8)
	{
		if(*szScan < '0' || (*szScan > '9' && *szScan < 'A') || (*szScan > 'F' && *szScan < 'a') || *szScan > 'f')
		{
			bHexStr = false;
			break;
		}
		++szScan;
	}

	return bHexStr;
}

//#################################################################################################
bool IsHexStringW(PCWSTR sz)
{
	bool bHexStr = true;

	if(sz == nullptr || *sz == g_chNullW)
		return false;

	PCWSTR szScan = sz;
	while(*szScan != g_chNullW)
	{
		if(*szScan < L'0' || (*szScan > L'9' && *szScan < L'A') || (*szScan > L'F' && *szScan < L'a') || *szScan > L'f')
		{
			bHexStr = false;
			break;
		}
		++szScan;
	}

	return bHexStr;
}
