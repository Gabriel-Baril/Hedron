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
		virtual const char* Name() { return "INativeScript"; }
		virtual void OnUpdate(float dt) = 0;
		
		void Bind(flecs::entity e);
		flecs::entity GetGameObject();
	protected:
		flecs::entity m_Entity;
	};

	struct NativeScriptComponent
	{
		vector<INativeScript*> scripts;

		void Add(INativeScript* script)
		{
			scripts.push_back(script);
		}

		void Update(float dt)
		{
			for (const auto& script : scripts)
			{
				script->OnUpdate(dt);
			}
		}

		~NativeScriptComponent()
		{
			for (const auto& script : scripts)
			{
				HINFO("Freed script: '{0}'", script->Name());
				delete script;
			}
		}
	};
}