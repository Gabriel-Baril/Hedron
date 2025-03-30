#pragma once

#include "core/core_define.h"
#include "core/core_type.h"
#include "core/core_internal_api.h"

namespace hdn
{
	class HDN_MODULE_CORE_API Timestep
	{
	public:
		Timestep(f32 time = 0.0f)
			: m_Time(time)
		{
		}

		f32 seconds() const { return m_Time; }
		f32 milliseconds() const { return m_Time * 1000; }

		operator f32() { return m_Time; }
	private:
		f32 m_Time;
	};
}