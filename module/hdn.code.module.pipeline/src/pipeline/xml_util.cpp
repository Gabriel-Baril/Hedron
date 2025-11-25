#include "core/core.h"

#include "xml_util.h"

namespace hdn
{
	const char* try_get_xml_attr(const pugi::xml_node &node, const char* attr)
	{
		const pugi::char_t *attrValue = node.attribute(attr).as_string();
		return attrValue;
	}
	
	const char* get_xml_attr(const pugi::xml_node &node, const char* attr)
	{
		const char* attrValue = try_get_xml_attr(node, attr);
		HDN_CORE_ASSERT(attrValue, "'{0}' attibute cannot be found. Use try_get_xml_attr instead if you can omit the attribute", attr);
		return attrValue;
	}
}