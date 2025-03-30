#include "core/core_log.h"

#if USING(LOG_ENABLE)
#include "core/core_macro.h"
#include "core/stl/vector.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <iostream>
#include <csignal>

#if USING(HDN_PLATFORM_WINDOWS)
#include <windows.h>
#endif
#endif

namespace hdn
{
#if USING(LOG_ENABLE)
	static Ref<spdlog::logger> s_CoreLogger;

	class CustomLevelFormatter : public spdlog::custom_flag_formatter {
	public:
		void format(const spdlog::details::log_msg& msg, const std::tm& tm_time, spdlog::memory_buf_t& dest) override {
			MAYBE_UNUSED(tm_time);
			const char* level_map[] = { "TRACE", "DEBUG", "INFO ", "WARN ", "ERR  ", "CRIT ", "OFF  " }; // Make sure each level have 5 characters to make the messages aligned in the console
			auto level = static_cast<size_t>(msg.level);
			if (level < sizeof(level_map) / sizeof(level_map[0])) {
				spdlog::fmt_lib::format_to(std::back_inserter(dest), "{}", level_map[level]);
			}
		}

		std::unique_ptr<spdlog::custom_flag_formatter> clone() const override {
			return spdlog::details::make_unique<CustomLevelFormatter>();
		}
	};

	void signal_handler_callback(int signal) {
		HWARN("Received signal: {}", signal);
		spdlog::shutdown();  // Flush and clean up
		std::exit(signal);   // Exit gracefully
	}

#if USING(HDN_PLATFORM_WINDOWS)
	BOOL WINAPI console_handler_callback(DWORD signal) {
		if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT) {
			HWARN("Console is closing or interrupted!");
			spdlog::shutdown();  // Flush logs before termination
		}
		return TRUE;  // Indicate signal was handled
	}
#endif

	Ref<spdlog::logger>& log_get_core_logger()
	{
		return s_CoreLogger;
	}
#endif


	void log_init()
	{
#if USING(LOG_ENABLE)
		try
		{
			std::signal(SIGINT, signal_handler_callback);
			std::signal(SIGTERM, signal_handler_callback);
			std::signal(SIGABRT, signal_handler_callback);
#if USING(HDN_PLATFORM_WINDOWS)
			SetConsoleCtrlHandler(console_handler_callback, TRUE);
#endif

			vector<spdlog::sink_ptr> sinks;

#if USING(LOG_CONSOLE_ENABLE)
			auto consoleSink = make_ref<spdlog::sinks::stdout_color_sink_mt>();
			consoleSink->set_level(spdlog::level::trace);
			auto formatter = std::make_unique<spdlog::pattern_formatter>();
			formatter->add_flag<CustomLevelFormatter>('L')  // Custom level formatter
				.set_pattern("\033[90m[%n]\033[0m %^[%L] %v%$");      // Console pattern: [<logger_name>] [<severity>] <message>
			consoleSink->set_formatter(std::move(formatter));
			sinks.push_back(consoleSink);
#endif

#if USING(LOG_FILE_ENABLE)

			auto fileSink = make_ref<spdlog::sinks::basic_file_sink_mt>("console.log", true); // The true make sure we append to the file instead of overwriting it
			fileSink->set_level(spdlog::level::trace);
			auto file_formatter = std::make_unique<spdlog::pattern_formatter>();
			file_formatter->add_flag<CustomLevelFormatter>('L')  // Custom level formatter
				.set_pattern("[%n] [%Y-%m-%d %H:%M:%S.%e] [PID:%P] [TID:%t] [%L] [%s:%!(%#)] %v");  // File pattern
			fileSink->set_formatter(std::move(file_formatter));
			sinks.push_back(fileSink);
#endif

			s_CoreLogger = make_ref<spdlog::logger>("HDN", sinks.begin(), sinks.end());
			s_CoreLogger->set_level(spdlog::level::trace);

			spdlog::set_default_logger(s_CoreLogger);
			spdlog::set_level(spdlog::level::trace);
#if USING(LOG_ALWAYS_FLUSH)
			spdlog::flush_on(spdlog::level::trace);
#endif
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cerr << "Spdlog Initialization Failed" << ex.what() << std::endl;
		}
#endif
	}
}
