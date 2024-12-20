#include <spdlog/sinks/stdout_color_sinks.h>
#include "Core/CoreLog.h"

#if USING(ENABLE_LOG)
namespace hdn
{
	static Ref<spdlog::logger> s_CoreLogger;
	static Ref<spdlog::logger> s_ClientLogger;

	void Log_Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // Timestamp + logger name + message
		s_CoreLogger = spdlog::stdout_color_mt("HDN");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	Ref<spdlog::logger>& Log_GetCoreLogger()
	{
		return s_CoreLogger;
	}

	Ref<spdlog::logger>& Log_GetClientLogger()
	{
		return s_ClientLogger;
	}
}
#endif
