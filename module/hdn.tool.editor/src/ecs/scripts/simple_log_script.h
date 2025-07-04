#pragma once

#include "ecs/components/native_script_component.h"

namespace hdn
{
	class SimpleLogScript : public INativeScript
	{
		virtual const char* Name()
		{ 
			return "SimpleLogScript";
		}

		virtual void on_update(float dt) override
		{
			// HWARN("SimpleLogScript");
		}
	};
}