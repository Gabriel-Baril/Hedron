#include "native_script_component.h"

namespace dm
{

	void INativeScript::bind(flecs::entity e)
	{
		m_Entity = e;
	}

	flecs::entity INativeScript::get_gameobject()
	{
		return m_Entity;
	}
}
