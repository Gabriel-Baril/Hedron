#pragma once

#include "pugixml/pugixml.hpp"

namespace dm
{
	const char *try_get_xml_attr(const pugi::xml_node &node, const char *attr);
	const char *get_xml_attr(const pugi::xml_node &node, const char *attr);
}
