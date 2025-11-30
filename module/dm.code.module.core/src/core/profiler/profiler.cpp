#pragma once

#include "profiler.h"

#if USING(DM_PERF_PROFILE)
namespace dm
{
	struct Instrumentor
	{
		InstrumentationSession *currentSession = nullptr;
		std::ofstream outputStream;
		i32 profileCount = 0;
	};

	static Instrumentor s_Instrumentor;

	static void instrumentor_begin_session_internal(Instrumentor &instance, const std::string &name, const std::string &filepath)
	{
		instance.outputStream.open(filepath);
		instrumentor_write_header();
		instance.currentSession = new InstrumentationSession{name};
	}

	void instrumentor_begin_session(const std::string &name, const std::string &filepath)
	{
		instrumentor_begin_session_internal(s_Instrumentor, name, filepath);
	}

	static void instrumentor_end_session_internal(Instrumentor &instance)
	{
		instrumentor_write_footer();
		instance.outputStream.close();
		delete instance.currentSession;
		instance.currentSession = nullptr;
		instance.profileCount = 0;
	}

	void instrumentor_end_session()
	{
		instrumentor_end_session_internal(s_Instrumentor);
	}

	static void instrumentor_write_profile_internal(Instrumentor &instance, const ProfileResult &result)
	{
		if (instance.profileCount++ > 0)
			instance.outputStream << ",";

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		instance.outputStream << "{";
		instance.outputStream << "\"cat\":\"function\",";
		instance.outputStream << "\"dur\":" << (result.end - result.start) << ',';
		instance.outputStream << "\"name\":\"" << name << "\",";
		instance.outputStream << "\"ph\":\"X\",";
		instance.outputStream << "\"pid\":0,";
		instance.outputStream << "\"tid\":" << result.threadID << ",";
		instance.outputStream << "\"ts\":" << result.start;
		instance.outputStream << "}";

		instance.outputStream.flush();
	}

	void instrumentor_write_profile(const ProfileResult &result)
	{
		instrumentor_write_profile_internal(s_Instrumentor, result);
	}

	static void instrumentor_write_header_internal(Instrumentor &instance)
	{
		instance.outputStream << "{\"otherData\": {},\"traceEvents\":[";
		instance.outputStream.flush();
	}

	void instrumentor_write_header()
	{
		instrumentor_write_header_internal(s_Instrumentor);
	}

	static void instrumentor_write_footer_internal(Instrumentor &instance)
	{
		instance.outputStream << "]}";
		instance.outputStream.flush();
	}

	void instrumentor_write_footer()
	{
		instrumentor_write_footer_internal(s_Instrumentor);
	}

	InstrumentationTimer::InstrumentationTimer(const char *name)
			: m_Name(name), m_Stopped(false)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!m_Stopped)
			this->stop();
	}

	void InstrumentationTimer::stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		size_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		instrumentor_write_profile(ProfileResult{m_Name, start, end, threadID});

		m_Stopped = true;
	}
}
#endif
