#pragma once

// To visualize the json file outputted you need to go on this site : "chrome://tracing"

#define HDR_PROFILE 0
#if HDR_PROFILE

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Hedron
{
	struct ProfileResult
	{
		const char* name;
		long long start;
		long long end;
		uint32_t threadID;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_currentSession;
		std::ofstream m_outputStream;
		int m_profileCount;

	public:
		Instrumentor()
			: m_currentSession(nullptr), m_profileCount(0)
		{
		}

		void begin_session(const std::string& name, const std::string& filePath = "result.json")
		{
			m_outputStream.open(filePath);
			write_header();
			m_currentSession = new InstrumentationSession{ name };
		}

		void end_session()
		{
			write_footer();
			m_outputStream.close();
			delete m_currentSession;
			m_currentSession = nullptr;
			m_profileCount = 0;
		}

		void write_profile(const ProfileResult& result)
		{
			if (m_profileCount++ > 0)
				m_outputStream << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_outputStream << "{";
			m_outputStream << "\"cat\":\"function\",";
			m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
			m_outputStream << "\"name\":\"" << name << "\",";
			m_outputStream << "\"ph\":\"X\",";
			m_outputStream << "\"pid\":0,";
			m_outputStream << "\"tid\":" << result.threadID << ",";
			m_outputStream << "\"ts\":" << result.start;
			m_outputStream << "}";

			m_outputStream.flush();
		}

		void write_header()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_outputStream.flush();
		}

		void write_footer()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		static Instrumentor& get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_startPointTime = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
				this->stop();
		}

		void stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPointTime).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::get().write_profile({ m_name, start, end, threadID });
			
			m_stopped = true;
		}
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock> m_startPointTime;
		bool m_stopped;
	};

}

#define HDR_PROFILE_BEGIN_SESSION(name, filePath) ::Hedron::Instrumentor::get().begin_session(name, filePath);
#define HDR_PROFILE_END_SESSION() ::Hedron::Instrumentor::get().end_session();
#define HDR_PROFILE_SCOPE(name) ::Hedron::InstrumentationTimer timer##__LINE__(name);
#define HDR_PROFILE_FUNCTION() HDR_PROFILE_SCOPE(__FUNCSIG__)
#else
#define HDR_PROFILE_BEGIN_SESSION(name, filePath)
#define HDR_PROFILE_END_SESSION()
#define HDR_PROFILE_SCOPE(name)
#define HDR_PROFILE_FUNCTION()
#endif