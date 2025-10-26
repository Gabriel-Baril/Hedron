#include "ideation_parser.h"

#include "nlohmann/json.hpp"

namespace hdn
{
	using json = nlohmann::json;

	void IdeationParser::parse_ideation_file(const string& path, Ideation& ideation)
	{
		std::ifstream file(path);
		json jsonData;
		file >> jsonData;

		// Access the "Nodes" array
		if (jsonData.contains("Nodes")) {
			const auto& nodes = jsonData["Nodes"];

			for (const auto& node : nodes) {
				// Access basic fields
				ideation.ID = node["ID"];
				ideation.nodeType = node["NodeType"];
				ideation.name = node["Name"];


				// Access nested Payload object
				if (node.contains("Payload")) {
					const auto& payload = node["Payload"];

					// TODO: Refactor field getted based on NodeType
					string semantic = payload.value("Semantic", "N/A");
					string text = payload.value("Text", "N/A");

				}

				// Access nested Metadata object
				if (node.contains("Metadata")) {
					const auto& metadata = node["Metadata"];
					string author = metadata.value("Author", "Unknown");
					int createdDate = metadata.value("CreatedDate", 0);
					bool inlineFlag = metadata.value("Inline", false);
					MAYBE_UNUSED(author);
					MAYBE_UNUSED(createdDate);
					MAYBE_UNUSED(inlineFlag);

					// Handle dependencies
					if (metadata.contains("Dependencies")) {
						const auto& dependencies = metadata["Dependencies"];
						for (const auto& dependency : dependencies)
						{
							MAYBE_UNUSED(dependency);
						}
					}
				}
			}
		}

	}
}
