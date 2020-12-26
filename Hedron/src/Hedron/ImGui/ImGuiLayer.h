#pragma once

#include "Hedron/Layer.h"

namespace Hedron
{
	class HEDRON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void on_attach();
		void on_detach();
		void on_update();
		void on_event(Event& event);
	private:
		float m_time = 0.0f;
	};
}