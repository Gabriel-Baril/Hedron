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
		~ImGuiLayer();

		virtual void on_attach() override;
		virtual void on_detach() override;

		void begin();
		void end();
	};
}