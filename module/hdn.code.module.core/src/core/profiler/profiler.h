#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

#include "core/core_define.h"

#if USING( PERF_PROFILE )
namespace hdn
{
	struct ProfileResult
	{
		const char *name;
		u64 start;
		u64 end;
		u64 threadID;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	void instrumentor_begin_session( const std::string &name, const std::string &filepath = "result.json" );
	void instrumentor_end_session();
	void instrumentor_write_profile( const ProfileResult &result );
	void instrumentor_write_header();
	void instrumentor_write_footer();

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer( const char *name );
		~InstrumentationTimer();
		void stop();
	private:
		const char *m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		bool m_Stopped;
	};

}

#define HDR_PROFILE_BEGIN_SESSION(name, filepath) ::hdn::instrumentor_begin_session(name, filepath);
#define HDR_PROFILE_END_SESSION() ::hdn::instrumentor_end_session();
#define HDR_PROFILE_SCOPE(name) ::hdn::InstrumentationTimer timer##__LINE__(name);
#define HDR_PROFILE_FUNCTION() HDR_PROFILE_SCOPE(__FUNCSIG__)
#else
#define HDR_PROFILE_BEGIN_SESSION(name, filepath)
#define HDR_PROFILE_END_SESSION()
#define HDR_PROFILE_SCOPE(name)
#define HDR_PROFILE_FUNCTION()
#endif