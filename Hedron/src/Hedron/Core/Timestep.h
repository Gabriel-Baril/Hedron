#pragma once

namespace Hedron
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_time(time)
		{
		}

		float get_seconds() const { return m_time; }
		float get_milliseconds() const { return m_time * 1000; }

		operator float() { return m_time; }
	private:
		float m_time;
	};
}