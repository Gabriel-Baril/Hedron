#include "feature_convert.h"

#include "srcdb/generated/feature_generated.h"
#include "srcdb/xml_util.h"
#include "srcdb/fbs_util.h"
#include "feature.h"

namespace hdn
{	
	static constexpr const char* XML_FEATURE_NODE_FEATURES_NAME = "Features";
	static constexpr const char* XML_FEATURE_NODE_FEATURE_NAME = "Feature";
	static constexpr const char* XML_FEATURE_ATTR_FEATURE_NAME = "feature";
	
	static constexpr const char* XML_FEATURE_NODE_ZONES_NAME = "Zones";
	static constexpr const char* XML_FEATURE_NODE_ZONE_NAME = "Zone";
	static constexpr const char* XML_FEATURE_ATTR_ZONE_NAME = "zone";
	
	static bool feature_parse(flatbuffers::FlatBufferBuilder &builder, const pugi::xml_node &node, const SourceContext &ctx)
	{	
		// Meta
		auto fbMeta = create_meta(builder, ctx, FEATURE_CODE_VERSION);

		// Name & id
		const char* nameStr = get_xml_attr(node, "name");
		uint64_t id = static_cast<uint64_t>(hash_generate(nameStr, strlen(nameStr)));
		auto fbName = builder.CreateString(nameStr);
		
		// <Features>
		auto fbFeatures = create_string_vector_from_xml(
			builder,
			node.child(XML_FEATURE_NODE_FEATURES_NAME),
			XML_FEATURE_NODE_FEATURE_NAME,
			XML_FEATURE_ATTR_FEATURE_NAME
		);
		
		// <Zones>
		auto fbZones = create_string_vector_from_xml(
			builder,
			node.child(XML_FEATURE_NODE_ZONES_NAME),
			XML_FEATURE_NODE_ZONE_NAME,
			XML_FEATURE_ATTR_ZONE_NAME
		);
		

		// TODO
		// auto fbFeatureAsset = CreateFeatureAsset(
		// 		builder,
		// 	id,
		// 	fbMeta,
		// 		fbName,
		// 		fbFeatures,
		// 	fbZones
		// );

		// FinishFeatureAssetBuffer(builder, fbFeatureAsset);
		
		return true;
	}

	bool feature_parse_callback(const pugi::xml_node &node, const SourceContext &ctx)
	{
		flatbuffers::FlatBufferBuilder builder(2048);
		feature_parse(builder, node, ctx);

		// TODO
		// FeatureAssetSignature sig;
		// sig.name = nameStr;
		// object_store(sig, builder.GetBufferPointer(), builder.GetSize()); // cache_create_entry(id, builder.GetBufferPointer(), builder.GetSize(), true);
		
		return false;
	}
}