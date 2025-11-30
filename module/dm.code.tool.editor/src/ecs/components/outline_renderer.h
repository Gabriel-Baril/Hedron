#pragma once

#include "transform_component.h"

#include "flecs/flecs.h"

namespace dm
{
	void inspector_component_render(TransformComponent &component);

	// template<typename Component>
	void render_with_header(flecs::entity &entity, const char *controlLabel);
}
