#pragma once

#include "Hedron/Core.h"
#include "Hedron/Events/Event.h"
#include "Hedron/Core/Timestep.h"

namespace Hedron
{
	class HEDRON_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(Timestep ts) {}
		virtual void on_imgui_render() {}
		virtual void on_event(Event& event) {}
	
		inline const std::string& get_name() const { return m_debugName; }
	protected:
		std::string m_debugName;
	};
}