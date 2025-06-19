#pragma once

#include "systems/system_physics_world.h"

#include "flecs/flecs.h"
#include "frame_info.h"
#include "core/event/events.h"

#include "systems/system_simple_render.h"
#include "systems/system_point_light.h"
#include "systems/system_physics_gameobject.h"
#include "systems/system_update_transform.h"
#include "systems/system_update_script.h"
#include "systems/system_physics_world.h"

namespace hdn
{
	class RuntimeScene
	{
	public:
		RuntimeScene();

		void init();

		bool on_window_resized(WindowResizedEvent& event);
		void load_example_scene();
		void update(FrameInfo& frameInfo);
		void render(FrameInfo& frameInfo);
		Entity create_entity(const char* name = nullptr);
	private:
		flecs::world m_EcsWorld;

		Ref<UpdateTransformSystem> m_UpdateTransformSystem;
		Ref<UpdateScriptSystem> m_UpdateScriptSystem;
		Ref<SimpleRenderSystem> m_SimpleRenderSystem;
		Ref<PointLightSystem> m_PointLightSystem;
		Ref<PhysicsGameObjectSystem> m_PhysicsGameObjectSystem;
		Ref<PhysicsWorldSystem> m_PhysicsWorldSystem;
	};
}