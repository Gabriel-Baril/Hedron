#include "ideation_parser.h"

#include "nlohmann/json.hpp"

namespace hdn
{
	using json = nlohmann::json;

	void IdeationParser::ParseIdeationFile(const std::string& path, Ideation& ideation)
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
					std::string semantic = payload.value("Semantic", "N/A");
					std::string text = payload.value("Text", "N/A");

				}

				// Access nested Metadata object
				if (node.contains("Metadata")) {
					const auto& metadata = node["Metadata"];
					std::string author = metadata.value("Author", "Unknown");
					int createdDate = metadata.value("CreatedDate", 0);
					bool inlineFlag = metadata.value("Inline", false);


					// Handle dependencies
					if (metadata.contains("Dependencies")) {
						const auto& dependencies = metadata["Dependencies"];
						for (const auto& dependency : dependencies)
						{

						}
					}
				}
			}
		}

	}
}
