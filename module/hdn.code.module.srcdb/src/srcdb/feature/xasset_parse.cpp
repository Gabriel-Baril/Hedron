#include "xasset_parse.h"
#include "xasset.h"

#include "srcdb/generated/feature_generated.h"
#include "srcdb/xml_util.h"
#include "srcdb/fbs_util.h"

namespace hdn
{	
	static constexpr const char* XML_FEATURE_NODE_FEATURES_NAME = "Features";
	static constexpr const char* XML_FEATURE_NODE_FEATURE_NAME = "Feature";
	static constexpr const char* XML_FEATURE_ATTR_FEATURE_NAME = "feature";
	
	static constexpr const char* XML_FEATURE_NODE_ZONES_NAME = "Zones";
	static constexpr const char* XML_FEATURE_NODE_ZONE_NAME = "Zone";
	static constexpr const char* XML_FEATURE_ATTR_ZONE_NAME = "zone";
	
	static bool feature_asset_parse(flatbuffers::FlatBufferBuilder &builder, const pugi::xml_node &node, const SourceContext &ctx)
	{
		// Name & id
		const char* nameStr = get_xml_attr(node, "name");
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
		

		Signature<XFeatureAssetObject> sig(nameStr);
		const u64 oId = object_get_id(sig);
		auto fbFeatureAsset = CreateXFeatureAsset(
			builder,
			oId,
			fbName,
			fbFeatures,
			fbZones
		);

		FinishXFeatureAssetBuffer(builder, fbFeatureAsset);
		cache_obj_store(oId, builder.GetBufferPointer(), builder.GetSize());
		
		return true;
	}

	bool xasset_parse_feature(const pugi::xml_node &node, const SourceContext &ctx)
	{
		flatbuffers::FlatBufferBuilder builder(2048);
		feature_asset_parse(builder, node, ctx);

		// TODO
		// FeatureAssetSignature sig;
		// sig.name = nameStr;
		// object_store(sig, builder.GetBufferPointer(), builder.GetSize()); // cache_create_entry(id, builder.GetBufferPointer(), builder.GetSize(), true);
		
		return false;
	}
}