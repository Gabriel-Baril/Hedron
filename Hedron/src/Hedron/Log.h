#pragma once

#include <memory>

#include "Core.h"
#include <spdlog/spdlog.h>
#include  "spdlog/fmt/ostr.h"

namespace Hedron
{
	// Wrapper class for logging
	class HEDRON_API Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// TODO: Make sure to set those macros to nothing for the distribution build 
// Core log macros
#define HDR_CORE_TRACE(...)    ::Hedron::Log::getCoreLogger()->trace(__VA_ARGS__);
#define HDR_CORE_INFO(...)     ::Hedron::Log::getCoreLogger()->info(__VA_ARGS__);
#define HDR_CORE_WARNING(...)  ::Hedron::Log::getCoreLogger()->warn(__VA_ARGS__);
#define HDR_CORE_ERROR(...)    ::Hedron::Log::getCoreLogger()->error(__VA_ARGS__);
#define HDR_CORE_CRITICAL(...) ::Hedron::Log::getCoreLogger()->critical(__VA_ARGS__);

// Client log macros
#define HDR_TRACE(...)    ::Hedron::Log::getClientLogger()->trace(__VA_ARGS__);
#define HDR_INFO(...)     ::Hedron::Log::getClientLogger()->info(__VA_ARGS__);
#define HDR_WARNING(...)  ::Hedron::Log::getClientLogger()->warn(__VA_ARGS__);
#define HDR_ERROR(...)    ::Hedron::Log::getClientLogger()->error(__VA_ARGS__);
#define HDR_CRITICAL(...) ::Hedron::Log::getClientLogger()->critical(__VA_ARGS__);