#pragma once

#define NOMINMAX
#include <windows.h>
#include <cstdint>
#include <memory>
#include <string>

using ERRCODE = unsigned int;

// Macro to eliminate unused variable compiler warnings
#ifndef UNUSED
#define UNUSED(x)	(void)(x)
#endif

#ifndef MIN
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)	((a) < (b) ? (b) : (a))
#endif

static constexpr char g_chNull8 = '\0';
static constexpr wchar_t g_chNullW = L'\0';
static constexpr char16_t g_chNull16 = u'\0';

static PCSTR g_szNull8 = "";
static PCWSTR g_szNullW = L"";
static const char16_t *g_szNull16 = u"";

static PCSTR g_szEol8 = "\r\n";
static PCWSTR g_szEolW = L"\r\n";
static const char16_t *g_szEol16 = u"\r\n";
static constexpr size_t g_nEolLen = 2;
#define g_szEol	g_szEolW
