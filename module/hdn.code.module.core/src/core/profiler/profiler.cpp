#pragma once

#include "profiler.h"

#if USING( PERF_PROFILE )
namespace hdn
{
	struct Instrumentor
	{
		InstrumentationSession *CurrentSession = nullptr;
		std::ofstream OutputStream;
		int ProfileCount = 0;
	};

	static Instrumentor s_Instrumentor;

	static void Instrumentor_BeginSession_Internal( Instrumentor &instance, const std::string &name, const std::string &filepath )
	{
		instance.OutputStream.open( filepath );
		Instrumentor_WriteHeader();
		instance.CurrentSession = new InstrumentationSession{ name };
	}

	void Instrumentor_BeginSession( const std::string &name, const std::string &filepath )
	{
		Instrumentor_BeginSession_Internal( s_Instrumentor, name, filepath );
	}

	static void Instrumentor_EndSession_Internal( Instrumentor &instance )
	{
		Instrumentor_WriteFooter();
		instance.OutputStream.close();
		delete instance.CurrentSession;
		instance.CurrentSession = nullptr;
		instance.ProfileCount = 0;
	}

	void Instrumentor_EndSession()
	{
		Instrumentor_EndSession_Internal( s_Instrumentor );
	}

	static void Instrumentor_WriteProfile_Internal( Instrumentor &instance, const ProfileResult &result )
	{
		if ( instance.ProfileCount++ > 0 )
			instance.OutputStream << ",";

		std::string name = result.Name;
		std::replace( name.begin(), name.end(), '"', '\'' );

		instance.OutputStream << "{";
		instance.OutputStream << "\"cat\":\"function\",";
		instance.OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		instance.OutputStream << "\"name\":\"" << name << "\",";
		instance.OutputStream << "\"ph\":\"X\",";
		instance.OutputStream << "\"pid\":0,";
		instance.OutputStream << "\"tid\":" << result.ThreadID << ",";
		instance.OutputStream << "\"ts\":" << result.Start;
		instance.OutputStream << "}";

		instance.OutputStream.flush();
	}

	void Instrumentor_WriteProfile( const ProfileResult &result )
	{
		Instrumentor_WriteProfile_Internal( s_Instrumentor, result );
	}

	static void Instrumentor_WriteHeader_Internal( Instrumentor &instance )
	{
		instance.OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		instance.OutputStream.flush();
	}

	void Instrumentor_WriteHeader()
	{
		Instrumentor_WriteHeader_Internal( s_Instrumentor );
	}

	static void Instrumentor_WriteFooter_Internal( Instrumentor &instance )
	{
		instance.OutputStream << "]}";
		instance.OutputStream.flush();
	}

	void Instrumentor_WriteFooter()
	{
		Instrumentor_WriteFooter_Internal( s_Instrumentor );
	}

	InstrumentationTimer::InstrumentationTimer( const char *name )
		: m_Name( name ), m_Stopped( false )
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if ( !m_Stopped )
			this->Stop();
	}

	void InstrumentationTimer::Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		size_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor_WriteProfile( ProfileResult{ m_Name, start, end, threadID } );

		m_Stopped = true;
	}
}
#endif