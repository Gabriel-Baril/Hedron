#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Hedron
{
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // Timestamp + logger name + message
		s_coreLogger = spdlog::stdout_color_mt("HEDRON");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);
	}

}
