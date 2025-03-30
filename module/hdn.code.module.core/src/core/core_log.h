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
#if USING(LOG_ENABLE)
	HDN_MODULE_CORE_API Ref<spdlog::logger>& log_get_core_logger();
#endif
}


#if !USING(DEV)
#define HBREAK() std::terminate();
#else
#define HBREAK() __debugbreak();
#endif


#if USING(LOG_ENABLE)
#define HTRACE(...) { SPDLOG_LOGGER_TRACE(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HDEBUG(...)  { SPDLOG_LOGGER_DEBUG(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HINFO(...)  { SPDLOG_LOGGER_INFO(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HWARN(...)  { SPDLOG_LOGGER_WARN(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HERR(...)	{ SPDLOG_LOGGER_ERROR(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HCRIT(...)	{ SPDLOG_LOGGER_CRITICAL(::hdn::log_get_core_logger(), __VA_ARGS__); }
#define HFATAL(...) { HCRIT(__VA_ARGS__); HBREAK(); }
#else
#define HTRACE(...)
#define HDEBUG(...)
#define HINFO(...)
#define HWARN(...)
#define HERR(...)
#define HCRIT(...)
#define HFATAL(...)
#endif

#if USING(ASSERT_ENABLE)
#define HASSERT(x, ...) { if(!(x)) { HFATAL(__VA_ARGS__); } }
#define HASSERT_PTR(x, ...) { if(x == nullptr) { HFATAL(__VA_ARGS__); } }
#define HASSERT_INDEX(min, index, max, ...) { if(!(index >= 0 && index <= max)) { HFATAL(__VA_ARGS__); } }
#else
#define HASSERT(...)
#define HASSERT_PTR(...)
#define HASSERT_INDEX(...)
#endif

#if USING(THROW_ENABLE)
#define HTHROW_FMT(exception, message, ...) { throw exception(message); }
#define HTHROW(exception, message) { throw exception(message); }
#else
#define HTHROW_FMT(exception, message, ...) { HFATAL(message, __VA_ARGS__); }
#define HTHROW(exception, message) { HFATAL(message); }
#endif

// Compared to an assert and check will exist even in non debug build
#if !USING(DEV)
#define HCHECK_FMT(x, ...)	HCHECK(x)
#define HCHECK(x)			{ if(!(x)) { HBREAK(); } }
#else
#define HCHECK_FMT(x, ...)	HASSERT(x, __VA_ARGS__);
#define HCHECK(x)			HASSERT(x);
#endif