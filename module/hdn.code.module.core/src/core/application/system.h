#pragma once

#include "core/event/event.h"

namespace hdn
{
	// 
	class ISystem
	{
	public:
		virtual void init() {};
		virtual void update() {};
		virtual void shutdown() {};
		virtual void on_event(Event& event) {}
	private:
		// System dependencies
	};
}