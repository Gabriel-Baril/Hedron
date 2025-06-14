#include "gameobject.h"
#include "hobj/scene/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"

namespace hdn
{
	HDNGameObject HDNGameObject::MakePointLight(flecs::world& ecs, f32 intensity, f32 radius, vec3f32 color)
	{
		HDNGameObject gameObj = HDNGameObject::CreateGameObject(ecs);
		
		TransformComponent transformC{};
		transformC.scale.x = radius;
		gameObj.Set(transformC);

		ColorComponent colorC{};
		colorC.color = color;
		gameObj.Set(colorC);

		PointLightComponent pointLightC;
		pointLightC.lightIntensity = intensity;
		gameObj.Set(pointLightC);

		return gameObj;
	}
}