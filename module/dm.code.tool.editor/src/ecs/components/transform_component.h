#pragma once

#include "core/core.h"
#include "component_base.h"

namespace dm
{
	struct TransformComponent : public ComponentBase
	{
		bool HasTranslationChanged = false; // True if the position has changed in the current frame, false otherwise
		bool HasRotationChanged = false;		// True if the rotation has changed in the current frame, false otherwise
		bool HasScaleChanged = false;				// True if the scale has changed in the current frame, false otherwise

		vec3f32 position{};
		vec3f32 scale{1.0f, 1.0f, 1.0f};
		vec3f32 rotation{}; // In radians (Euler YXZ)

		mat4f32 worldMatrix;

		mat4f32 to_mat() const;
		mat3f32 normal_matrix() const;
		bool changed() const;
		void clear_change();
	};
}
