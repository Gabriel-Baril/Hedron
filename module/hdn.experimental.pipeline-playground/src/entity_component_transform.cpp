#include "entity_component_transform.h"

namespace hdn
{
	bool component_parse_transform(pugi::xml_node componentNode, EntityDef& entDef)
	{
		TransformComponent transformC;
		transformC.position.x = componentNode.child("Translation").attribute("x").as_float();
		transformC.position.y = componentNode.child("Translation").attribute("y").as_float();
		transformC.position.z = componentNode.child("Translation").attribute("z").as_float();

		transformC.rotation.x = componentNode.child("Rotation").attribute("x").as_float();
		transformC.rotation.y = componentNode.child("Rotation").attribute("y").as_float();
		transformC.rotation.z = componentNode.child("Rotation").attribute("z").as_float();

		transformC.scale.x = componentNode.child("Scale").attribute("x").as_float();
		transformC.scale.y = componentNode.child("Scale").attribute("y").as_float();
		transformC.scale.z = componentNode.child("Scale").attribute("z").as_float();
		entDef.add_component(transformC);

		return true;
	}
}