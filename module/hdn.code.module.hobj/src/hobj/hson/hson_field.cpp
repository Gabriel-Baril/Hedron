#include "hson_field.h"

namespace hdn
{
	static constexpr const char* s_HsonFieldTypeStr[Underlying(hson_field_t::count)] = {
		"hson_undefined",
		"hson_object",
		"hson_struct",
		"hson_payload",
		"hson_i8",
		"hson_i16",
		"hson_i32",
		"hson_i64",
		"hson_u8",
		"hson_u16",
		"hson_u32",
		"hson_u64",
		"hson_f32",
		"hson_f64",
		"hson_string"
	};


	const char* hson_field_to_string(hson_field_t fieldType)
	{
		return s_HsonFieldTypeStr[Underlying(fieldType)];
	}

}