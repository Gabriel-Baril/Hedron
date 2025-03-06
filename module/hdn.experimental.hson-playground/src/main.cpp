#include <iostream>
#include <vector>
#include <algorithm>

#include "hson/hson.h"
#include "pair.h"

int main()
{
	using namespace hdn;
	Log_Init();

	vector<byte> out;
	{
		hson_builder_t def;
		byte pixels[5] = { 23, 65, 89, 39, 10 };
		f32 arr[4] = { 12.4f, 7.6f, -8.9f, 14.32f };
		u64 modelRef = 0; //GetAsset( "model_0" );

		def["pixels"]					= hson::pack<byte>( pixels, 5 );
		def["name"]						= hson::pack( "gbaril" );
		def["age"]						= hson::pack<i8>( 22 );
		def["pair"]["x"]				= hson::pack<f32>( 16 );
		def["pair"]["y"]				= hson::pack<f32>( 44 );
		def["arr"]						= hson::pack<f32>( arr, 4 );
		def["arr_obj"][0]["a"]			= hson::pack<f32>( 32 );
		def["arr_obj"][0]["b"]			= hson::pack<f32>( 12 );
		def["arr_obj"][1]["pair"]["a"]	= hson::pack<f32>( 32 );
		def["arr_obj"][1]["pair"]["b"]	= hson::pack<f32>( 12 );
		def["model_ref"]				= hson::pack<u64>( modelRef );
		def["packed_pair"]				= hson::pack( Pair{ 12, 56 } );

		// def["__type"] = hson::pack<hson_struct_hdef_type>( HDEF_MAP );
		// def["__version"] = hson::pack<hson_struct_u64>(12);
		// def["__tags"] = hson::array<hson_struct_string>(3);
		// def["__tags"][0] = hson::pack<hson_struct_string>("wip");
		// def["__tags"][1] = hson::pack<hson_struct_string>("dark");
		// def["__tags"][2] = hson::pack<hson_struct_string>("swamp");
		// def["__sources"] = hson::array<hson_object_asset_source>(1);
		// def["__sources"][0] = hson::pack<hson_object_asset_source>();
		// def["__sources"][0]["field"] = hson::pack<hson_struct_string>("data/pixels");
		// def["__sources"][0]["path"] = hson::pack<hson_struct_string>("path/to/raw_image_2.png");
		// def["__sources"][0]["source_hash"] = hson::pack<hson_struct_u64>(9837929990112112233);

		def.write( out );
	}
	
	{
		HINFO("Buffer size: {0}", out.size());

		hson_t def{ out };

		const byte* pixels = def["pixels"].as<byte>();
		const char* name = def["name"].as<char>();
		const i8* age = def["age"].as<i8>();
		const f32* pair_x = def["pair"]["x"].as<f32>();
		const f32* pair_y = def["pair"]["x"].as<f32>();
		const f32* arr = def["arr"].as<f32>();
		const f32* arr_obj_0_a = def["arr_obj"][0]["a"].as<f32>();
		const f32* arr_obj_0_b = def["arr_obj"][0]["b"].as<f32>();
		const f32* arr_obj_1_pair_a = def["arr_obj"][1]["pair"]["a"].as<f32>();
		const f32* arr_obj_1_pair_b = def["arr_obj"][1]["pair"]["b"].as<f32>();
		const u64* model_ref = def["model_ref"].as<u64>();
		const Pair* packed_pair = def["packed_pair"].as<Pair>();

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
	}

	return 0;
}