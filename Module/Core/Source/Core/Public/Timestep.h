#pragma once

#include "core/core_defines.h"
#include "core/core_type.h"

namespace hedron
{
	class HEDRON_API Timestep
	{
	public:
		Timestep(float32 time = 0.0f)
			: m_Time(time)
		{
		}

		float32 Seconds() const { return m_Time; }
		float32 Milliseconds() const { return m_Time * 1000; }

		operator float32() { return m_Time; }
	private:
		float32 m_Time;
	};
}