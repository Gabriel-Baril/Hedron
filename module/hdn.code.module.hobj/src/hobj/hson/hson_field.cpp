#include "hson_field.h"

namespace hdn
{
	static constexpr const char* s_HsonFieldTypeStr[Underlying(HsonField::COUNT)] = {
		"HSON_UNDEFINED",
		"HSON_OBJECT",
		"HSON_STRUCT",
		"HSON_PAYLOAD",
		"HSON_I8",
		"HSON_I16",
		"HSON_I32",
		"HSON_I64",
		"HSON_U8",
		"HSON_U16",
		"HSON_U32",
		"HSON_U64",
		"HSON_F32",
		"HSON_F64",
		"HSON_STRING"
	};


	const char* hson_field_to_string(HsonField fieldType)
	{
		return s_HsonFieldTypeStr[Underlying(fieldType)];
	}

}