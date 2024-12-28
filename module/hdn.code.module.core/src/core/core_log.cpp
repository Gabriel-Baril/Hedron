#include "core/core_log.h"

#if USING(LOG_ENABLE)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "core/core_macro.h"

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

	void SignalHandlerCallback(int signal) {
		HWARN("Received signal: {}", signal);
		spdlog::shutdown();  // Flush and clean up
		std::exit(signal);   // Exit gracefully
	}

#if USING(HDN_PLATFORM_WINDOWS)
	BOOL WINAPI ConsoleHandlerCallback(DWORD signal) {
		if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT) {
			HWARN("Console is closing or interrupted!");
			spdlog::shutdown();  // Flush logs before termination
		}
		return TRUE;  // Indicate signal was handled
	}
#endif

	Ref<spdlog::logger>& Log_GetCoreLogger()
	{
		return s_CoreLogger;
	}
#endif


	void Log_Init()
	{
#if USING(LOG_ENABLE)
		try
		{
			std::signal(SIGINT, SignalHandlerCallback);
			std::signal(SIGTERM, SignalHandlerCallback);
			std::signal(SIGABRT, SignalHandlerCallback);
#if USING(HDN_PLATFORM_WINDOWS)
			SetConsoleCtrlHandler(ConsoleHandlerCallback, TRUE);
#endif

			TVector<spdlog::sink_ptr> sinks;

#if USING(LOG_CONSOLE_ENABLE)
			auto consoleSink = CreateRef<spdlog::sinks::stdout_color_sink_mt>();
			consoleSink->set_level(spdlog::level::trace);
			auto formatter = std::make_unique<spdlog::pattern_formatter>();
			formatter->add_flag<CustomLevelFormatter>('L')  // Custom level formatter
				.set_pattern("\033[90m[%n]\033[0m %^[%L] %v%$");      // Console pattern: [<logger_name>] [<severity>] <message>
			consoleSink->set_formatter(std::move(formatter));
			sinks.push_back(consoleSink);
#endif

#if USING(LOG_FILE_ENABLE)

			auto fileSink = CreateRef<spdlog::sinks::basic_file_sink_mt>("console.log", true); // The true make sure we append to the file instead of overwriting it
			fileSink->set_level(spdlog::level::trace);
			auto file_formatter = std::make_unique<spdlog::pattern_formatter>();
			file_formatter->add_flag<CustomLevelFormatter>('L')  // Custom level formatter
				.set_pattern("[%n] [%Y-%m-%d %H:%M:%S.%e] [PID:%P] [TID:%t] [%L] [%s:%!(%#)] %v");  // File pattern
			fileSink->set_formatter(std::move(file_formatter));
			sinks.push_back(fileSink);
#endif

			s_CoreLogger = CreateRef<spdlog::logger>("HDN", sinks.begin(), sinks.end());
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
