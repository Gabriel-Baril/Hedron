#include <iostream>
#include <vector>
#include <algorithm>

#include "hobj/hson/hson.h"
#include "hobj/hson/hson_build.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"
#include "pair.h"

#include "core/io/stream_save.h"
#include "hobj/hson/hson_hobj.h"

void create_hson()
{
	using namespace dm;
	HsonBuilder def;
	{
		u8 pixels[5] = {23, 65, 89, 39, 10};
		f32 arr[4] = {12.4f, 7.6f, -8.9f, 14.32f};
		u64 modelRef = 0; // GetAsset( "model_0" );

		def["pixels"] = hson::pack<u8>(pixels, 5);
		def["name"] = hson::pack("gbaril");
		def["age"] = hson::pack<i8>(22);
		def["arr"] = hson::pack<f32>(arr, 4);
		def["model_ref"] = hson::pack(modelRef);
		def["packed_pair"] = hson::pack(Pair{12, 56});
		def["pair"]["x"] = hson::pack<f32>(16);
		def["pair"]["y"] = hson::pack<f32>(44);
		def["arr_obj"][0]["a"] = hson::pack<f32>(32);
		def["arr_obj"][0]["b"] = hson::pack<f32>(12);
		def["arr_obj"][1]["pair"]["a"] = hson::pack<f32>(32);
		def["arr_obj"][1]["pair"]["b"] = hson::pack<f32>(12);
		const char *names[3] = {"wip", "dark", "swamp"};
		for (int i = 0; i < 3; i++)
		{
			def["_tags"][i] = hson::pack(names[i]);
		}
		def["__sources"][0]["field_test"] = hson::pack<f32>(32);
		def["__sources"][0]["path"] = hson::pack<f32>(12); // hson::pack("path/to/raw_image_2.png", strlen("path/to/raw_image_2.png") + 1);
		def["__sources"][0]["source_hash"] = hson::pack<u64>(9837929990112112233);
	}

	HRef<HHson> hson = HObjectRegistry::get().create<HHson>("player_profile");
	hson_builder_build(def, hson->get_hson());

	HObjectFilesystemData data;
	data.path = "player_profile.hobj";

	HObjectRegistry::get().object_save(hson.get(), "local", &data, sizeof(data));
}

int main()
{
	using namespace dm;
	log_init();

	HObjectRegistry &registry = HObjectRegistry::get();
	registry.add_source<FilesystemObjectSource>("local", "objects");
	registry.populate();

	// CreateHson();

	HHson *hson = registry.get<HHson>("player_profile");
	Hson &h = hson->get_hson();
	DM_INFO_LOG("Name: {0}", h["name"].as<char>());

	registry.stats();

	return 0;
}
