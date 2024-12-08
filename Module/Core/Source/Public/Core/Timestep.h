#pragma once

#include "Core/CoreDefine.h"
#include "Core/CoreType.h"

namespace hdn
{
	class Timestep
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