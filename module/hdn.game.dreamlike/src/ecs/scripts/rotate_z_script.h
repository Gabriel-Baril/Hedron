#pragma once

#include "ecs/components/native_script_component.h"
#include "hobj/scene/transform_component.h"

#include "../../gameobject.h"

#include <glm/gtc/constants.hpp>

namespace hdn
{
	class RotateZScript : public INativeScript
	{
		virtual const char* Name()
		{
			return "RotateZScript";
		}

		virtual void OnUpdate(float dt) override
		{
			TransformComponent* transformC = m_Entity.get_mut<TransformComponent>();
			transformC->rotation.z += dt;
			HWARN("RotateZScript: {0}", transformC->rotation.z);
		}
	};
}