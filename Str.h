#pragma once

#include "Common.h"
#include <sstream>

// Windows headers interfere with these definitions
#undef LoadString

// Forward declarations
class CStr8;
class CStrW;
class CStr16;

//#################################################################################################
class CStr8 final
{
private:
	friend class CStrW;
	friend class CStr16;

public:
	enum EPrintfType : uint8_t
	{
		EPT_Printf = 0
	};

	enum EFormatType : uint8_t
	{
		EFT_Format = 0
	};

	static const size_t not_found;
	static const size_t end;

	// Constructs an empty string object
	CStr8(void);
	// Copy and move constructors
	CStr8(const CStr8 &src);
	CStr8(CStr8 &&src) noexcept;
	// Constructs a string object set to the string/character
	explicit CStr8(const CStrW &str);
	explicit CStr8(const CStr16 &str);
	explicit CStr8(const std::string &str);
	explicit CStr8(const std::wstring &str);
	explicit CStr8(PCSTR sz);
	explicit CStr8(PCWSTR sz);
	explicit CStr8(const char ch);
	explicit CStr8(const wchar_t ch);
	// Constructs a string object set to a substring, given the starting location and length
	CStr8(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	CStr8(PCSTR sz, const size_t nStart, const size_t nLen = end);
	// Constructs a formatted string using printf formatting; set 'eIgnored' to 'EPT_Printf' and 'szFormat' contains the string with format specifiers
	CStr8(const EPrintfType eIgnored, PCSTR szFormat, ...);
	// Constructs a formatted string using C++ variadic templates; set 'eIgnored' to 'EFT_Format' and 'szFormat' contains the string with format specifiers
	template<typename ...ARGS>
	CStr8(const EFormatType eIgnored, PCSTR szFormat, const ARGS&... args)
		: m_sz((PSTR)g_szNull8),
		m_nBufLen(0),
		m_nStrLen(0)
	{
		UNUSED(eIgnored);

		*this = VFormat(szFormat, args...);
	}

	~CStr8(void);

	// Attaches to an existing null-terminated string buffer; buffer should have been allocated using 'malloc'
	CStr8 &Attach(PSTR sz, const size_t nBufLen);
	// Releases the internal string buffer, the caller is responsible for freeing the buffer with 'free'
	PSTR Detach(void) noexcept;

	// Overwrites the memory with all zeros to securely erase the string
	void SecureErase(void);

	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the count of characters (which may be different from length in UTF8 strings)
	size_t GetCount(const bool bIncludeNullTerm = false) const;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;

	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;
	// Empties the string, optionally freeing the memory
	void Empty(const bool bFree = false);
	// Returns the first character in the string, or '\0' if the string is empty
	char GetFirst(void) const;
	// Returns the last character in the string, or '\0' if the string is empty
	char GetLast(void) const;
	// Returns the character at the given index, or '\0' if the index is beyond the string
	char GetAt(const size_t nIndex) const;
	// Sets the character at the given index, so long as index is not beyond the string
	void SetAt(const size_t nIndex, const char ch);
	// Returns a pointer to the raw string data
	operator PCSTR(void) const noexcept;

	// Returns a UTF8 copy of the string
	CStr8 AsUtf8(void) const;
	// Returns a wchar_t copy of the string
	CStrW AsWide(void) const;
	CStrW AsNative(void) const;

	// Copy and move assignment operators
	CStr8 &operator=(const CStr8 &src);
	CStr8 &operator=(CStr8 &&src) noexcept;
	// Assigns the string object to the string/character
	CStr8 &operator=(const CStrW &str);
	CStr8 &operator=(const CStr16 &str);
	CStr8 &operator=(const std::string &str);
	CStr8 &operator=(const std::wstring &str);
	CStr8 &operator=(PCSTR sz);
	CStr8 &operator=(PCWSTR sz);
	CStr8 &operator=(const char ch);
	CStr8 &operator=(const wchar_t ch);

	// Assigns the string object to the string/character
	void Assign(const CStr8 &str);
	void Assign(const CStrW &str);
	void Assign(const CStr16 &str);
	void Assign(const std::string &str);
	void Assign(const std::wstring &str);
	void Assign(PCSTR sz);
	void Assign(PCWSTR sz);
	void Assign(const char ch);
	void Assign(const wchar_t ch);
	// Assigns the string object to a substring, given the starting location and length
	void Assign(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	void Assign(const CStrW &str, const size_t nStart, const size_t nLen = end);
	void Assign(PCSTR sz, const size_t nStart, const size_t nLen = end);
	void Assign(PCWSTR sz, const size_t nStart, const size_t nLen = end);

	// Appends the string/character
	CStr8 &operator+=(const CStr8 &str);
	CStr8 &operator+=(const CStrW &str);
	CStr8 &operator+=(PCSTR sz);
	CStr8 &operator+=(PCWSTR sz);
	CStr8 &operator+=(const char ch);
	CStr8 &operator+=(const wchar_t ch);

	// Prepends the string/character
	void Prepend(const CStr8 &str);
	void Prepend(const CStrW &str);
	void Prepend(PCSTR sz);
	void Prepend(PCWSTR sz);
	void Prepend(const char ch);
	void Prepend(const wchar_t ch);
	// Prepends a substring, given the starting location and length
	void Prepend(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	void Prepend(const CStrW &str, const size_t nStart, const size_t nLen = end);
	void Prepend(PCSTR sz, const size_t nStart, const size_t nLen = end);
	void Prepend(PCWSTR sz, const size_t nStart, const size_t nLen = end);

	// Appends the string/character
	void Append(const CStr8 &str);
	void Append(const CStrW &str);
	void Append(PCSTR sz);
	void Append(PCWSTR sz);
	void Append(const char ch);
	void Append(const wchar_t ch);
	// Appends a substring, given the starting location and length
	void Append(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	void Append(const CStrW &str, const size_t nStart, const size_t nLen = end);
	void Append(PCSTR sz, const size_t nStart, const size_t nLen = end);
	void Append(PCWSTR sz, const size_t nStart, const size_t nLen = end);

	friend std::ostream &operator<<(std::ostream &stream, const CStr8 &str);

	// Tests for equal (case sensitive)
	bool operator==(const CStr8 &str) const;
	bool operator==(PCSTR sz) const;
	bool operator==(const char ch) const;

	// Tests for not equal (case sensitive)
	bool operator!=(const CStr8 &str) const;
	bool operator!=(PCSTR sz) const;
	bool operator!=(const char ch) const;

	// Tests for less than (case sensitive)
	bool operator<(const CStr8 &str) const;
	bool operator<(PCSTR sz) const;
	bool operator<(const char ch) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CStr8 &str) const;
	bool operator<=(PCSTR sz) const;
	bool operator<=(const char ch) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CStr8 &str) const;
	bool operator>(PCSTR sz) const;
	bool operator>(const char ch) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CStr8 &str) const;
	bool operator>=(PCSTR sz) const;
	bool operator>=(const char ch) const;

	// Compares the string object against the string/character, optionally case insensitive
	int Compare(const CStr8 &str, const bool bCaseInsensitive = false) const;
	int Compare(PCSTR sz, const bool bCaseInsensitive = false) const;
	int Compare(const char ch, const bool bCaseInsensitive = false) const;

	// Compares strings with embedded numbers, optionally case insensitive
	int CompareAlphaNum(const CStr8 &str, const bool bCaseInsensitive = false) const;

	// Compares a substring of the string object against the entire string/character, given the starting location and length, optionally case insensitive
	int CompareMid(const CStr8 &str, const size_t nStart, const size_t nCount = end, const bool bCaseInsensitive = false) const;
	int CompareMid(PCSTR sz, const size_t nStart, const size_t nCount = end, const bool bCaseInsensitive = false) const;
	int CompareMid(const char ch, const size_t nStart, const bool bCaseInsensitive = false) const;
	// Compares the leftmost characters of the string object against the string/character, up to 'nCount' characters, optionally case insensitive
	int CompareLeft(const CStr8 &str, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareLeft(PCSTR sz, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareLeft(const char ch, const bool bCaseInsensitive = false) const;
	// Compares the rightmost characters of the string object against the string/character, up to 'nCount' characters, optionally case insensitive
	int CompareRight(const CStr8 &str, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareRight(PCSTR sz, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareRight(const char ch, const bool bCaseInsensitive = false) const;

	// Returns the count of instances of a given character in the string object, optionally case insensitive
	size_t Count(const char chFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the count of characters that the string object and provided string match starting at the left, optionally case insensitive
	size_t CountCompare(const CStr8 &str, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t CountCompare(PCSTR sz, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the count of characters that the string object and provided string match starting at the right, optionally case insensitive
	size_t ReverseCountCompare(const CStr8 &str, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t ReverseCountCompare(PCSTR sz, const size_t nStart = end, const bool bCaseInsensitive = false) const;

	// Tests if the string/character is in the string object given the starting index, optionally case insensitive
	bool Contains(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	bool Contains(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	bool Contains(const char chFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Tests for any instance of any character in the provided string given the starting index, optionally case insensitive
	bool ContainsOneOf(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	bool ContainsOneOf(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the first instance of the string/character in the string object given the starting index, optionally case insensitive; returns 'not_found' if the string/character is not in the string object
	size_t Find(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t Find(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t Find(const char chFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the last instance of the string/character in the string object given the starting index and working backwards, optionally case insensitive; returns 'not_found' if the string/character is not in the string object
	size_t ReverseFind(const CStr8 &strFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t ReverseFind(PCSTR szFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t ReverseFind(const char chFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	// Returns the index of the first instance of any character in the provided string given the starting index, optionally case insensitive; returns 'not_found' if none of the characters in the string are in the string object
	size_t FindFirstOf(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t FindFirstOf(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the first instance of any character not in the provided string given the starting index, optionally case insensitive; returns 'not_found' if all of the characters in the string object are in the provided string
	size_t FindFirstNotOf(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t FindFirstNotOf(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the last instance of any character in the provided string given the starting index and working backwards, optionally case insensitive; returns 'not_found' if none of the characters in the string are in the string object
	size_t FindLastOf(const CStr8 &strFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t FindLastOf(PCSTR szFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	// Returns the index of the last instance of any character not in the provided string given the starting index and working backwards, optionally case insensitive; returns 'not_found' if all of the characters in the string object are in the provided string
	size_t FindLastNotOf(const CStr8 &strFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t FindLastNotOf(PCSTR szFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;

	// Returns a substring of the string object starting at the given index and going until the first character not found in the provided string, optionally case insensitive
	CStr8 SpanIncluding(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	CStr8 SpanIncluding(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns a substring of the string object starting at the given index and going until the first character found in the provided string, optionally case insensitive
	CStr8 SpanExcluding(const CStr8 &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	CStr8 SpanExcluding(PCSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;

	// Inserts the string/character at the given index, if the index is beyond the string the string/character is appended; returns the new string length
	size_t Insert(const size_t nIndex, const CStr8 &strInsert);
	size_t Insert(const size_t nIndex, PCSTR szInsert);
	size_t Insert(const size_t nIndex, const char chInsert);
	// Removes 'nCount' characters from the string object starting at the given index; returns the new string length
	size_t Delete(const size_t nIndex, const size_t nCount = 1);
	// Removes all instances of the string/character from the string object, optionally case insensitive; returns the number removed
	size_t Remove(const CStr8 &strRemove, const bool bCaseInsensitive = false);
	size_t Remove(PCSTR szRemove, const bool bCaseInsensitive = false);
	size_t Remove(const char chRemove, const bool bCaseInsensitive = false);
	// Removes any and all characters in the string from the string object, optionally case insensitive; returns the number removed
	size_t RemoveAllOf(const CStr8 &strRemove, const bool bCaseInsensitive = false);
	size_t RemoveAllOf(PCSTR szRemove, const bool bCaseInsensitive = false);
	// Keeps any and all characters in the string from the string object, optionally case insensitive; returns the number kept
	size_t KeepAllOf(const CStr8 &strKeep, const bool bCaseInsensitive = false);
	size_t KeepAllOf(PCSTR szKeep, const bool bCaseInsensitive = false);
	// Replaces all instances of the string/character in the string object with the new string/character, optionally case insensitive; returns the number replaced
	size_t Replace(const CStr8 &strOld, const CStr8 &strNew, const bool bCaseInsensitive = false);
	size_t Replace(PCSTR szOld, PCSTR szNew, const bool bCaseInsensitive = false);
	size_t Replace(const char chOld, const char chNew, const bool bCaseInsensitive = false);

	// Efficiently swap two strings without allocating new buffers
	void Swap(CStr8 &src) noexcept;

	// Returns a substring from the string object given the starting index and length
	CStr8 GetMid(const size_t nStart, const size_t nCount = end) const;
	// Returns a substring containing 'nCount' characters from the left of the string object
	CStr8 GetLeft(const size_t nCount) const;
	// Returns a substring containing 'nCount' characters from the right of the string object
	CStr8 GetRight(const size_t nCount) const;

	// Removes any and all characters in the provided string/character from both the left and right of the string object, optionally case insensitive
	CStr8 &Trim(const CStr8 &strTrim, const bool bCaseInsensitive = false);
	CStr8 &Trim(PCSTR szTrim, const bool bCaseInsensitive = false);
	CStr8 &Trim(const char chTrim, const bool bCaseInsensitive = false);
	// Removes any whitespace characters from both the left and right of the string object
	CStr8 &Trim(void);
	// Removes any and all characters in the provided string/character from the left of the string object, optionally case insensitive
	CStr8 &TrimLeft(const CStr8 &strTrim, const bool bCaseInsensitive = false);
	CStr8 &TrimLeft(PCSTR szTrim, const bool bCaseInsensitive = false);
	CStr8 &TrimLeft(const char chTrim, const bool bCaseInsensitive = false);
	// Removes any whitespace characters from the left of the string object
	CStr8 &TrimLeft(void);
	// Removes any and all characters in the provided string/character from the right of the string object, optionally case insensitive
	CStr8 &TrimRight(const CStr8 &strTrim, const bool bCaseInsensitive = false);
	CStr8 &TrimRight(PCSTR szTrim, const bool bCaseInsensitive = false);
	CStr8 &TrimRight(const char chTrim, const bool bCaseInsensitive = false);
	// Removes any whitespace characters from the right of the string object
	CStr8 &TrimRight(void);

	// Removes all characters from the string object except the middle 'nCount' characters beginning at 'nStart'
	CStr8 &CropMid(const size_t nStart, const size_t nCount = end);
	// Removes all characters from the string object except the leftmost 'nCount' characters
	CStr8 &CropLeft(const size_t nCount);
	// Removes all characters from the string object except the rightmost 'nCount' characters
	CStr8 &CropRight(const size_t nCount);

	// Returns an uppercase copy of the string
	CStr8 GetUpper(void) const;
	// Returns a lowercase copy of the string
	CStr8 GetLower(void) const;

	// Converts all characters in the string object to uppercase
	CStr8 &MakeUpper(void);
	// Converts all characters in the string object to lowercase
	CStr8 &MakeLower(void);
	// Reverses all characters in the string front to back
	CStr8 &MakeReverse(void);

	// Returns a substring of the string object starting at the given index and going until the first character in the provided string/character is found, optionally case insensitive
	CStr8 Tokenize(const CStr8 &strTokens, size_t &nStart, const bool bCaseInsensitive = false) const;
	CStr8 Tokenize(PCSTR szTokens, size_t &nStart, const bool bCaseInsensitive = false) const;
	CStr8 Tokenize(const char chToken, size_t &nStart, const bool bCaseInsensitive = false) const;

	// Formats the string using standard printf syntax
	bool Printf(PCSTR szFormat, ...);
	bool PrintfV(PCSTR szFormat, va_list vaArgs);

	// Formats the string using variadic templates
	template<typename ...ARGS>
	void Format(PCSTR szFormat, const ARGS&... args)
	{
		*this = VFormat(szFormat, args...);
	}

	// Ensures the buffer is at least 'nBufLen' in length
	bool ReserveBuffer(const size_t nBufLen);
	// Returns the length of the buffer in characters
	size_t GetBufferLength(void) const noexcept;
	// Returns the size of the buffer in bytes
	size_t GetBufferSize(void) const noexcept;
	// Returns a pointer to the raw string data for direct buffer manipulation, ensuring the buffer is at least 'nMinBufLen' in length
	PSTR GetBuffer(const size_t nMinBufLen = 0);
	// Updates the string object after a call to GetBuffer(), and setting the null-terminator to 'nNewStrLen' or calculating the string length if 'end'
	void ReleaseBuffer(const size_t nNewStrLen = end);
	// Allocates a new string buffer only as long as the current string object, thus freeing any extra unused buffer
	void FreeExtra(void);

	void DebugPrint(const bool bAppendNewline = true, const bool bConsolePrint = false) const;

private:
	PSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static size_t GetLength(PCSTR sz, const size_t nMaxLen);
	static PSTR FindStrPtr(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static PSTR FindStrPtrI(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static size_t FindStrIndex(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static size_t FindStrIndexI(PCSTR sz, const size_t nStrLen, PCSTR szFind, const size_t nFindLen);
	static PCSTR memchri(PCSTR sz, const char ch, const size_t nLen);
	static int memcmpi(PCSTR sz1, PCSTR sz2, const size_t nLen);
	static size_t str2int(PCSTR sz);

	template<typename T>
	static std::string ToString(const T &val)
	{
		std::ostringstream stream;
		stream << val;
		return stream.str();
	}

	// VFormat is a function that provides a safe C++ alternative to C printf.  It uses C++11 variadic templates
	// The argument position is surrounded by curly braces.  Extra curly braces are ignored, as are invalid positions inside the curly braces.
	// Examples
	//  VFormat("Name: {0}, Age: {1}", "Brian", 42);		"Name: Brian, Age: 42"
	//  VFormat("{1} == {0}", 3.14, "pi");					"pi == 3.14"
	//  VFormat("Test {{0}}", 15);							"Test {15}"
	//  VFormat("String {a} {2}", 3);						"String {a} {2}"
	template<typename ...ARGS>
	static CStr8 VFormat(PCSTR szFormat, const ARGS&... args)
	{
		std::ostringstream stream;

		const std::string arrArgs[] = {ToString(args)...};
		size_t nArgsCount = sizeof...(args);

		PCSTR szScan = szFormat;
		PCSTR szEnd = szScan + GetLength(szFormat, end);
		while(szScan != szEnd)
		{
			if(*szScan == '{')
			{
				bool bValidIndex = true;

				PCSTR szOpen = szScan;
				PCSTR szLeft = ++szScan;
				while(szScan != szEnd)
				{
					if(*szScan == '}')
						break;

					if(*szScan < '0' || *szScan > '9')
					{
						bValidIndex = false;
						break;
					}

					++szScan;
				}

				PCSTR szRight = szScan;
				if(szScan == szEnd || *szScan != '}' || szLeft == szRight)
					bValidIndex = false;

				if(bValidIndex)
				{
					CStr8 strIndex(szLeft, 0, szRight - szLeft);
					size_t nIndex = str2int(strIndex);
					if(nIndex < nArgsCount)
						stream << arrArgs[nIndex];
					else
					{
						stream << CStr8(szOpen, 0, szScan - szOpen);
						continue;
					}
				}
				else
				{
					stream << CStr8(szOpen, 0, szScan - szOpen);
					continue;
				}
			}
			else
				stream << *szScan;

			++szScan;
		}

		return CStr8(stream.str());
	}
};


//#################################################################################################
class CStrW final
{
private:
	friend class CStr8;
	friend class CStr16;

public:
	enum EPrintfType : uint8_t
	{
		EPT_Printf = 0
	};

	enum EFormatType : uint8_t
	{
		EFT_Format = 0
	};

	static const size_t not_found;
	static const size_t end;

	// Constructs an empty string object
	CStrW(void);
	// Copy and move constructors
	CStrW(const CStrW &src);
	CStrW(CStrW &&src) noexcept;
	// Constructs a string object set to the string/character
	explicit CStrW(const CStr8 &str);
	explicit CStrW(const CStr16 &str);
	explicit CStrW(const std::wstring &str);
	explicit CStrW(const std::string &str);
	explicit CStrW(PCWSTR sz);
	explicit CStrW(PCSTR sz);
	explicit CStrW(const wchar_t ch);
	explicit CStrW(const char ch);
	// Constructs a string object set to a substring, given the starting location and length
	CStrW(const CStrW &str, const size_t nStart, const size_t nLen = end);
	CStrW(PCWSTR sz, const size_t nStart, const size_t nLen = end);
	// Constructs a formatted string using printf formatting; set 'eIgnored' to 'EPT_Printf' and 'szFormat' contains the string with format specifiers
	CStrW(const EPrintfType eIgnored, PCWSTR szFormat, ...);
	// Constructs a formatted string using C++ variadic templates; set 'eIgnored' to 'EFT_Format' and 'szFormat' contains the string with format specifiers
	template<typename ...ARGS>
	CStrW(const EFormatType eIgnored, PCWSTR szFormat, const ARGS&... args)
		: m_sz((PWSTR)g_szNullW),
		m_nBufLen(0),
		m_nStrLen(0)
	{
		UNUSED(eIgnored);

		*this = VFormat(szFormat, args...);
	}

	enum ELoadType : uint8_t
	{
		ELT_Load = 0
	};
	// Constructs a string by loading it from the string resources table for the current application module
	CStrW(const ELoadType eIgnored, const int nId);

	~CStrW(void);

	// Attaches to an existing null-terminated string buffer; buffer should have been allocated using 'malloc'
	CStrW &Attach(PWSTR sz, const size_t nBufLen);
	// Releases the internal string buffer, the caller is responsible for freeing the buffer with 'free'
	PWSTR Detach(void) noexcept;

	// Overwrites the memory with all zeros to securely erase the string
	void SecureErase(void);

	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the count of characters
	size_t GetCount(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;

	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;
	// Empties the string, optionally freeing the memory
	void Empty(const bool bFree = false);
	// Returns the first character in the string, or '\0' if the string is empty
	wchar_t GetFirst(void) const;
	// Returns the last character in the string, or '\0' if the string is empty
	wchar_t GetLast(void) const;
	// Returns the character at the given index, or '\0' if the index is beyond the string
	wchar_t GetAt(const size_t nIndex) const;
	// Sets the character at the given index, so long as index is not beyond the string
	void SetAt(const size_t nIndex, const wchar_t ch);
	// Returns a pointer to the raw string data
	operator PCWSTR(void) const noexcept;

	// Returns a UTF8 copy of the string
	CStr8 AsUtf8(void) const;
	// Returns a wchar_t copy of the string
	CStrW AsWide(void) const;
	CStrW AsNative(void) const;

	// Copy and move assignment operators
	CStrW &operator=(const CStrW &src);
	CStrW &operator=(CStrW &&src) noexcept;
	// Assigns the string object to the string/character
	CStrW &operator=(const CStr8 &str);
	CStrW &operator=(const CStr16 &str);
	CStrW &operator=(const std::wstring &str);
	CStrW &operator=(const std::string &str);
	CStrW &operator=(PCWSTR sz);
	CStrW &operator=(PCSTR sz);
	CStrW &operator=(const wchar_t ch);
	CStrW &operator=(const char ch);

	// Assigns the string object to the string/character
	void Assign(const CStrW &str);
	void Assign(const CStr8 &str);
	void Assign(const CStr16 &str);
	void Assign(const std::wstring &str);
	void Assign(const std::string &str);
	void Assign(PCWSTR sz);
	void Assign(PCSTR sz);
	void Assign(const wchar_t ch);
	void Assign(const char ch);
	// Assigns the string object to a substring, given the starting location and length
	void Assign(const CStrW &str, const size_t nStart, const size_t nLen = end);
	void Assign(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	void Assign(PCWSTR sz, const size_t nStart, const size_t nLen = end);
	void Assign(PCSTR sz, const size_t nStart, const size_t nLen = end);

	// Appends the string/character
	CStrW &operator+=(const CStrW &str);
	CStrW &operator+=(const CStr8 &str);
	CStrW &operator+=(PCWSTR sz);
	CStrW &operator+=(PCSTR sz);
	CStrW &operator+=(const wchar_t ch);
	CStrW &operator+=(const char ch);

	// Prepends the string/character
	void Prepend(const CStrW &str);
	void Prepend(const CStr8 &str);
	void Prepend(PCWSTR sz);
	void Prepend(PCSTR sz);
	void Prepend(const wchar_t ch);
	void Prepend(const char ch);
	// Prepends a substring, given the starting location and length
	void Prepend(const CStrW &str, const size_t nStart, const size_t nLen = end);
	void Prepend(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	void Prepend(PCWSTR sz, const size_t nStart, const size_t nLen = end);
	void Prepend(PCSTR sz, const size_t nStart, const size_t nLen = end);

	// Appends the string/character
	void Append(const CStrW &str);
	void Append(const CStr8 &str);
	void Append(PCWSTR sz);
	void Append(PCSTR sz);
	void Append(const wchar_t ch);
	void Append(const char ch);
	// Appends a substring, given the starting location and length
	void Append(const CStrW &str, const size_t nStart, const size_t nLen = end);
	void Append(const CStr8 &str, const size_t nStart, const size_t nLen = end);
	void Append(PCWSTR sz, const size_t nStart, const size_t nLen = end);
	void Append(PCSTR sz, const size_t nStart, const size_t nLen = end);

	// Returns the concatenation of two strings/characters
	friend CStrW operator+(const CStrW &str1, const CStrW &str2);
	friend CStrW operator+(const CStrW &str1, const CStr8 &str2);
	friend CStrW operator+(const CStr8 &str1, const CStrW &str2);
	friend CStrW operator+(const CStr8 &str1, const CStr8 &str2);
	friend CStrW operator+(const CStrW &str1, PCWSTR sz2);
	friend CStrW operator+(const CStr8 &str1, PCWSTR sz2);
	friend CStrW operator+(const CStrW &str1, PCSTR sz2);
	friend CStrW operator+(const CStr8 &str1, PCSTR sz2);
	friend CStrW operator+(const CStrW &str1, const wchar_t ch2);
	friend CStrW operator+(const CStr8 &str1, const wchar_t ch2);
	friend CStrW operator+(const CStrW &str1, const char ch2);
	friend CStrW operator+(const CStr8 &str1, const char ch2);
	friend CStrW operator+(PCWSTR sz1, const CStrW &str2);
	friend CStrW operator+(PCWSTR sz1, const CStr8 &str2);
	friend CStrW operator+(PCSTR sz1, const CStrW &str2);
	friend CStrW operator+(PCSTR sz1, const CStr8 &str2);
	friend CStrW operator+(const wchar_t ch1, const CStrW &str2);
	friend CStrW operator+(const wchar_t ch1, const CStr8 &str2);
	friend CStrW operator+(const char ch1, const CStrW &str2);
	friend CStrW operator+(const char ch1, const CStr8 &str2);

	friend std::wostream &operator<<(std::wostream &stream, const CStrW &str);

	// Tests for equal (case sensitive)
	bool operator==(const CStrW &str) const;
	bool operator==(PCWSTR sz) const;
	bool operator==(const wchar_t ch) const;

	// Tests for not equal (case sensitive)
	bool operator!=(const CStrW &str) const;
	bool operator!=(PCWSTR sz) const;
	bool operator!=(const wchar_t ch) const;

	// Tests for less than (case sensitive)
	bool operator<(const CStrW &str) const;
	bool operator<(PCWSTR sz) const;
	bool operator<(const wchar_t ch) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CStrW &str) const;
	bool operator<=(PCWSTR sz) const;
	bool operator<=(const wchar_t ch) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CStrW &str) const;
	bool operator>(PCWSTR sz) const;
	bool operator>(const wchar_t ch) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CStrW &str) const;
	bool operator>=(PCWSTR sz) const;
	bool operator>=(const wchar_t ch) const;

	// Compares the string object against the string/character, optionally case insensitive
	int Compare(const CStrW &str, const bool bCaseInsensitive = false) const;
	int Compare(PCWSTR sz, const bool bCaseInsensitive = false) const;
	int Compare(const wchar_t ch, const bool bCaseInsensitive = false) const;

	// Compares strings with embedded numbers, optionally case insensitive
	int CompareAlphaNum(const CStrW &str, const bool bCaseInsensitive = false) const;

	// Compares a substring of the string object against the entire string/character, given the starting location and length, optionally case insensitive
	int CompareMid(const CStrW &str, const size_t nStart, const size_t nCount = end, const bool bCaseInsensitive = false) const;
	int CompareMid(PCWSTR sz, const size_t nStart, const size_t nCount = end, const bool bCaseInsensitive = false) const;
	int CompareMid(const wchar_t ch, const size_t nStart, const bool bCaseInsensitive = false) const;
	// Compares the leftmost characters of the string object against the string/character, up to 'nCount' characters, optionally case insensitive
	int CompareLeft(const CStrW &str, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareLeft(PCWSTR sz, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareLeft(const wchar_t ch, const bool bCaseInsensitive = false) const;
	// Compares the rightmost characters of the string object against the string/character, up to 'nCount' characters, optionally case insensitive
	int CompareRight(const CStrW &str, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareRight(PCWSTR sz, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareRight(const wchar_t ch, const bool bCaseInsensitive = false) const;

	// Returns the count of instances of a given character in the string object, optionally case insensitive
	size_t Count(const wchar_t chFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the count of characters that the string object and provided string match starting at the left, optionally case insensitive
	size_t CountCompare(const CStrW &str, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t CountCompare(PCWSTR sz, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the count of characters that the string object and provided string match starting at the right, optionally case insensitive
	size_t ReverseCountCompare(const CStrW &str, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t ReverseCountCompare(PCWSTR sz, const size_t nStart = end, const bool bCaseInsensitive = false) const;

	// Tests if the string/character is in the string object given the starting index, optionally case insensitive
	bool Contains(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	bool Contains(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	bool Contains(const wchar_t chFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Tests for any instance of any character in the provided string given the starting index, optionally case insensitive
	bool ContainsOneOf(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	bool ContainsOneOf(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the first instance of the string/character in the string object given the starting index, optionally case insensitive; returns 'not_found' if the string/character is not in the string object
	size_t Find(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t Find(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t Find(const wchar_t chFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the last instance of the string/character in the string object given the starting index and working backwards, optionally case insensitive; returns 'not_found' if the string/character is not in the string object
	size_t ReverseFind(const CStrW &strFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t ReverseFind(PCWSTR szFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t ReverseFind(const wchar_t chFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	// Returns the index of the first instance of any character in the provided string given the starting index, optionally case insensitive; returns 'not_found' if none of the characters in the string are in the string object
	size_t FindFirstOf(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t FindFirstOf(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the first instance of any character not in the provided string given the starting index, optionally case insensitive; returns 'not_found' if all of the characters in the string object are in the provided string
	size_t FindFirstNotOf(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	size_t FindFirstNotOf(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns the index of the last instance of any character in the provided string given the starting index and working backwards, optionally case insensitive; returns 'not_found' if none of the characters in the string are in the string object
	size_t FindLastOf(const CStrW &strFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t FindLastOf(PCWSTR szFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	// Returns the index of the last instance of any character not in the provided string given the starting index and working backwards, optionally case insensitive; returns 'not_found' if all of the characters in the string object are in the provided string
	size_t FindLastNotOf(const CStrW &strFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;
	size_t FindLastNotOf(PCWSTR szFind, const size_t nStart = end, const bool bCaseInsensitive = false) const;

	// Returns a substring of the string object starting at the given index and going until the first character not found in the provided string, optionally case insensitive
	CStrW SpanIncluding(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	CStrW SpanIncluding(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	// Returns a substring of the string object starting at the given index and going until the first character found in the provided string, optionally case insensitive
	CStrW SpanExcluding(const CStrW &strFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;
	CStrW SpanExcluding(PCWSTR szFind, const size_t nStart = 0, const bool bCaseInsensitive = false) const;

	// Inserts the string/character at the given index, if the index is beyond the string the string/character is appended; returns the new string length
	size_t Insert(const size_t nIndex, const CStrW &strInsert);
	size_t Insert(const size_t nIndex, PCWSTR szInsert);
	size_t Insert(const size_t nIndex, const wchar_t chInsert);
	// Removes 'nCount' characters from the string object starting at the given index; returns the new string length
	size_t Delete(const size_t nIndex, const size_t nCount = 1);
	// Removes all instances of the string/character from the string object, optionally case insensitive; returns the number removed
	size_t Remove(const CStrW &strRemove, const bool bCaseInsensitive = false);
	size_t Remove(PCWSTR szRemove, const bool bCaseInsensitive = false);
	size_t Remove(const wchar_t chRemove, const bool bCaseInsensitive = false);
	// Removes any and all characters in the string from the string object, optionally case insensitive; returns the number removed
	size_t RemoveAllOf(const CStrW &strRemove, const bool bCaseInsensitive = false);
	size_t RemoveAllOf(PCWSTR szRemove, const bool bCaseInsensitive = false);
	// Keeps any and all characters in the string from the string object, optionally case insensitive; returns the number kept
	size_t KeepAllOf(const CStrW &strKeep, const bool bCaseInsensitive = false);
	size_t KeepAllOf(PCWSTR szKeep, const bool bCaseInsensitive = false);
	// Replaces all instances of the string/character in the string object with the new string/character, optionally case insensitive; returns the number replaced
	size_t Replace(const CStrW &strOld, const CStrW &strNew, const bool bCaseInsensitive = false);
	size_t Replace(PCWSTR szOld, PCWSTR szNew, const bool bCaseInsensitive = false);
	size_t Replace(const wchar_t chOld, const wchar_t chNew, const bool bCaseInsensitive = false);

	// Efficiently swap two strings without allocating new buffers
	void Swap(CStrW &src) noexcept;

	// Returns a substring from the string object given the starting index and length
	CStrW GetMid(const size_t nStart, const size_t nCount = end) const;
	// Returns a substring containing 'nCount' characters from the left of the string object
	CStrW GetLeft(const size_t nCount) const;
	// Returns a substring containing 'nCount' characters from the right of the string object
	CStrW GetRight(const size_t nCount) const;

	// Removes any and all characters in the provided string/character from both the left and right of the string object, optionally case insensitive
	CStrW &Trim(const CStrW &strTrim, const bool bCaseInsensitive = false);
	CStrW &Trim(PCWSTR szTrim, const bool bCaseInsensitive = false);
	CStrW &Trim(const wchar_t chTrim, const bool bCaseInsensitive = false);
	// Removes any whitespace characters from both the left and right of the string object
	CStrW &Trim(void);
	// Removes any and all characters in the provided string/character from the left of the string object, optionally case insensitive
	CStrW &TrimLeft(const CStrW &strTrim, const bool bCaseInsensitive = false);
	CStrW &TrimLeft(PCWSTR szTrim, const bool bCaseInsensitive = false);
	CStrW &TrimLeft(const wchar_t chTrim, const bool bCaseInsensitive = false);
	// Removes any whitespace characters from the left of the string object
	CStrW &TrimLeft(void);
	// Removes any and all characters in the provided string/character from the right of the string object, optionally case insensitive
	CStrW &TrimRight(const CStrW &strTrim, const bool bCaseInsensitive = false);
	CStrW &TrimRight(PCWSTR szTrim, const bool bCaseInsensitive = false);
	CStrW &TrimRight(const wchar_t chTrim, const bool bCaseInsensitive = false);
	// Removes any whitespace characters from the right of the string object
	CStrW &TrimRight(void);

	// Removes all characters from the string object except the middle 'nCount' characters beginning at 'nStart'
	CStrW &CropMid(const size_t nStart, const size_t nCount = end);
	// Removes all characters from the string object except the leftmost 'nCount' characters
	CStrW &CropLeft(const size_t nCount);
	// Removes all characters from the string object except the rightmost 'nCount' characters
	CStrW &CropRight(const size_t nCount);

	// Returns an uppercase copy of the string
	CStrW GetUpper(void) const;
	// Returns a lowercase copy of the string
	CStrW GetLower(void) const;

	// Converts all characters in the string object to uppercase
	CStrW &MakeUpper(void);
	// Converts all characters in the string object to lowercase
	CStrW &MakeLower(void);
	// Reverses all characters in the string front to back
	CStrW &MakeReverse(void);

	// Returns a substring of the string object starting at the given index and going until the first character in the provided string/character is found, optionally case insensitive
	CStrW Tokenize(const CStrW &strTokens, size_t &nStart, const bool bCaseInsensitive = false) const;
	CStrW Tokenize(PCWSTR szTokens, size_t &nStart, const bool bCaseInsensitive = false) const;
	CStrW Tokenize(const wchar_t chToken, size_t &nStart, const bool bCaseInsensitive = false) const;

	bool LoadString(const UINT nId);

	// Formats the string using standard printf syntax
	bool Printf(PCWSTR szFormat, ...);
	bool PrintfV(PCWSTR szFormat, va_list vaArgs);

	// Formats the string using variadic templates
	template<typename ...ARGS>
	void Format(PCWSTR szFormat, const ARGS&... args)
	{
		*this = VFormat(szFormat, args...);
	}

	// Ensures the buffer is at least 'nBufLen' in length
	bool ReserveBuffer(const size_t nBufLen);
	// Returns the length of the buffer in characters
	size_t GetBufferLength(void) const noexcept;
	// Returns the size of the buffer in bytes
	size_t GetBufferSize(void) const noexcept;
	// Returns a pointer to the raw string data for direct buffer manipulation, ensuring the buffer is at least 'nMinBufLen' in length
	PWSTR GetBuffer(const size_t nMinBufLen = 0);
	// Updates the string object after a call to GetBuffer(), and setting the null-terminator to 'nNewStrLen' or calculating the string length if 'end'
	void ReleaseBuffer(const size_t nNewStrLen = end);
	// Allocates a new string buffer only as long as the current string object, thus freeing any extra unused buffer
	void FreeExtra(void);

	void DebugPrint(const bool bAppendNewline = true, const bool bConsolePrint = false) const;

private:
	PWSTR m_sz;				// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static size_t GetLength(PCWSTR sz, const size_t nMaxLen);
	static PWSTR FindStrPtr(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static PWSTR FindStrPtrI(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static size_t FindStrIndex(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static size_t FindStrIndexI(PCWSTR sz, const size_t nStrLen, PCWSTR szFind, const size_t nFindLen);
	static PCWSTR wmemchri(PCWSTR sz, const wchar_t ch, const size_t nLen);
	static int wmemcmpi(PCWSTR sz1, PCWSTR sz2, const size_t nLen);
	static size_t str2int(PCWSTR sz);

	template<typename T>
	static std::wstring ToString(const T &val)
	{
		std::wostringstream stream;
		stream << val;
		return stream.str();
	}

	// VFormat is a function that provides a safe C++ alternative to C printf.  It uses C++11 variadic templates
	// The argument position is surrounded by curly braces.  Extra curly braces are ignored, as are invalid positions inside the curly braces.
	// Examples
	//  VFormat("Name: {0}, Age: {1}", "Brian", 42);		"Name: Brian, Age: 42"
	//  VFormat("{1} == {0}", 3.14, "pi");					"pi == 3.14"
	//  VFormat("Test {{0}}", 15);							"Test {15}"
	//  VFormat("String {a} {2}", 3);						"String {a} {2}"
	template<typename ...ARGS>
	static CStrW VFormat(PCWSTR szFormat, const ARGS&... args)
	{
		std::wostringstream stream;

		const std::wstring arrArgs[] = {ToString(args)...};
		size_t nArgsCount = sizeof...(args);

		PCWSTR szScan = szFormat;
		PCWSTR szEnd = szScan + GetLength(szFormat, end);
		while(szScan != szEnd)
		{
			if(*szScan == L'{')
			{
				bool bValidIndex = true;

				PCWSTR szOpen = szScan;
				PCWSTR szLeft = ++szScan;
				while(szScan != szEnd)
				{
					if(*szScan == L'}')
						break;

					if(*szScan < L'0' || *szScan > L'9')
					{
						bValidIndex = false;
						break;
					}

					++szScan;
				}

				PCWSTR szRight = szScan;
				if(szScan == szEnd || *szScan != L'}' || szLeft == szRight)
					bValidIndex = false;

				if(bValidIndex)
				{
					CStrW strIndex(szLeft, 0, szRight - szLeft);
					size_t nIndex = str2int(strIndex);
					if(nIndex < nArgsCount)
						stream << arrArgs[nIndex];
					else
					{
						stream << CStrW(szOpen, 0, szScan - szOpen);
						continue;
					}
				}
				else
				{
					stream << CStrW(szOpen, 0, szScan - szOpen);
					continue;
				}
			}
			else
				stream << *szScan;

			++szScan;
		}

		return CStrW(stream.str());
	}
};


//#################################################################################################
class CStr16 final
{
private:
	friend class CStr8;
	friend class CStrW;

public:
	static const size_t not_found;
	static const size_t end;

	// Constructs an empty string object
	CStr16(void);
	// Copy and move constructors
	CStr16(const CStr16 &src);
	CStr16(CStr16 &&src) noexcept;
	// Constructs a string object set to the string/character
	explicit CStr16(const CStr8 &str);
	explicit CStr16(const CStrW &str);
	explicit CStr16(const char16_t *sz);
	explicit CStr16(const char16_t ch);
	// Constructs a string object set to a substring, given the starting location and length
	CStr16(const CStr16 &str, const size_t nStart, const size_t nLen = end);
	CStr16(const char16_t *sz, const size_t nStart, const size_t nLen = end);

	~CStr16(void);

	// Attaches to an existing null-terminated string buffer; buffer should have been allocated using 'malloc'
	CStr16 &Attach(char16_t *sz, const size_t nBufLen);
	// Releases the internal string buffer, the caller is responsible for freeing the buffer with 'free'
	char16_t *Detach(void) noexcept;

	// Overwrites the memory with all zeros to securely erase the string
	void SecureErase(void);

	// Returns the length in characters
	size_t GetLength(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the count of characters
	size_t GetCount(const bool bIncludeNullTerm = false) const noexcept;
	// Returns the size in bytes
	size_t GetSize(const bool bIncludeNullTerm = false) const noexcept;

	// Returns whether the string is empty
	bool IsEmpty(void) const noexcept;
	// Empties the string, optionally freeing the memory
	void Empty(const bool bFree = false);
	// Returns the first character in the string, or '\0' if the string is empty
	char16_t GetFirst(void) const;
	// Returns the last character in the string, or '\0' if the string is empty
	char16_t GetLast(void) const;
	// Returns the character at the given index, or '\0' if the index is beyond the string
	char16_t GetAt(const size_t nIndex) const;
	// Sets the character at the given index, so long as index is not beyond the string
	void SetAt(const size_t nIndex, const char16_t ch);
	// Returns a pointer to the raw string data
	operator const char16_t*(void) const noexcept;

	// Returns a UTF8 copy of the string
	CStr8 AsUtf8(void) const;
	// Returns a wchar_t copy of the string
	CStrW AsWide(void) const;
	CStrW AsNative(void) const;

	// Copy and move assignment operators
	CStr16 &operator=(const CStr16 &src);
	CStr16 &operator=(CStr16 &&src) noexcept;
	// Assigns the string object to the string/character
	CStr16 &operator=(const CStr8 &str);
	CStr16 &operator=(const CStrW &str);
	CStr16 &operator=(const char16_t *sz);
	CStr16 &operator=(const char16_t ch);

	// Assigns the string object to the string/character
	void Assign(const CStr16 &str);
	void Assign(const CStr8 &str);
	void Assign(const CStrW &str);
	void Assign(const char16_t *sz);
	void Assign(const char16_t ch);
	// Assigns the string object to a substring, given the starting location and length
	void Assign(const CStr16 &str, const size_t nStart, const size_t nLen = end);
	void Assign(const char16_t *sz, const size_t nStart, const size_t nLen = end);

	// Appends the string/character
	CStr16 &operator+=(const CStr16 &str);
	CStr16 &operator+=(const CStr8 &str);
	CStr16 &operator+=(const CStrW &str);
	CStr16 &operator+=(const char16_t *sz);
	CStr16 &operator+=(const char16_t ch);

	// Prepends the string/character
	void Prepend(const CStr16 &str);
	void Prepend(const CStr8 &str);
	void Prepend(const CStrW &str);
	void Prepend(const char16_t *sz);
	void Prepend(const char16_t ch);
	// Prepends a substring, given the starting location and length
	void Prepend(const CStr16 &str, const size_t nStart, const size_t nLen = end);
	void Prepend(const char16_t *sz, const size_t nStart, const size_t nLen = end);

	// Appends the string/character
	void Append(const CStr16 &str);
	void Append(const CStr8 &str);
	void Append(const CStrW &str);
	void Append(const char16_t *sz);
	void Append(const char16_t ch);
	// Appends a substring, given the starting location and length
	void Append(const CStr16 &str, const size_t nStart, const size_t nLen = end);
	void Append(const char16_t *sz, const size_t nStart, const size_t nLen = end);

	// Tests for equal (case sensitive)
	bool operator==(const CStr16 &str) const;
	bool operator==(const char16_t *sz) const;
	bool operator==(const char16_t ch) const;

	// Tests for not equal (case sensitive)
	bool operator!=(const CStr16 &str) const;
	bool operator!=(const char16_t *sz) const;
	bool operator!=(const char16_t ch) const;

	// Tests for less than (case sensitive)
	bool operator<(const CStr16 &str) const;
	bool operator<(const char16_t *sz) const;
	bool operator<(const char16_t ch) const;

	// Tests for less than or equal (case sensitive)
	bool operator<=(const CStr16 &str) const;
	bool operator<=(const char16_t *sz) const;
	bool operator<=(const char16_t ch) const;

	// Tests for greater than (case sensitive)
	bool operator>(const CStr16 &str) const;
	bool operator>(const char16_t *sz) const;
	bool operator>(const char16_t ch) const;

	// Tests for greater than or equal (case sensitive)
	bool operator>=(const CStr16 &str) const;
	bool operator>=(const char16_t *sz) const;
	bool operator>=(const char16_t ch) const;

	// Compares the string object against the string/character, optionally case insensitive
	int Compare(const CStr16 &str, const bool bCaseInsensitive = false) const;
	int Compare(const char16_t *sz, const bool bCaseInsensitive = false) const;
	int Compare(const char16_t ch, const bool bCaseInsensitive = false) const;

	// Compares a substring of the string object against the entire string/character, given the starting location and length, optionally case insensitive
	int CompareMid(const CStr16 &str, const size_t nStart, const size_t nCount = end, const bool bCaseInsensitive = false) const;
	int CompareMid(const char16_t *sz, const size_t nStart, const size_t nCount = end, const bool bCaseInsensitive = false) const;
	int CompareMid(const char16_t ch, const size_t nStart, const bool bCaseInsensitive = false) const;
	// Compares the leftmost characters of the string object against the string/character, up to 'nCount' characters, optionally case insensitive
	int CompareLeft(const CStr16 &str, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareLeft(const char16_t *sz, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareLeft(const char16_t ch, const bool bCaseInsensitive = false) const;
	// Compares the rightmost characters of the string object against the string/character, up to 'nCount' characters, optionally case insensitive
	int CompareRight(const CStr16 &str, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareRight(const char16_t *sz, const size_t nCount, const bool bCaseInsensitive = false) const;
	int CompareRight(const char16_t ch, const bool bCaseInsensitive = false) const;

	// Efficiently swap two strings without allocating new buffers
	void Swap(CStr16 &src) noexcept;

	// Returns a substring from the string object given the starting index and length
	CStr16 GetMid(const size_t nStart, const size_t nCount = end) const;
	// Returns a substring containing 'nCount' characters from the left of the string object
	CStr16 GetLeft(const size_t nCount) const;
	// Returns a substring containing 'nCount' characters from the right of the string object
	CStr16 GetRight(const size_t nCount) const;

	// Removes any whitespace characters from both the left and right of the string object
	CStr16 &Trim(void);
	// Removes any whitespace characters from the left of the string object
	CStr16 &TrimLeft(void);
	// Removes any whitespace characters from the right of the string object
	CStr16 &TrimRight(void);

	// Removes all characters from the string object except the middle 'nCount' characters beginning at 'nStart'
	CStr16 &CropMid(const size_t nStart, const size_t nCount = end);
	// Removes all characters from the string object except the leftmost 'nCount' characters
	CStr16 &CropLeft(const size_t nCount);
	// Removes all characters from the string object except the rightmost 'nCount' characters
	CStr16 &CropRight(const size_t nCount);

	// Returns an uppercase copy of the string
	CStr16 GetUpper(void) const;
	// Returns a lowercase copy of the string
	CStr16 GetLower(void) const;

	// Converts all characters in the string object to uppercase
	CStr16 &MakeUpper(void);
	// Converts all characters in the string object to lowercase
	CStr16 &MakeLower(void);
	// Reverses all characters in the string front to back
	CStr16 &MakeReverse(void);

	// Ensures the buffer is at least 'nBufLen' in length
	bool ReserveBuffer(const size_t nBufLen);
	// Returns the length of the buffer in characters
	size_t GetBufferLength(void) const noexcept;
	// Returns the size of the buffer in bytes
	size_t GetBufferSize(void) const noexcept;
	// Returns a pointer to the raw string data for direct buffer manipulation, ensuring the buffer is at least 'nMinBufLen' in length
	char16_t *GetBuffer(const size_t nMinBufLen = 0);
	// Updates the string object after a call to GetBuffer(), and setting the null-terminator to 'nNewStrLen' or calculating the string length if 'end'
	void ReleaseBuffer(const size_t nNewStrLen = end);
	// Allocates a new string buffer only as long as the current string object, thus freeing any extra unused buffer
	void FreeExtra(void);

	void DebugPrint(const bool bAppendNewline = true, const bool bConsolePrint = false) const;

private:
	char16_t *m_sz;			// String buffer
	size_t m_nBufLen;		// Length of the buffer in characters
	size_t m_nStrLen;		// Length of the string in characters

	bool Alloc(const size_t nBufLen);
	void Dealloc(void);

	static size_t GetLength(const char16_t *sz, const size_t nMaxLen);
};


using CStr = CStrW;
