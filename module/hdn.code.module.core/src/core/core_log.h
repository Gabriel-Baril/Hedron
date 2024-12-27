#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "core/core_define.h"
#include "core/core_macro.h"
#include "core/core_internal_api.h"

namespace hdn
{
	HDN_MODULE_CORE_API void Log_Init();
	HDN_MODULE_CORE_API Ref<spdlog::logger>& Log_GetCoreLogger();
}


#if !USING(DEV)
#define HBREAK() std::terminate();
#else
#define HBREAK() __debugbreak();
#endif


#if USING(ENABLE_LOG)
#define HTRACE(...) { ::hdn::Log_GetCoreLogger()->trace(__VA_ARGS__); }
#define HINFO(...)  { ::hdn::Log_GetCoreLogger()->info(__VA_ARGS__); }
#define HWARN(...)  { ::hdn::Log_GetCoreLogger()->warn(__VA_ARGS__); }
#define HERR(...)	{ ::hdn::Log_GetCoreLogger()->error(__VA_ARGS__); }
#define HCRIT(...)	{ ::hdn::Log_GetCoreLogger()->critical(__VA_ARGS__); }
#define HFATAL(...) { HCRIT(__VA_ARGS__); HBREAK(); }
#else
#define HTRACE(...)
#define HINFO(...)
#define HWARN(...)
#define HERR(...)
#define HCRIT(...)
#define HFATAL(...)
#endif

#if USING(ENABLE_ASSERT)
#define HASSERT(x, ...) { if(!(x)) { HFATAL(__VA_ARGS__); } }
#define HASSERT_PTR(x, ...) { if(x == nullptr) { HFATAL(__VA_ARGS__); } }
#define HASSERT_INDEX(min, index, max, ...) { if(!(index >= 0 && index <= max)) { HFATAL(__VA_ARGS__); } }
#else
#define HASSERT(...)
#define HASSERT_PTR(...)
#define HASSERT_INDEX(...)
#endif

#if USING(ENABLE_THROW)
#define HTHROW(exception, message) { throw exception(message); }
#else
#define HTHROW(exception, message) { HFATAL(message); }
#endif

// Compared to an assert and check will exist even in non debug build
#if !USING(DEV)
#define HCHECK(x)		{ if(!(x)) { HBREAK(); } }
#define HCHECK(x, ...)	HCHECK(x)
#else
#define HCHECK(x)		HASSERT(x);
#define HCHECK(x, ...)	HASSERT(x, __VA_ARGS__);
#endif