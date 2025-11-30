#pragma once

#include "core/core.h"
#include "core/event/event.h"

namespace dm
{
	class IApplicationLayer
	{
	public:
		virtual void begin() = 0;
		virtual void on_update(float dt) = 0;
		virtual void end() = 0;

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_event(Event &event) {}

		inline void enable()
		{
			m_Enabled = true;
		}

		inline void disable()
		{
			m_Enabled = false;
		}

		inline bool enabled()
		{
			return m_Enabled;
		}

	private:
		bool m_Enabled = true;
		int m_LayerID = 0;
		int m_LayerType = 0;
		string m_Name;
	};
}
