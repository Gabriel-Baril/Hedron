#include "scene.h"

#include "ecs/components/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"

namespace dm
{
	Scene::Scene()
	{
		m_Registry.set_threads(std::thread::hardware_concurrency());
	}

	Entity Scene::make_point_light(f32 intensity, f32 radius, vec3f32 color)
	{
		Entity gameObj = create();

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
