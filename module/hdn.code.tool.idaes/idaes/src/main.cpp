#include "ideation_parse.h"
#include "ideation_database.h"

int main()
{
	IdeationNode node;
	parse_ideation_file("../data/ideation.json", node);
	return 0;
}


// void example_json()
// {
// 	const char json[] = "{\"a\" : true, \"b\" : [false, null, \"foo\"]}";
// 	struct json_value_s* root = json_parse(json, strlen(json));
// 	assert(root->type == json_type_object);
// 
// 	struct json_object_s* object = (struct json_object_s*)root->payload;
// 	assert(object->length == 2);
// 
// 	struct json_object_element_s* a = object->start;
// 	struct json_string_s* a_name = a->name;
// 	assert(strcmp(a_name->string, "a") == 0);
// 	assert(a_name->string_size == strlen("a"));
// 
// 	struct json_value_s* a_value = a->value;
// 	assert(a_value->type == json_type_true);
// 	assert(a_value->payload == nullptr);
// 
// 	std::cout << (a_value->type == json_type_true) << std::endl;
// 
// 	free(root);
// }