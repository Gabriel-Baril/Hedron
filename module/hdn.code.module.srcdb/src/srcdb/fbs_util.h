#include "pugixml/pugixml.hpp"

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
	
	inline auto create_meta(flatbuffers::FlatBufferBuilder &builder, const SourceContext &ctx, u64 codeVersion)
	{
		// Create file dependencies
		hdn::FileDependencyBuilder fd1(builder);
		std::string filePathStr = ctx.path.string();
		const char *filePath = filePathStr.c_str();
		fd1.add_path_hash(hash_generate(filePath, strlen(filePath)));
		fd1.add_last_modified_timestamp(filesystem_last_write_time(ctx.path));
		auto fd_offset1 = fd1.Finish();

		flatbuffers::Offset<hdn::FileDependency> file_dep_array[] = {fd_offset1};
		auto fbFileDeps = builder.CreateVector(file_dep_array, 1);

		// Build Metadata
		return hdn::CreateMetadata(builder, fbFileDeps, codeVersion);
	}
}