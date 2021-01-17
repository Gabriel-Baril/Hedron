#pragma once

#include "Hedron/Events/ApplicationEvents.h"
#include "Hedron/Events/KeyboardEvents.h"
#include "Hedron/Events/MouseEvents.h"
#include "Hedron/Core/Layer.h"

namespace Hedron
{
	class HEDRON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_event(Event& event) override;

		void begin();
		void end();

		void block_events(bool blockImGuiEvents) { m_blockImGuiEvents = blockImGuiEvents; }

		void set_dark_theme_colors();
	private:
		bool m_blockImGuiEvents = true;
		float m_time = 0.0f;
	};
}