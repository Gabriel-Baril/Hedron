#include "hdrpch.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Hedron/Core/Log.h"

namespace Hedron
{
	Ref<spdlog::logger> Log::s_coreLogger;
	Ref<spdlog::logger> Log::s_clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // Timestamp + logger name + message
		s_coreLogger = spdlog::stdout_color_mt("HEDRON");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);
	}

}
