#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Hedron/Core/Core.h"

namespace Hedron
{
	// Wrapper class for logging
	class HEDRON_API Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& get_core_logger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& get_client_logger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

// TODO: Make sure to set those macros to nothing for the distribution build 
// Core log macros
#define HDR_CORE_TRACE(...)    ::Hedron::Log::get_core_logger()->trace(__VA_ARGS__);
#define HDR_CORE_INFO(...)     ::Hedron::Log::get_core_logger()->info(__VA_ARGS__);
#define HDR_CORE_WARNING(...)  ::Hedron::Log::get_core_logger()->warn(__VA_ARGS__);
#define HDR_CORE_ERROR(...)    ::Hedron::Log::get_core_logger()->error(__VA_ARGS__);
#define HDR_CORE_CRITICAL(...) ::Hedron::Log::get_core_logger()->critical(__VA_ARGS__);

// Client log macros
#define HDR_TRACE(...)    ::Hedron::Log::get_client_logger()->trace(__VA_ARGS__);
#define HDR_INFO(...)     ::Hedron::Log::get_client_logger()->info(__VA_ARGS__);
#define HDR_WARNING(...)  ::Hedron::Log::get_client_logger()->warn(__VA_ARGS__);
#define HDR_ERROR(...)    ::Hedron::Log::get_client_logger()->error(__VA_ARGS__);
#define HDR_CRITICAL(...) ::Hedron::Log::get_client_logger()->critical(__VA_ARGS__);