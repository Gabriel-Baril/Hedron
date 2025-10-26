#pragma once

#include "ecs/components/native_script_component.h"
#include "ecs/components/transform_component.h"

#include <glm/gtc/constants.hpp>

namespace hdn
{
	class RotateZScript : public INativeScript
	{
		virtual const char* name()
		{
			return "RotateZScript";
		}

		virtual void on_update(float dt) override
		{
			TransformComponent* transformC = m_Entity.get_mut<TransformComponent>();
			transformC->rotation.z += dt;
			HWARN("RotateZScript: {0}", transformC->rotation.z);
		}
	};
}