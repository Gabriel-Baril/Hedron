#include <iostream>
#include <vector>
#include <algorithm>

#include "hobj/hson/hson.h"
#include "hobj/hson/hson_build.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"
#include "pair.h"

#include "core/io/stream_save.h"

int main()
{
	using namespace hdn;
	Log_Init();

	{
		HObjectRegistry& registry = HObjectRegistry::Get();
		registry.AddSource<FilesystemObjectSource>("local", "/objects");
		registry.Build();

		Ref<HObject> obj = registry.Create<HObject>();

		HObjectFilesystemData data;
		data.path = "/path/to/obj.hobj";
		registry.Save(obj.get(), "local", &data, sizeof(data));
	}

	hostream stream;
	{
		hson_builder_t def;
		{
			byte pixels[5] = { 23, 65, 89, 39, 10 };
			f32 arr[4] = { 12.4f, 7.6f, -8.9f, 14.32f };
			u64 modelRef = 0; //GetAsset( "model_0" );

			def["pixels"] = hson::pack<byte>(pixels, 5);
			def["name"] = hson::pack("gbaril");
			def["age"] = hson::pack<i8>(22);
			def["arr"] = hson::pack<f32>(arr, 4);
			def["model_ref"] = hson::pack(modelRef);
			def["packed_pair"] = hson::pack(Pair{ 12, 56 });
			def["pair"]["x"] = hson::pack<f32>(16);
			def["pair"]["y"] = hson::pack<f32>(44);
			def["arr_obj"][0]["a"] = hson::pack<f32>(32);
			def["arr_obj"][0]["b"] = hson::pack<f32>(12);
			def["arr_obj"][1]["pair"]["a"] = hson::pack<f32>(32);
			def["arr_obj"][1]["pair"]["b"] = hson::pack<f32>(12);
			const char* names[3] = { "wip", "dark", "swamp" };
			for (int i = 0; i < 3; i++)
			{
				def["_tags"][i] = hson::pack(names[i]);
			}
			def["__sources"][0]["field_test"] = hson::pack<f32>(32);
			def["__sources"][0]["path"] = hson::pack<f32>(12);// hson::pack("path/to/raw_image_2.png", strlen("path/to/raw_image_2.png") + 1);
			def["__sources"][0]["source_hash"] = hson::pack<u64>(9837929990112112233);
		}

		hson_t h;
		hson_builder_build(def, h);
		hson_serialize(stream, h);

		{
			const byte* pixels = h["pixels"].as<byte>();
			const char* name = h["name"].as<char>();
			const i8* age = h["age"].as<i8>();
			const f32* pair_x = h["pair"]["x"].as<f32>();
			const f32* pair_y = h["pair"]["y"].as<f32>();
			const f32* arr = h["arr"].as<f32>();
			const f32* arr_obj_0_a = h["arr_obj"][0]["a"].as<f32>();
			const f32* arr_obj_0_b = h["arr_obj"][0]["b"].as<f32>();
			const f32* arr_obj_1_pair_a = h["arr_obj"][1]["pair"]["a"].as<f32>();
			const f32* arr_obj_1_pair_b = h["arr_obj"][1]["pair"]["b"].as<f32>();
			const u64* model_ref = h["model_ref"].as<u64>();
			const Pair* packed_pair = h["packed_pair"].as<Pair>();
			HINFO("'pixels[0]': {0}", pixels[0]);
			HINFO("'pixels[1]': {0}", pixels[1]);
			HINFO("'pixels[2]': {0}", pixels[2]);
			HINFO("'pixels[3]': {0}", pixels[3]);
			HINFO("'pixels[4]': {0}", pixels[4]);
			HINFO("'name': {0}", name);
			HINFO("'age': {0}", *age);
			HINFO("'pair_x': {0}", *pair_x);
			HINFO("'pair_y': {0}", *pair_y);
			HINFO("'arr[0]': {0}", arr[0]);
			HINFO("'arr[1]': {0}", arr[1]);
			HINFO("'arr[2]': {0}", arr[2]);
			HINFO("'arr[3]': {0}", arr[3]);
			HINFO("'arr_obj_0_a': {0}", *arr_obj_0_a);
			HINFO("'arr_obj_0_b': {0}", *arr_obj_0_b);
			HINFO("'arr_obj_1_pair_a': {0}", *arr_obj_1_pair_a);
			HINFO("'arr_obj_1_pair_b': {0}", *arr_obj_1_pair_b);
			HINFO("'model_ref': {0}", *model_ref);
			HINFO("'packed_pair->x': {0}", packed_pair->x);
			HINFO("'packed_pair->y': {0}", packed_pair->y);
			HINFO("Name: {0}", hson_field_to_string(*h["__sources"].get_field_type()));
		}
	}
	
	{
		// HINFO("Buffer size: {0}", out.size());
		stream.get_stream_buffer()->ResetHead();
		histream inStream{stream.get_stream_buffer()};
		hson_t h;
		hson_deserialize(inStream, h);

		const byte* pixels = h["pixels"].as<byte>();
		const char* name = h["name"].as<char>();
		const i8* age = h["age"].as<i8>();
		const f32* pair_x = h["pair"]["x"].as<f32>();
		const f32* pair_y = h["pair"]["y"].as<f32>();
		const f32* arr = h["arr"].as<f32>();
		const f32* arr_obj_0_a = h["arr_obj"][0]["a"].as<f32>();
		const f32* arr_obj_0_b = h["arr_obj"][0]["b"].as<f32>();
		const f32* arr_obj_1_pair_a = h["arr_obj"][1]["pair"]["a"].as<f32>();
		const f32* arr_obj_1_pair_b = h["arr_obj"][1]["pair"]["b"].as<f32>();
		const u64* model_ref = h["model_ref"].as<u64>();
		const Pair* packed_pair = h["packed_pair"].as<Pair>();

		HINFO("'pixels[0]': {0}", pixels[0]);
		HINFO("'pixels[1]': {0}", pixels[1]);
		HINFO("'pixels[2]': {0}", pixels[2]);
		HINFO("'pixels[3]': {0}", pixels[3]);
		HINFO("'pixels[4]': {0}", pixels[4]);
		HINFO("'name': {0}", name);
		HINFO("'age': {0}", *age);
		HINFO("'pair_x': {0}", *pair_x);
		HINFO("'pair_y': {0}", *pair_y);
		HINFO("'arr[0]': {0}", arr[0]);
		HINFO("'arr[1]': {0}", arr[1]);
		HINFO("'arr[2]': {0}", arr[2]);
		HINFO("'arr[3]': {0}", arr[3]);
		HINFO("'arr_obj_0_a': {0}", *arr_obj_0_a);
		HINFO("'arr_obj_0_b': {0}", *arr_obj_0_b);
		HINFO("'arr_obj_1_pair_a': {0}", *arr_obj_1_pair_a);
		HINFO("'arr_obj_1_pair_b': {0}", *arr_obj_1_pair_b);
		HINFO("'model_ref': {0}", *model_ref);
		HINFO("'packed_pair->x': {0}", packed_pair->x);
		HINFO("'packed_pair->y': {0}", packed_pair->y);

		save_binary_to_file("player_profile.hobj", stream.data(), stream.size());
	}

	return 0;
}