#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Core/CoreDefine.h"
#include "Core/CoreMacro.h"

namespace hdn
{
	void Log_Init();
	Ref<spdlog::logger>& Log_GetCoreLogger();
	Ref<spdlog::logger>& Log_GetClientLogger();
}

#if USING(ENABLE_LOG)
#define HDN_CORE_TRACE(...)    ::hdn::Log_GetCoreLogger()->trace(__VA_ARGS__);
#define HDN_CORE_INFO(...)     ::hdn::Log_GetCoreLogger()->info(__VA_ARGS__);
#define HDN_CORE_WARNING(...)  ::hdn::Log_GetCoreLogger()->warn(__VA_ARGS__);
#define HDN_CORE_ERROR(...)    ::hdn::Log_GetCoreLogger()->error(__VA_ARGS__);
#define HDN_CORE_CRITICAL(...) ::hdn::Log_GetCoreLogger()->critical(__VA_ARGS__);

#define HDN_TRACE(...)    ::hdn::Log_GetClientLogger()->trace(__VA_ARGS__);
#define HDN_INFO(...)     ::hdn::Log_GetClientLogger()->info(__VA_ARGS__);
#define HDN_WARNING(...)  ::hdn::Log_GetClientLogger()->warn(__VA_ARGS__);
#define HDN_ERROR(...)    ::hdn::Log_GetClientLogger()->error(__VA_ARGS__);
#define HDN_CRITICAL(...) ::hdn::Log_GetClientLogger()->critical(__VA_ARGS__);

#define HDN_CORE_VOID_TRACE(x, ...) if(!(x)) { HDN_CORE_TRACE(__VA_ARGS__); return; }
#define HDN_CORE_VOID_INFO(x, ...) if(!(x)) { HDN_CORE_INFO(__VA_ARGS__); return; }
#define HDN_CORE_VOID_WARNING(x, ...) if(!(x)) { HDN_CORE_WARNING(__VA_ARGS__); return; }
#define HDN_CORE_VOID_ERROR(x, ...) if(!(x)) { HDN_CORE_ERROR(__VA_ARGS__); return; }
#define HDN_CORE_VOID_CRITICAL(x, ...) if(!(x)) { HDN_CORE_CRITICAL(__VA_ARGS__); return; }

#define HDN_VOID_TRACE(x, ...) if(!(x)) { HDN_TRACE(__VA_ARGS__); return; }
#define HDN_VOID_INFO(x, ...) if(!(x)) { HDN_INFO(__VA_ARGS__); return; }
#define HDN_VOID_WARNING(x, ...) if(!(x)) { HDN_WARNING(__VA_ARGS__); return; }
#define HDN_VOID_ERROR(x, ...) if(!(x)) { HDN_ERROR(__VA_ARGS__); return; }
#define HDN_VOID_CRITICAL(x, ...) if(!(x)) { HDN_CRITICAL(__VA_ARGS__); return; }

#define HDN_CORE_RETURN_TRACE(x, returnvalue,...) if(!(x)) { HDN_CORE_TRACE(__VA_ARGS__); return returnvalue; }
#define HDN_CORE_RETURN_INFO(x, returnvalue,...) if(!(x)) { HDN_CORE_INFO(__VA_ARGS__); return returnvalue; }
#define HDN_CORE_RETURN_WARNING(x, returnvalue,...) if(!(x)) { HDN_CORE_WARNING(__VA_ARGS__); return returnvalue; }
#define HDN_CORE_RETURN_ERROR(x, returnvalue,...) if(!(x)) { HDN_CORE_ERROR(__VA_ARGS__); return returnvalue; }
#define HDN_CORE_RETURN_CRITICAL(x, returnvalue,...) if(!(x)) { HDN_CORE_CRITICAL(__VA_ARGS__); return returnvalue; }

#define HDN_RETURN_TRACE(x, returnvalue,...) if(!(x)) { HDN_TRACE(__VA_ARGS__); return returnvalue; }
#define HDN_RETURN_INFO(x, returnvalue,...) if(!(x)) { HDN_INFO(__VA_ARGS__); return returnvalue; }
#define HDN_RETURN_WARNING(x, returnvalue,...) if(!(x)) { HDN_WARNING(__VA_ARGS__); return returnvalue; }
#define HDN_RETURN_ERROR(x, returnvalue,...) if(!(x)) { HDN_ERROR(__VA_ARGS__); return returnvalue; }
#define HDN_RETURN_CRITICAL(x, returnvalue,...) if(!(x)) { HDN_CRITICAL(__VA_ARGS__); return returnvalue; }
#else
#define HDN_CORE_TRACE(...)
#define HDN_CORE_INFO(...)
#define HDN_CORE_WARNING(...)
#define HDN_CORE_ERROR(...)
#define HDN_CORE_CRITICAL(...)

#define HDN_TRACE(...)
#define HDN_INFO(...)
#define HDN_WARNING(...)
#define HDN_ERROR(...)
#define HDN_CRITICAL(...)

#define HDN_CORE_VOID_TRACE(x, ...)
#define HDN_CORE_VOID_INFO(x, ...) 
#define HDN_CORE_VOID_WARNING(x, ...)
#define HDN_CORE_VOID_ERROR(x, ...)
#define HDN_CORE_VOID_CRITICAL(x, ...)

#define HDN_VOID_TRACE(x, ...)
#define HDN_VOID_INFO(x, ...) 
#define HDN_VOID_WARNING(x, ...)
#define HDN_VOID_ERROR(x, ...)
#define HDN_VOID_CRITICAL(x, ...)

#define HDN_CORE_RETURN_TRACE(x, returnvalue,...)
#define HDN_CORE_RETURN_INFO(x, returnvalue,...)
#define HDN_CORE_RETURN_WARNING(x, returnvalue,...)
#define HDN_CORE_RETURN_ERROR(x, returnvalue,...)
#define HDN_CORE_RETURN_CRITICAL(x, returnvalue,...)

#define HDN_RETURN_TRACE(x, returnvalue,...)
#define HDN_RETURN_INFO(x, returnvalue,...)
#define HDN_RETURN_WARNING(x, returnvalue,...)
#define HDN_RETURN_ERROR(x, returnvalue,...)
#define HDN_RETURN_CRITICAL(x, returnvalue,...)
#endif