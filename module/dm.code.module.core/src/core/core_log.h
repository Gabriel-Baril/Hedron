#pragma once

#include "core/core_define.h"
#include "core/core_macro.h"
#include "core/core_internal_api.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace dm
{
	DM_MODULE_CORE_API void log_init();
#if USING(DM_LOG_ENABLE)
	DM_MODULE_CORE_API Ref<spdlog::logger> &log_get_core_logger();
#endif
}

#if !USING(DM_DEV)
#define DM_CORE_BREAK() std::terminate();
#else
#define DM_CORE_BREAK() __debugbreak();
#endif

#if USING(DM_LOG_ENABLE)
#define DM_TRACE_LOG(...)                                          \
	{                                                                \
		SPDLOG_LOGGER_TRACE(::dm::log_get_core_logger(), __VA_ARGS__); \
	}
#define DM_DEBUG_LOG(...)                                          \
	{                                                                \
		SPDLOG_LOGGER_DEBUG(::dm::log_get_core_logger(), __VA_ARGS__); \
	}
#define DM_INFO_LOG(...)                                          \
	{                                                               \
		SPDLOG_LOGGER_INFO(::dm::log_get_core_logger(), __VA_ARGS__); \
	}
#define DM_WARNING_LOG(...)                                       \
	{                                                               \
		SPDLOG_LOGGER_WARN(::dm::log_get_core_logger(), __VA_ARGS__); \
	}
#define DM_ERROR_LOG(...)                                          \
	{                                                                \
		SPDLOG_LOGGER_ERROR(::dm::log_get_core_logger(), __VA_ARGS__); \
	}
#define DM_CRITICAL_LOG(...)                                          \
	{                                                                   \
		SPDLOG_LOGGER_CRITICAL(::dm::log_get_core_logger(), __VA_ARGS__); \
	}
#define DM_FATAL_LOG(...)         \
	{                               \
		DM_CRITICAL_LOG(__VA_ARGS__); \
		DM_CORE_BREAK();              \
	}
#else
#define DM_TRACE_LOG(...)
#define DM_DEBUG_LOG(...)
#define DM_INFO_LOG(...)
#define DM_WARNING_LOG(...)
#define DM_ERROR_LOG(...)
#define DM_CRITICAL_LOG(...)
#define DM_FATAL_LOG(...)
#endif

#if USING(DM_ASSERT_ENABLE)
#define DM_CORE_ASSERT_MACRO(expr, ...) \
	do                                    \
	{                                     \
		if (!(expr))                        \
		{                                   \
			DM_FATAL_LOG(__VA_ARGS__);        \
		}                                   \
	} while (false)

#define DM_CORE_ASSERT_MACRO_ALWAYS(...) \
	do                                     \
	{                                      \
		DM_FATAL_LOG(__VA_ARGS__);           \
	} while (false)

#define DM_CORE_ASSERT(expr, ...) DM_CORE_ASSERT_MACRO((expr), __VA_ARGS__)
#define DM_CORE_ASSERT_PTR(expr, ...) DM_CORE_ASSERT_MACRO((expr), __VA_ARGS__)
#define DM_CORE_ASSERT_RANGE(min, index, max, ...) DM_CORE_ASSERT_MACRO((index >= min && index <= max), __VA_ARGS__)
#define DM_CORE_ASSERT_ALWAYS(...) DM_CORE_ASSERT_MACRO_ALWAYS(__VA_ARGS__)
#define DM_CORE_UNIMPLEMENTED() DM_CORE_ASSERT_MACRO_ALWAYS("unimplemented code")
#define DM_CORE_UNREACHABLE() DM_CORE_ASSERT_MACRO_ALWAYS("unreachable code")
#else
#define DM_CORE_ASSERT_MACRO(...)
#define DM_CORE_ASSERT_MACRO_ALWAYS(...)

#define DM_CORE_ASSERT(...)
#define DM_CORE_ASSERT_PTR(...)
#define DM_CORE_ASSERT_RANGE(...)
#define DM_CORE_ASSERT_ALWAYS(...)

#define DM_CORE_UNIMPLEMENTED()
#define DM_CORE_UNREACHABLE()
#endif

#if USING(DM_THROW_ENABLE)
#define DM_CORE_THROW_FMT(exception, message, ...) \
	{                                                \
		throw exception(message);                      \
	}
#define DM_CORE_THROW(exception, message) \
	{                                       \
		throw exception(message);             \
	}
#else
#define DM_CORE_THROW_FMT(exception, message, ...) \
	{                                                \
		DM_FATAL_LOG(message, __VA_ARGS__);            \
	}
#define DM_CORE_THROW(exception, message) \
	{                                       \
		DM_FATAL_LOG(message);                \
	}
#endif

// Compared to an assert and check will exist even in non debug build
#if !USING(DM_DEV)
#define DM_CORE_CHECK_FMT(x, ...) DM_CORE_CHECK(x)
#define DM_CORE_CHECK(x) \
	{                      \
		if (!(x))            \
		{                    \
			DM_CORE_BREAK();   \
		}                    \
	}
#else
#define DM_CORE_CHECK_FMT(x, ...) DM_CORE_ASSERT(x, __VA_ARGS__);
#define DM_CORE_CHECK(x) DM_CORE_ASSERT(x);
#endif
