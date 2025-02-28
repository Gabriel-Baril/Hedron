#pragma once

#include "core/core.h"
#include "component_base.h"

namespace hdn
{
	struct TransformComponent : public ComponentBase
	{
		bool HasTranslationChanged = false; // True if the position has changed in the current frame, false otherwise
		bool HasRotationChanged = false; // True if the rotation has changed in the current frame, false otherwise
		bool HasScaleChanged = false; // True if the scale has changed in the current frame, false otherwise

		vec3f32 translation{};
		vec3f32 scale{ 1.0f, 1.0f, 1.0f };
		vec3f32 rotation{}; // In radians (Euler YXZ)

		mat4f32 worldMatrix;

		mat4f32 Mat4() const;
		mat3f32 NormalMatrix() const;
		bool Changed() const;
		void ClearChange();
	};
}
