#pragma once
#include "core/core.h"
#include "core/stl/vector.h"

#include "flecs/flecs.h"

namespace hdn
{
	class HDNGameObject;

	class INativeScript
	{
	public:
		virtual const char* name() { return "INativeScript"; }
		virtual void on_update(float dt) = 0;
		
		void bind(flecs::entity e);
		flecs::entity get_gameobject();
	protected:
		flecs::entity m_Entity;
	};

	struct NativeScriptComponent
	{
		vector<INativeScript*> scripts;

		void add(INativeScript* script)
		{
			scripts.push_back(script);
		}

		void update(float dt)
		{
			for (const auto& script : scripts)
			{
				script->on_update(dt);
			}
		}

		~NativeScriptComponent()
		{
			for (const auto& script : scripts)
			{
				HINFO("Freed script: '{0}'", script->name());
				delete script;
			}
		}
	};
}