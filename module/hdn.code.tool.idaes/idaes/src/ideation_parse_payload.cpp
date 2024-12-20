#include "ideation_parse_payload.h"
#include "sheredom-json/json.h"

using ParseFunc = bool(*)(struct json_object_s* jsonObjectElement, IdeationPayload& node);
static ParseFunc s_ParseFuncs[underlying(IdeationNodeType::Size)] =
{
	parse_ideation_payload_text
};

bool parse_ideation_payload(struct json_object_s* jsonObject, IdeationNode& node)
{
	if (node.Type != IdeationNodeType::Invalid)
	{
		return false;
	}

	const auto& func = s_ParseFuncs[underlying(node.Type)];
	IdeationPayload payload;
	if (func != nullptr)
	{
		return func(jsonObject, payload);
	}
	return false;
}