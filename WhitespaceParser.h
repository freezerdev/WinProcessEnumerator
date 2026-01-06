#pragma once

#include "Common.h"
#include "Str.h"
#include <vector>

//#################################################################################################
class CWhitespaceParser final
{
public:
	CWhitespaceParser(void) = default;
	CWhitespaceParser(const CWhitespaceParser &src) = default;
	CWhitespaceParser(CWhitespaceParser &&src) noexcept;
	explicit CWhitespaceParser(PCWSTR sz);
	explicit CWhitespaceParser(const CStr &str);

	size_t GetCount(void) const;
	size_t GetLength(const bool bIncludeNullTerm = false) const;	// Length in characters
	size_t GetSize(const bool bIncludeNullTerm = false) const;		// Size in bytes

	bool IsEmpty(void) const noexcept;
	void Empty(void);

	CStr Get(void) const;
	CStr Get(const size_t nParam) const;

	CWhitespaceParser &operator=(const CWhitespaceParser &src) = default;
	CWhitespaceParser &operator=(CWhitespaceParser &&src) noexcept;
	CWhitespaceParser &operator=(PCWSTR sz);
	CWhitespaceParser &operator=(const CStr &str);

	void Assign(PCWSTR sz);
	void Assign(const CStr &str);
	void Assign(const CWhitespaceParser &src);

	CWhitespaceParser &operator+=(PCWSTR sz);
	CWhitespaceParser &operator+=(const CStr &str);
	CWhitespaceParser &operator+=(const CWhitespaceParser &src);

	void Append(PCWSTR sz);
	void Append(const CStr &str);
	void Append(const CWhitespaceParser &src);

	void Insert(const size_t nParam, const CStr &str);
	void Delete(const size_t nParam, const size_t nParamCount = 1);

private:
	std::vector<CStr> m_vecParams;
};
