#include "Public/ideation_parse.h"

#include "Public/ideation_parse_payload.h"

#include "json/json.h"

#include <assert.h>
#include <fstream>
#include <sstream>
#include <iostream>

enum class IdeationObjectField
{
	ID,
	IdeationType,
	Name,
	Payload,
	Metadata,
	Size,
	Invalid
};

static const char* s_IdeationObjectFieldStr[underlying(IdeationObjectField::Size)] =
{
	"ID",
	"NodeType",
	"Name",
	"Payload",
	"Metadata"
};

bool parse_ideation_id(struct json_object_element_s* jsonObjectElement, IdeationNode& node)
{
	struct json_value_s* value = jsonObjectElement->value;
	assert(value->type == json_type_string);

	// Parse uuid (string -> UUID)
	return true;
}

bool parse_ideation_type(struct json_object_element_s* jsonObjectElement, IdeationNode& node)
{
	struct json_value_s* value = jsonObjectElement->value;
	assert(value->type == json_type_string);


	return false;
}

bool parse_ideation_name(struct json_object_element_s* jsonObjectElement, IdeationNode& node)
{
	struct json_value_s* value = jsonObjectElement->value;
	assert(value->type == json_type_string);

	return false;
}

bool parse_ideation_payload(struct json_object_element_s* jsonObjectElement, IdeationNode& node)
{
	struct json_value_s* value = jsonObjectElement->value;
	assert(value->type == json_type_object);

	parse_ideation_payload((struct json_object_s*)value->payload, node);

	return false;
}

bool parse_ideation_metadata(struct json_object_element_s* jsonObjectElement, IdeationNode& node)
{
	struct json_value_s* value = jsonObjectElement->value;
	assert(value->type == json_type_object);

	return false;
}

using ParseFunc = bool(*)(struct json_object_element_s* jsonObjectElement, IdeationNode& node);
static ParseFunc s_ParseFuncs[underlying(IdeationObjectField::Size)] =
{
	parse_ideation_id,
	parse_ideation_type,
	parse_ideation_name,
	parse_ideation_payload,
	parse_ideation_metadata,
};

static IdeationObjectField GetIdeationObjectFieldFromName(const char* name)
{
	for (int i = 0; i < underlying(IdeationObjectField::Size); i++)
	{
		if (strcmp(name, s_IdeationObjectFieldStr[i]) == 0)
		{
			return static_cast<IdeationObjectField>(i);
		}
	}
	return IdeationObjectField::Invalid;
}

static bool ParseIdeationObjectField(IdeationObjectField field, struct json_object_element_s* jsonObjectElement, IdeationNode& node)
{
	const auto func = s_ParseFuncs[underlying(field)];
	if (func != nullptr)
	{
		return func(jsonObjectElement, node);
	}
	return false;
}

static bool parse_ideation_object(json_object_s* object, IdeationNode& node)
{
	struct json_object_element_s* currentObjectElement = (struct json_object_element_s*)object->start;
	while (currentObjectElement != nullptr)
	{
		struct json_string_s* currentObjectElementName = currentObjectElement->name;
		IdeationObjectField field = GetIdeationObjectFieldFromName(currentObjectElementName->string);
		assert(field != IdeationObjectField::Invalid);

		if (!ParseIdeationObjectField(field, currentObjectElement, node))
		{
			std::cout << "Failed to parse '" << currentObjectElementName->string << "'" << std::endl;
		}
		
		currentObjectElement = currentObjectElement->next;
	}

	return true;
}

bool parse_ideation_json(const std::string& jsonIdeationContent, IdeationNode& node)
{
	struct json_value_s* root = json_parse(jsonIdeationContent.c_str(), strlen(jsonIdeationContent.c_str()));
	assert(root->type == json_type_object);

	struct json_object_s* object = (struct json_object_s*)root->payload;
	assert(object->length == 1); // The 'Nodes' array

	struct json_object_element_s* ideationNodes = object->start;
	struct json_value_s* ideationNodes_value = ideationNodes->value;
	assert(ideationNodes_value->type == json_type_array);

	struct json_array_s* ideationNodesArray = (struct json_array_s*)ideationNodes_value->payload;
	struct json_array_element_s* currentIdeationNode = ideationNodesArray->start;

	while (currentIdeationNode != nullptr)
	{
		json_value_s* ideationObject_value = currentIdeationNode->value;
		assert(ideationObject_value->type == json_type_object);

		parse_ideation_object((struct json_object_s*)ideationObject_value->payload, node);

		std::cout << "Parsing ideation" << std::endl;
		currentIdeationNode = currentIdeationNode->next;
	}

	free(root);

	std::cout << "Json Parsed!" << std::endl;

	return true;
}

bool parse_ideation_file(const std::string& jsonPath, IdeationNode& node)
{
	std::ifstream file(jsonPath);
	if (!file.is_open()) {
		std::cerr << "Could not open the file!" << std::endl;
		return false;
	}

	std::stringstream buffer;
	buffer << file.rdbuf(); // Read the entire file into the buffer
	std::string content = buffer.str();

	std::cout << "File content:\n" << content << std::endl;

	if (!parse_ideation_json(content, node))
	{
		return false;
	}

	file.close();
	return true;
}