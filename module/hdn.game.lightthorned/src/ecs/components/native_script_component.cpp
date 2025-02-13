#include "native_script_component.h"

#include "hdn_game_object.h"

namespace hdn
{

	void INativeScript::Bind(flecs::entity e)
	{
		m_Entity = e;
	}

	flecs::entity INativeScript::GetGameObject()
	{
		return m_Entity;
	}
}
