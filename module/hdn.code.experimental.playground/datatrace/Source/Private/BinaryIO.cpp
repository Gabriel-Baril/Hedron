#include "DataTrace/BinaryIO.h"

namespace hdn::bin
{
	// template<typename VecType>
	// void ReadVec2(VecType& vecType, BinaryReader& reader)
	// {
	// 	Read(vecType.x, reader);
	// 	Read(vecType.y, reader);
	// }
	// 
	// template<typename VecType>
	// void ReadVec3(VecType& vecType, BinaryReader& reader)
	// {
	// 	Read(vecType.x, reader);
	// 	Read(vecType.y, reader);
	// 	Read(vecType.z, reader);
	// }
	// 
	// template<typename VecType>
	// void ReadVec4(VecType& vecType, BinaryReader& reader)
	// {
	// 	Read(vecType.x, reader);
	// 	Read(vecType.y, reader);
	// 	Read(vecType.z, reader);
	// 	Read(vecType.w, reader);
	// }

	void Push(FGroupBufferWriter& writer, const char* semantic)
	{
		writer.Push(semantic);
	}

	void Pop(FGroupBufferWriter& writer)
	{
		writer.Pop();
	}

	void Write(const std::string& object, FBufferWriter& writer)
	{
		const auto size = object.size();
		writer.Write<size_t>(size);
		writer.Write(object.c_str(), size + 1);
	}

	// void Read(bool& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<bool>();
	// }
	// 
	// void Read(int8& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<int8>();
	// }
	// 
	// void Read(uint8& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<uint8>();
	// }
	// 
	// void Read(int16& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<int16>();
	// }
	// 
	// void Read(uint16& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<uint16>();
	// }
	// 
	// void Read(int32& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<int32>();
	// }
	// 
	// void Read(uint32& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<uint32>();
	// }
	// 
	// void Read(int64& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<int64>();
	// }
	// 
	// void Read(uint64& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<uint64>();
	// }
	// 
	// void Read(float32& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<float32>();
	// }
	// 
	// void Read(float64& object, BinaryReader& reader)
	// {
	// 	object = reader.Read<float64>();
	// }
	// 
	// void Read(vec2i8& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2ui8& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2i16& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2ui16& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2i32& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2ui32& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2i64& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2ui64& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2f32& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec2f64& object, BinaryReader& reader)
	// {
	// 	ReadVec2(object, reader);
	// }
	// 
	// void Read(vec3i8& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3ui8& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3i16& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3ui16& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3i32& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3ui32& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3i64& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3ui64& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3f32& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec3f64& object, BinaryReader& reader)
	// {
	// 	ReadVec3(object, reader);
	// }
	// 
	// void Read(vec4i8& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4ui8& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4i16& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4ui16& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4i32& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4ui32& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4i64& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4ui64& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4f32& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
	// 
	// void Read(vec4f64& object, BinaryReader& reader)
	// {
	// 	ReadVec4(object, reader);
	// }
}

