#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

#if USING( PERF_PROFILE )
namespace hdn
{
	struct ProfileResult
	{
		const char *Name;
		long long Start;
		long long End;
		size_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	void Instrumentor_BeginSession( const std::string &name, const std::string &filepath = "result.json" );
	void Instrumentor_EndSession();
	void Instrumentor_WriteProfile( const ProfileResult &result );
	void Instrumentor_WriteHeader();
	void Instrumentor_WriteFooter();

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer( const char *name );
		~InstrumentationTimer();
		void Stop();
	private:
		const char *m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		bool m_Stopped;
	};

}

#define HDR_PROFILE_BEGIN_SESSION(name, filepath) ::hedron::Instrumentor_BeginSession(name, filepath);
#define HDR_PROFILE_END_SESSION() ::hedron::Instrumentor_EndSession();
#define HDR_PROFILE_SCOPE(name) ::hedron::InstrumentationTimer timer##__LINE__(name);
#define HDR_PROFILE_FUNCTION() HDR_PROFILE_SCOPE(__FUNCSIG__)
#else
#define HDR_PROFILE_BEGIN_SESSION(name, filepath)
#define HDR_PROFILE_END_SESSION()
#define HDR_PROFILE_SCOPE(name)
#define HDR_PROFILE_FUNCTION()
#endif