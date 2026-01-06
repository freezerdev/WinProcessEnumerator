#pragma once

#include "Common.h"

static constexpr auto FW_NO_ERROR = 0x0000;

static constexpr auto FW_ERROR_ABORTED = 0xC000;
static constexpr auto FW_ERROR_ACCESS_DENIED = 0xC001;
static constexpr auto FW_ERROR_BUSY = 0xC002;
static constexpr auto FW_ERROR_ENCRYPTION_FAILED = 0xC003;
static constexpr auto FW_ERROR_EXISTS = 0xC004;
static constexpr auto FW_ERROR_HANDLE_EOF = 0xC005;
static constexpr auto FW_ERROR_INSUFFICIENT_BUFFER = 0xC006;
static constexpr auto FW_ERROR_INTERRUPTED = 0xC007;
static constexpr auto FW_ERROR_INVALID_HANDLE = 0xC008;
static constexpr auto FW_ERROR_INVALID_PARAMETER = 0xC009;
static constexpr auto FW_ERROR_INVALID_DATA = 0xC00A;
static constexpr auto FW_ERROR_IO_PENDING = 0xC00B;
static constexpr auto FW_ERROR_MORE_DATA = 0xC00C;
static constexpr auto FW_ERROR_NO_MORE_DATA = 0xC00D;
static constexpr auto FW_ERROR_NOT_ENOUGH_MEMORY = 0xC00E;
static constexpr auto FW_ERROR_NOT_FOUND = 0xC00F;
static constexpr auto FW_ERROR_QUOTA = 0xC010;
static constexpr auto FW_ERROR_CRC = 0xC011;
static constexpr auto FW_ERROR_NOT_SUPPORTED = 0xC012;
static constexpr auto FW_ERROR_HASH = 0xC013;
static constexpr auto FW_ERROR_TOO_LARGE = 0xC014;
static constexpr auto FW_ERROR_LOW_LEVEL = 0xC015;
static constexpr auto FW_ERROR_IS_DIR = 0xC016;
static constexpr auto FW_ERROR_IS_NOT_DIR = 0xC017;
static constexpr auto FW_ERROR_LINK = 0xC018;
static constexpr auto FW_ERROR_HANDLES = 0xC019;
static constexpr auto FW_ERROR_EMPTY = 0xC01A;
static constexpr auto FW_ERROR_SIGN_FAIL = 0xC01B;
static constexpr auto FW_ERROR_NOT_CONNECTED = 0xC01C;
static constexpr auto FW_ERROR_SHUTTING_DOWN = 0xC01D;
static constexpr auto FW_ERROR_ALREADY_CONNECTED = 0xC01E;
static constexpr auto FW_ERROR_AUTHENTICATION_FAILED = 0xC01F;
static constexpr auto FW_ERROR_NOT_IMPLEMENTED = 0xC020;
static constexpr auto FW_ERROR_NETWORK_ISSUE = 0xC021;
static constexpr auto FW_ERROR_SERVER_WEBCALL_FAILED = 0xC022;
static constexpr auto FW_ERROR_SERVER_AUTH_FAILED = 0xC023;
static constexpr auto FW_ERROR_SERVER_EXCEPTION = 0xC024;
static constexpr auto FW_ERROR_DISABLED = 0xC025;
static constexpr auto FW_ERROR_NOT_EMPTY = 0xC026;
static constexpr auto FW_ERROR_SOME_FAILED = 0xC026;

static constexpr auto FW_ERROR_UNKNOWN = 0xCFFF;

ERRCODE ConvertFromNativeErrorCode(const ERRCODE nNativeErrorCode) noexcept;
