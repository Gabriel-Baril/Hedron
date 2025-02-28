#pragma once

#include "transform_component.h"

#include "flecs/flecs.h"

namespace hdn
{
	void inspector_component_render(TransformComponent& component);

	// template<typename Component>
	void RenderWithHeader(flecs::entity& entity, const char* controlLabel);
}