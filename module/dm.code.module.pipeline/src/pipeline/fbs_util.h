#pragma once

#include "pugixml/pugixml.hpp"

#include "xml_util.h"

namespace hdn
{
	inline auto create_string_vector_from_xml(
		flatbuffers::FlatBufferBuilder &builder,
		const pugi::xml_node &xNode,
		const char* childNodeName,
		const char* childAttrName
	)
	{
		std::vector<flatbuffers::Offset<flatbuffers::String>> fbVec{};
		if (xNode)
		{
			for (const auto &xSubnode : xNode.children(childNodeName))
			{
				const char* attrValueStr = get_xml_attr(xSubnode, childAttrName);
				auto fbFeatureName = builder.CreateString(attrValueStr);
				fbVec.emplace_back(fbFeatureName);
			}
		}
		return builder.CreateVector(fbVec);
	}
}