#pragma once


#include "core/core_define.h"
#include "core/core_macro.h"
#include "core/core_internal_api.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace hdn
{
	HDN_MODULE_CORE_API void log_init();
#if USING(HDN_LOG_ENABLE)
	HDN_MODULE_CORE_API Ref<spdlog::logger>& log_get_core_logger();
#endif
}


#if !USING(HDN_DEV)
#define HDN_CORE_BREAK() std::terminate();
#else
#define HDN_CORE_BREAK() __debugbreak();
#endif


#if USING(HDN_LOG_ENABLE)
#define HDN_TRACE_LOG(...) { SPDLOG_LOGGER_TRACE(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDN_DEBUG_LOG(...)  { SPDLOG_LOGGER_DEBUG(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDN_INFO_LOG(...)  { SPDLOG_LOGGER_INFO(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDN_WARNING_LOG(...)  { SPDLOG_LOGGER_WARN(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDN_ERROR_LOG(...)	{ SPDLOG_LOGGER_ERROR(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDN_CRITICAL_LOG(...)	{ SPDLOG_LOGGER_CRITICAL(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDN_FATAL_LOG(...) { HDN_CRITICAL_LOG(__VA_ARGS__); HDN_CORE_BREAK(); }
#else
#define HDN_TRACE_LOG(...)
#define HDN_DEBUG_LOG(...)
#define HDN_INFO_LOG(...)
#define HDN_WARNING_LOG(...)
#define HDN_ERROR_LOG(...)
#define HDN_CRITICAL_LOG(...)
#define HDN_FATAL_LOG(...)
#endif

#if USING(HDN_ASSERT_ENABLE)
#define HDN_CORE_ASSERT_MACRO(expr, ...) \
	do                                   \
	{                                    \
		if ( !( expr ) )				 \
		{                                \
			HDN_FATAL_LOG(__VA_ARGS__);	 \
		}                                \
	} while ( false )

#define HDN_CORE_ASSERT_MACRO_ALWAYS(...) \
	do                                    \
	{                                     \
		HDN_FATAL_LOG(__VA_ARGS__);	      \
	} while ( false )

#define HDN_CORE_ASSERT(expr, ...)					HDN_CORE_ASSERT_MACRO( (expr), __VA_ARGS__)
#define HDN_CORE_ASSERT_PTR(expr, ...)				HDN_CORE_ASSERT_MACRO( (expr), __VA_ARGS__)
#define HDN_CORE_ASSERT_RANGE(min, index, max, ...) HDN_CORE_ASSERT_MACRO( (index >= min && index <= max), __VA_ARGS__)
#define HDN_CORE_ASSERT_ALWAYS(...)					HDN_CORE_ASSERT_MACRO_ALWAYS(__VA_ARGS__)
#define HDN_CORE_UNIMPLEMENTED()					HDN_CORE_ASSERT_MACRO_ALWAYS("unimplemented code")
#define HDN_CORE_UNREACHABLE()						HDN_CORE_ASSERT_MACRO_ALWAYS("unreachable code")
#else
#define HDN_CORE_ASSERT_MACRO(...)
#define HDN_CORE_ASSERT_MACRO_ALWAYS(...)

#define HDN_CORE_ASSERT(...)
#define HDN_CORE_ASSERT_PTR(...)
#define HDN_CORE_ASSERT_RANGE(...)
#define HDN_CORE_ASSERT_ALWAYS(...)

#define HDN_CORE_UNIMPLEMENTED()
#define HDN_CORE_UNREACHABLE()
#endif

#if USING(HDN_THROW_ENABLE)
#define HDN_CORE_THROW_FMT(exception, message, ...) { throw exception(message); }
#define HDN_CORE_THROW(exception, message) { throw exception(message); }
#else
#define HDN_CORE_THROW_FMT(exception, message, ...) { HDN_FATAL_LOG(message, __VA_ARGS__); }
#define HDN_CORE_THROW(exception, message) { HDN_FATAL_LOG(message); }
#endif

// Compared to an assert and check will exist even in non debug build
#if !USING(HDN_DEV)
#define HDN_CORE_CHECK_FMT(x, ...)	HDN_CORE_CHECK(x)
#define HDN_CORE_CHECK(x)			{ if(!(x)) { HDN_CORE_BREAK(); } }
#else
#define HDN_CORE_CHECK_FMT(x, ...)	HDN_CORE_ASSERT(x, __VA_ARGS__);
#define HDN_CORE_CHECK(x)			HDN_CORE_ASSERT(x);
#endif