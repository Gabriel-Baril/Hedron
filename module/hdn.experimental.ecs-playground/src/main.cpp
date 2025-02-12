#include "core/core.h"
#include "core/core_filesystem.h"

#include <flecs/flecs.h>
#include "nlohmann/json.hpp"

struct Position { float x; float y; };
struct Health { int value; };
struct Damage { int value; };

using json = nlohmann::ordered_json;

json SerializeComponent(const Position& component) { return { {"x", component.x}, {"y", component.y} }; }
json SerializeComponent(const Health& component) { return { {"value", component.value} }; }
json SerializeComponent(const Damage& component) { return { {"value", component.value} }; }

Position DeserializePosition(const json& j) { return Position{ j["x"], j["y"] }; }
Health DeserializeHealth(const json& j) { return Health{ j["value"] }; }
Damage DeserializeDamage(const json& j) { return Damage{ j["value"] }; }

json SerializeEntity(flecs::entity e)
{
	json j;
	j["name"] = e.name();
	if (e.has<Position>()) j["position"] = SerializeComponent(*e.get<Position>());
	if (e.has<Health>()) j["health"] = SerializeComponent(*e.get<Health>());
	if (e.has<Damage>()) j["damage"] = SerializeComponent(*e.get<Damage>());
	j["children"] = json::array();
	e.children([&](flecs::entity child) {
		HINFO("TEST");
		j["children"].push_back(SerializeEntity(child));
	});
	return j;
}

flecs::entity DeserializeEntity(flecs::world& ecs, const json& j, flecs::entity parent = flecs::entity())
{
	auto e = ecs.prefab(j["name"].get<std::string>().c_str());
	if (j.contains("position")) e.set(DeserializePosition(j["position"]));
	if (j.contains("health")) e.set(DeserializeHealth(j["health"]));
	if (j.contains("damage")) e.set(DeserializeDamage(j["damage"]));

	if (j.contains("children"))
	{
		for (const auto& childJson : j["children"])
		{
			auto child = DeserializeEntity(ecs, childJson, e);
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
	json savedData = SerializeEntity(playerPrefab);
	std::ofstream("prefab_data.json") << savedData.dump(4);
}

void example_deserialize()
{
	flecs::world ecs;
	ecs.set_threads(std::thread::hardware_concurrency());
	json savedData;
	std::ifstream("prefab_data.json") >> savedData;
	auto loadedPrefab = DeserializeEntity(ecs, savedData);
}

int main()
{
	using namespace hdn;
	Log_Init();
	
	example_deserialize();

	return 0;
}