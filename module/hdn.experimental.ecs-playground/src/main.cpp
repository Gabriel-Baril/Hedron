#include "core/core.h"
#include "core/core_filesystem.h"

#include <flecs/flecs.h>
#include "nlohmann/json.hpp"

struct Position { float x; float y; };
struct Health { int value; };
struct Damage { int value; };

using json = nlohmann::ordered_json;

json serialize_component(const Position& component) { return { {"x", component.x}, {"y", component.y} }; }
json serialize_component(const Health& component) { return { {"value", component.value} }; }
json serialize_component(const Damage& component) { return { {"value", component.value} }; }

Position deserialize_position(const json& j) { return Position{ j["x"], j["y"] }; }
Health deserialize_health(const json& j) { return Health{ j["value"] }; }
Damage deserialize_damage(const json& j) { return Damage{ j["value"] }; }

json serialize_entity(flecs::entity e)
{
	json j;
	j["name"] = e.name();
	if (e.has<Position>()) j["position"] = serialize_component(*e.get<Position>());
	if (e.has<Health>()) j["health"] = serialize_component(*e.get<Health>());
	if (e.has<Damage>()) j["damage"] = serialize_component(*e.get<Damage>());
	j["children"] = json::array();
	e.children([&](flecs::entity child) {
		j["children"].push_back(serialize_entity(child));
	});
	return j;
}

flecs::entity deserialize_entity(flecs::world& ecs, const json& j, flecs::entity parent = flecs::entity())
{
	auto e = ecs.prefab(j["name"].get<std::string>().c_str());
	if (j.contains("position")) e.set(deserialize_position(j["position"]));
	if (j.contains("health")) e.set(deserialize_health(j["health"]));
	if (j.contains("damage")) e.set(deserialize_damage(j["damage"]));
	if (j.contains("children"))
	{
		for (const auto& childJson : j["children"])
		{
			auto child = deserialize_entity(ecs, childJson, e);
			child.child_of(e);
		}
	}
	return e;
}

void example_serialize()
{
	flecs::world ecs;
	ecs.set_threads(std::thread::hardware_concurrency());
	auto playerPrefab = ecs.prefab("Player").set(Position{ 0, 0 }).set(Health{ 100 });
	auto weaponPrefab = ecs.prefab("Weapon").set(Damage{ 10 }).child_of(playerPrefab);
	auto attachmentPrefab = ecs.prefab("Scope").child_of(weaponPrefab);
	json savedData = serialize_entity(playerPrefab);
	std::ofstream("prefab_data.json") << savedData.dump(4);
}

void example_deserialize()
{
	flecs::world ecs;
	ecs.set_threads(std::thread::hardware_concurrency());
	json savedData;
	std::ifstream("prefab_data.json") >> savedData;
	auto loadedPrefab = deserialize_entity(ecs, savedData);
}

int main()
{
	using namespace hdn;
	Log_Init();
	
	example_deserialize();

	return 0;
}