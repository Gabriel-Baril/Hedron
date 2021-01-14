#include "hdrpch.h"
#include "Entity.h"

namespace Hedron
{
	Hedron::Entity::Entity(entt::entity handle, Scene* scene)
		: m_entityHandle(handle), m_scene(scene)
	{
	}
}

