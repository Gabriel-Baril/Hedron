#include <spdlog/sinks/stdout_color_sinks.h>
#include "core/CoreLog.h"

#if USING(ENABLE_LOG)
namespace hdn
{
	static Ref<spdlog::logger> s_CoreLogger;

	void Log_Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // Timestamp + logger name + message
		s_CoreLogger = spdlog::stdout_color_mt("HDN");
		s_CoreLogger->set_level(spdlog::level::trace);
	}

	Ref<spdlog::logger>& Log_GetCoreLogger()
	{
		return s_CoreLogger;
	}
}
#endif
