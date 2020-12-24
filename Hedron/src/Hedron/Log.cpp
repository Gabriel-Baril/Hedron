#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Hedron
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // Timestamp + logger name + message
		s_CoreLogger = spdlog::stdout_color_mt("HEDRON");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
