#include "gameobject.h"
#include "hobj/scene/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"

namespace hdn
{
	HDNGameObject HDNGameObject::make_point_light(flecs::world& ecs, f32 intensity, f32 radius, vec3f32 color)
	{
		HDNGameObject gameObj = HDNGameObject::create_game_object(ecs);
		
		TransformComponent transformC{};
		transformC.scale.x = radius;
		gameObj.set(transformC);

		ColorComponent colorC{};
		colorC.color = color;
		gameObj.set(colorC);

		PointLightComponent pointLightC;
		pointLightC.lightIntensity = intensity;
		gameObj.set(pointLightC);

		return gameObj;
	}
}