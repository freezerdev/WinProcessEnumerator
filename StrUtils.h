#pragma once

#include "Common.h"
#include "Str.h"

//#################################################################################################
// Converts between char/UTF8, wchar_t/UTF16, and char16_t/UTF16. Input can be a string or character, output is always a string.
// Returns the number of characters converted (or required if the output buffer is not large enough).  The output buffer is not guaranteed to be null-terminated.
size_t Utf8ToWide(PCSTR sz8, const size_t nStrLen8, PWSTR szW, const size_t nStrLenW);
size_t Utf8ToWide(const char ch8, PWSTR szW, const size_t nStrLenW);

size_t Utf8ToUtf16(PCSTR sz8, const size_t nStrLen8, char16_t *sz16, const size_t nStrLen16);
size_t Utf8ToUtf16(const char ch8, char16_t *sz16, const size_t nStrLen16);

size_t WideToUtf8(PCWSTR szW, const size_t nStrLenW, PSTR sz8, const size_t nStrLen8);
size_t WideToUtf8(const wchar_t chW, PSTR sz8, const size_t nStrLen8);

size_t WideToUtf16(PCWSTR szW, const size_t nStrLenW, char16_t *sz16, const size_t nStrLen16);
size_t WideToUtf16(const wchar_t chW, char16_t *sz16, const size_t nStrLen16);

size_t Utf16ToUtf8(const char16_t *sz16, const size_t nStrLen16, PSTR sz8, const size_t nStrLen8);
size_t Utf16ToUtf8(const char16_t ch16, PSTR sz8, const size_t nStrLen8);

size_t Utf16ToWide(const char16_t *sz16, const size_t nStrLen16, PWSTR szW, const size_t nStrLenW);
size_t Utf16ToWide(const char16_t ch16, PWSTR szW, const size_t nStrLenW);

//#################################################################################################
// Converts a string or character to lowercase or uppercase.  For strings the conversion is done in place.
void StringToLower(PSTR sz, const size_t nStrLen);
void StringToUpper(PSTR sz, const size_t nStrLen);

//#################################################################################################
inline char CharToLower(const char ch)
{	// This function is not UTF8 compliant, only use for ANSI characters
	char chResult = ch;
	LCMapStringA(LOCALE_SYSTEM_DEFAULT, LCMAP_LOWERCASE, &ch, 1, &chResult, 1);
	return chResult;
}

//#################################################################################################
inline char CharToUpper(const char ch)
{	// This function is not UTF8 compliant, only use for ANSI characters
	char chResult = ch;
	LCMapStringA(LOCALE_SYSTEM_DEFAULT, LCMAP_UPPERCASE, &ch, 1, &chResult, 1);
	return chResult;
}

void StringToLower(char16_t *sz, const size_t nStrLen);
void StringToUpper(char16_t *sz, const size_t nStrLen);

//#################################################################################################
inline void StringToLower(PWSTR sz, const size_t nStrLen)
{
#ifdef _DEBUG
	int nResult = LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, LCMAP_LOWERCASE, sz, (int)nStrLen, sz, (int)nStrLen, nullptr, nullptr, 0);
#else
	LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, LCMAP_LOWERCASE, sz, (int)nStrLen, sz, (int)nStrLen, nullptr, nullptr, 0);
#endif
}

//#################################################################################################
inline void StringToUpper(PWSTR sz, const size_t nStrLen)
{
#ifdef _DEBUG
	int nResult = LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, LCMAP_UPPERCASE, sz, (int)nStrLen, sz, (int)nStrLen, nullptr, nullptr, 0);
#else
	LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, LCMAP_UPPERCASE, sz, (int)nStrLen, sz, (int)nStrLen, nullptr, nullptr, 0);
#endif
}

//#################################################################################################
inline wchar_t CharToLower(const wchar_t ch)
{
	wchar_t chResult = ch;
	LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, LCMAP_LOWERCASE, &ch, 1, &chResult, 1, nullptr, nullptr, 0);
	return chResult;
}

//#################################################################################################
inline wchar_t CharToUpper(const wchar_t ch)
{
	wchar_t chResult = ch;
	LCMapStringEx(LOCALE_NAME_SYSTEM_DEFAULT, LCMAP_UPPERCASE, &ch, 1, &chResult, 1, nullptr, nullptr, 0);
	return chResult;
}

uint8_t GetMultiByteCharSize(PCSTR szSrc, const size_t nSrcSize);

//#################################################################################################
template<typename CHARTYPE>
size_t StringGetLength(const CHARTYPE *sz)
{
	size_t nLen = 0;

	static constexpr CHARTYPE chNull = 0;

	if(sz)
	{
		while(*sz++ != chNull)
			++nLen;
	}

	return nLen;
}

//#################################################################################################
template<typename CHARTYPE>
size_t StringGetLength(const CHARTYPE *sz, const size_t nMaxLen)
{
	size_t nLen = 0;

	static constexpr CHARTYPE chNull = 0;

	if(sz)
	{
		size_t nRemainingLen = nMaxLen;
		while(nRemainingLen-- && *sz++ != chNull)
			++nLen;
	}

	return nLen;
}

