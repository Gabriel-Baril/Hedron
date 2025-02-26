#pragma once

#include "core/core.h"
#include "core/event/event.h"

namespace hdn
{
	class IApplicationLayer
	{
	public:
		virtual void Begin() = 0;
		virtual void OnUpdate( float dt ) = 0;
		virtual void End() = 0;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}

		inline void Enable()
		{
			m_Enabled = true;
		}

		inline void Disable()
		{
			m_Enabled = false;
		}

		inline bool Enabled()
		{
			return m_Enabled;
		}
	private:
		bool m_Enabled = true;
		int m_LayerID = 0;
		int m_LayerType = 0;
		string name;
	};
}