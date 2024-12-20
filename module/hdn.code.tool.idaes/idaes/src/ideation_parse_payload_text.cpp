#include "ideation_parse_payload.h"
#include "json/json.h"
#include <assert.h>
#include <iostream>

enum class JsonField
{
	Semantic,
	Text,
	Size,
	Invalid
};

static const char* s_FieldStr[underlying(JsonField::Size)] =
{
	"Semantic",
	"Text"
};

static bool parse_ideation_payload_text_semantic(struct json_object_element_s* jsonObjectElement, IdeationPayload& payload)
{
	return false;
}

static bool parse_ideation_payload_text_text(struct json_object_element_s* jsonObjectElement, IdeationPayload& payload)
{
	return false;
}

using ParseFunc = bool(*)(struct json_object_element_s* jsonObjectElement, IdeationPayload& payload);
static ParseFunc s_ParseFuncs[underlying(JsonField::Size)] =
{
	parse_ideation_payload_text_semantic,
	parse_ideation_payload_text_text
};

static JsonField get_field_from_name(const char* name)
{
	for (int i = 0; i < underlying(JsonField::Size); i++)
	{
		if (strcmp(name, s_FieldStr[i]) == 0)
		{
			return static_cast<JsonField>(i);
		}
	}
	return JsonField::Invalid;
}

static bool parse_field(JsonField field, struct json_object_element_s* jsonObjectElement, IdeationPayload& payload)
{
	const auto func = s_ParseFuncs[underlying(field)];
	if (func != nullptr)
	{
		return func(jsonObjectElement, payload);
	}
	return false;
}

bool parse_ideation_payload_text(struct json_object_s* object, IdeationPayload& payload)
{
	struct json_object_element_s* currentObjectElement = (struct json_object_element_s*)object->start;
	while (currentObjectElement != nullptr)
	{
		struct json_string_s* currentObjectElementName = currentObjectElement->name;
		JsonField field = get_field_from_name(currentObjectElementName->string);
		assert(field != JsonField::Invalid);

		if (!parse_field(field, currentObjectElement, payload))
		{
			std::cout << "Failed to parse '" << currentObjectElementName->string << "'" << std::endl;
		}

		currentObjectElement = currentObjectElement->next;
	}

	return true;
}