int StringCompare8(PCSTR sz1, const size_t nStrLen1, PCSTR sz2, const size_t nStrLen2, const bool bCaseInsensitive = false);
int StringCompareW(PCWSTR sz1, const size_t nStrLen1, PCWSTR sz2, const size_t nStrLen2, const bool bCaseInsensitive = false);
int StringCompare16(const char16_t *sz1, const size_t nStrLen1, const char16_t *sz2, const size_t nStrLen2, const bool bCaseInsensitive = false);

// isspace/iswspace take an int which can result in errors if the character is negative
inline bool IsSpace8(const char ch) {return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v');}
inline bool IsSpaceW(const wchar_t ch) {return (ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r' || ch == L'\f' || ch == L'\v');}
inline bool IsSpace16(const char16_t ch) {return (ch == u' ' || ch == u'\t' || ch == u'\n' || ch == u'\r' || ch == u'\f' || ch == u'\v');}

inline bool IsAlphaChar8(const char ch) {return (ch >= 'A' && (ch <= 'Z' || ch >= 'a') && ch <= 'z');}
inline bool IsAlphaCharW(const wchar_t ch) {return (ch >= L'A' && (ch <= L'Z' || ch >= L'a') && ch <= L'z');}
inline bool IsDigitChar8(const char ch) {return (ch >= '0' && ch <= '9');}
inline bool IsDigitCharW(const wchar_t ch) {return (ch >= L'0' && ch <= L'9');}
inline bool IsAlphaDigitChar8(const char ch) {return (ch >= '0' && (ch <= '9' || ch >= 'A') && (ch <= 'Z' || ch >= 'a') && ch <= 'z');}
inline bool IsAlphaDigitCharW(const wchar_t ch) {return (ch >= L'0' && (ch <= L'9' || ch >= L'A') && (ch <= L'Z' || ch >= L'a') && ch <= L'z');}
inline bool IsHexChar8(const char ch) {return (ch >= '0' && (ch <= '9' || ch >= 'A') && (ch <= 'F' || ch >= 'a') && ch <= 'f');}
inline bool IsHexCharW(const wchar_t ch) {return (ch >= L'0' && (ch <= L'9' || ch >= L'A') && (ch <= L'F' || ch >= L'a') && ch <= L'f');}

bool IsAlphaString8(PCSTR sz);
bool IsAlphaStringW(PCWSTR sz);
bool IsDigitString8(PCSTR sz);
bool IsDigitStringW(PCWSTR sz);
bool IsNumberString8(PCSTR sz);
bool IsNumberStringW(PCWSTR sz);
bool IsAlphaDigitString8(PCSTR sz);
bool IsAlphaDigitStringW(PCWSTR sz);
bool IsHexString8(PCSTR sz);
bool IsHexStringW(PCWSTR sz);

//#################################################################################################
template<typename INTTYPE>
CStr8 IntegerToString8(const INTTYPE i, const uint8_t nBase = 10)
{
	static PCSTR szDigits = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char sz[66];	// An int64 in base 2 is 64 characters long, plus 2 extra for negative sign and null term
	PSTR szScan = (PSTR)sz + 65;
	*szScan = g_chNull8;

	INTTYPE I = i;
	do{
		int8_t lsd = (int8_t)(I % nBase);
		I /= nBase;
		--szScan;
		*szScan = szDigits[lsd + 35];	// 35 is the midpoint of the string above
	}while(I != 0);
	if(i < 0)
		*(--szScan) = '-';

	return CStr8(szScan);
}

//#################################################################################################
template<typename INTTYPE>
CStrW IntegerToStringW(const INTTYPE i, const uint8_t nBase = 10)
{
	static PCWSTR szDigits = L"ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	wchar_t sz[66];	// An int64 in base 2 is 64 characters long, plus 2 extra for negative sign and null term
	PWSTR szScan = (PWSTR)sz + 65;
	*szScan = g_chNullW;

	INTTYPE I = i;
	do{
		int8_t lsd = (int8_t)(I % nBase);
		I /= nBase;
		--szScan;
		*szScan = szDigits[lsd + 35];	// 35 is the midpoint of the string above
	}while(I != 0);
	if(i < 0)
		*(--szScan) = L'-';

	return CStrW(szScan);
}

//#################################################################################################
template<typename INTTYPE>
INTTYPE StringToInteger8(PCSTR sz, const uint8_t nBase = 10)
{
	INTTYPE i = 0;
	int8_t n;
	PCSTR szScan = sz;

	bool bNegative = false;
	if(*szScan == '-')
	{
		bNegative = true;
		++szScan;
	}

	while(*szScan != g_chNull8)
	{
		if(*szScan >= '0' && *szScan <= '9')
			n = (int8_t)(*szScan - '0');
		else if(*szScan >= 'A' && *szScan <= 'Z')
			n = (int8_t)(*szScan - '7');
		else if(*szScan >= 'a' && *szScan <= 'z')
			n = (int8_t)(*szScan - 'W');
		else
			break;

		i = i * nBase + n;
		++szScan;
	}

	if(bNegative)
		i *= -1;

	return i;
}

//#################################################################################################
template<typename INTTYPE>
INTTYPE StringToIntegerW(PCWSTR sz, const uint8_t nBase = 10)
{
	INTTYPE i = 0;
	int8_t n;
	PCWSTR szScan = sz;

	bool bNegative = false;
	if(*szScan == '-')
	{
		bNegative = true;
		++szScan;
	}

	while(*szScan != g_chNullW)
	{
		if(*szScan >= L'0' && *szScan <= L'9')
			n = (int8_t)(*szScan - L'0');
		else if(*szScan >= L'A' && *szScan <= L'Z')
			n = (int8_t)(*szScan - L'7');
		else if(*szScan >= L'a' && *szScan <= L'z')
			n = (int8_t)(*szScan - L'W');
		else
			break;

		i = i * nBase + n;
		++szScan;
	}

	if(bNegative)
		i *= -1;

	return i;
}

//#################################################################################################
template<typename FLOATTYPE>
CStr8 FloatToString8(const FLOATTYPE f, const size_t nPrecision = 6)
{
	CStr8 str;

	std::ostringstream stream;
	stream.precision(nPrecision);
	stream << std::fixed << f;
	str = stream.str();
	str.TrimRight('0');
	if(str.GetLast() == '.')
		str.Append('0');

	return str;
}

//#################################################################################################
template<typename FLOATTYPE>
CStrW FloatToStringW(const FLOATTYPE f, const size_t nPrecision = 6)
{
	CStrW str;

	std::wostringstream stream;
	stream.precision(nPrecision);
	stream << std::fixed << f;
	str = stream.str();
	str.TrimRight(L'0');
	if(str.GetLast() == L'.')
		str.Append(L'0');

	return str;
}

//#################################################################################################
template<typename FLOATTYPE>
FLOATTYPE StringToFloat8(PCSTR sz)
{
	FLOATTYPE f = 0.0;

	try
	{
		if(sizeof(FLOATTYPE) == sizeof(float))
			f = (FLOATTYPE)std::stof(sz, nullptr);
		else if(sizeof(FLOATTYPE) == sizeof(double))
			f = (FLOATTYPE)std::stod(sz, nullptr);
		else
			f = (FLOATTYPE)std::stold(sz, nullptr);
	}
	catch(const std::exception &e)
	{
		UNUSED(e);
	}

	return f;
}

//#################################################################################################
template<typename FLOATTYPE>
FLOATTYPE StringToFloatW(PCWSTR sz)
{
	FLOATTYPE f = 0.0;

	try
	{
		if(sizeof(FLOATTYPE) == sizeof(float))
			f = (FLOATTYPE)std::stof(sz, nullptr);
		else if(sizeof(FLOATTYPE) == sizeof(double))
			f = (FLOATTYPE)std::stod(sz, nullptr);
		else
			f = (FLOATTYPE)std::stold(sz, nullptr);
	}
	catch(const std::exception &e)
	{
		UNUSED(e);
	}

	return f;
}

// Floating point comparison functions; tests like (f1 == f2) do not always yield the expected results
template<typename FLOATTYPE>
inline bool FloatIsEqual(const FLOATTYPE f1, const FLOATTYPE f2, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f1 == f2
	return std::fabs(f1 - f2) <= std::fmax(std::fabs(f1), std::fabs(f2)) * fEpsilon;
}

template<typename FLOATTYPE>
inline bool FloatIsGreaterEqual(const FLOATTYPE f1, const FLOATTYPE f2, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f1 >= f2
	return (f1 > f2) || FloatIsEqual(f1, f2, fEpsilon);
}

template<typename FLOATTYPE>
inline bool FloatIsLessEqual(const FLOATTYPE f1, const FLOATTYPE f2, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f1 <= f2
	return (f1 < f2) || FloatIsEqual(f1, f2, fEpsilon);
}

template<typename FLOATTYPE>
inline bool FloatIsZero(const FLOATTYPE f, const FLOATTYPE fEpsilon = std::numeric_limits<FLOATTYPE>::epsilon())
{	// Equivalent to f == 0.0F
	return (std::fabs(f) <= fEpsilon);
}


#define StringCompare StringCompareW
#define IsSpace IsSpaceW
#define IsAlphaChar IsAlphaCharW
#define IsDigitChar IsDigitCharW
#define IsAlphaDigitChar IsAlphaDigitCharW
#define IsHexChar IsHexCharW
#define IsAlphaString IsAlphaStringW
#define IsDigitString IsDigitStringW
#define IsNumberString IsNumberStringW
#define IsAlphaDigitString IsAlphaDigitStringW
#define IsHexString IsHexStringW
#define IntegerToString IntegerToStringW
#define StringToInteger StringToIntegerW
#define FloatToString FloatToStringW
#define StringToFloat StringToFloatW
