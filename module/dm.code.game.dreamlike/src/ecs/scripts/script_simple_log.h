#pragma once

#include "ecs/components/native_script_component.h"

namespace dm
{
	class SimpleLogScript : public INativeScript
	{
		virtual const char *name()
		{
			return "SimpleLogScript";
		}

		virtual void on_update(float dt) override
		{
			DM_WARNING_LOG("SimpleLogScript");
		}
	};
}
