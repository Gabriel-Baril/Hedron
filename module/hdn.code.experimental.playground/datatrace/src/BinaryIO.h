#pragma once

#include <string>

#include "core/CoreType.h"

#include "BufferWriter.h"
#include "GroupBufferWriter.h"

namespace hdn::bin
{
	template<typename T>
	concept IsPrimitive =
		std::is_same_v<T, bool> ||
		std::is_same_v<T, byte> ||
		std::is_same_v<T, ubyte> ||
		std::is_same_v<T, int8> ||
		std::is_same_v<T, uint8> ||
		std::is_same_v<T, int16> ||
		std::is_same_v<T, uint16> ||
		std::is_same_v<T, int32> ||
		std::is_same_v<T, uint32> ||
		std::is_same_v<T, int64> ||
		std::is_same_v<T, uint64> ||
		std::is_same_v<T, float32> ||
		std::is_same_v<T, float64> ||
		std::is_same_v<T, vec2i8> ||
		std::is_same_v<T, vec2ui8> ||
		std::is_same_v<T, vec2i16> ||
		std::is_same_v<T, vec2ui16> ||
		std::is_same_v<T, vec2i32> ||
		std::is_same_v<T, vec2ui32> ||
		std::is_same_v<T, vec2i64> ||
		std::is_same_v<T, vec2ui64> ||
		std::is_same_v<T, vec2f32> ||
		std::is_same_v<T, vec2f64> ||
		std::is_same_v<T, vec3i8> ||
		std::is_same_v<T, vec3ui8> ||
		std::is_same_v<T, vec3i16> ||
		std::is_same_v<T, vec3ui16> ||
		std::is_same_v<T, vec3i32> ||
		std::is_same_v<T, vec3ui32> ||
		std::is_same_v<T, vec3i64> ||
		std::is_same_v<T, vec3ui64> ||
		std::is_same_v<T, vec3f32> ||
		std::is_same_v<T, vec3f64> ||
		std::is_same_v<T, vec4i8> ||
		std::is_same_v<T, vec4ui8> ||
		std::is_same_v<T, vec4i16> ||
		std::is_same_v<T, vec4ui16> ||
		std::is_same_v<T, vec4i32> ||
		std::is_same_v<T, vec4ui32> ||
		std::is_same_v<T, vec4i64> ||
		std::is_same_v<T, vec4ui64> ||
		std::is_same_v<T, vec4f32> ||
		std::is_same_v<T, vec4f64>;

	void Push(FGroupBufferWriter& writer, const char* semantic);
	
	template<typename T>
	void Meta(FGroupBufferWriter& writer, KeyValueType type, const char* key, const T& value)
	{
		writer.Meta(type, key, value);
	}

	void Pop(FGroupBufferWriter& writer);

	/* START -- Functions to make the interface more uniform */
	template<IsPrimitive T>
	inline void Write(const T& object, FBufferWriter& writer)
	{
		writer.Write(object);
	}

	template<typename T>
	inline void Write(const T& object, FGroupBufferWriter& writer, const char* semantic)
	{
		writer.Push(semantic);
		Write(object, writer);
		writer.Pop();
	}

	void Write(const std::string& object, FBufferWriter& writer);

	/* END -- Functions to make the interface more uniform */

	/* START -- Functions to make the interface more uniform */
	// void Read(bool& object, BinaryReader& reader);
	// 
	// void Read(int8& object, BinaryReader& reader);
	// void Read(uint8& object, BinaryReader& reader);
	// void Read(int16& object, BinaryReader& reader);
	// void Read(uint16& object, BinaryReader& reader);
	// void Read(int32& object, BinaryReader& reader);
	// void Read(uint32& object, BinaryReader& reader);
	// void Read(int64& object, BinaryReader& reader);
	// void Read(uint64& object, BinaryReader& reader);
	// void Read(float32& object, BinaryReader& reader);
	// void Read(float64& object, BinaryReader& reader);
	// 
	// void Read(vec2i8& object, BinaryReader& reader);
	// void Read(vec2ui8& object, BinaryReader& reader);
	// void Read(vec2i16& object, BinaryReader& reader);
	// void Read(vec2ui16& object, BinaryReader& reader);
	// void Read(vec2i32& object, BinaryReader& reader);
	// void Read(vec2ui32& object, BinaryReader& reader);
	// void Read(vec2i64& object, BinaryReader& reader);
	// void Read(vec2ui64& object, BinaryReader& reader);
	// void Read(vec2f32& object, BinaryReader& reader);
	// void Read(vec2f64& object, BinaryReader& reader);
	// 
	// void Read(vec3i8& object, BinaryReader& reader);
	// void Read(vec3ui8& object, BinaryReader& reader);
	// void Read(vec3i16& object, BinaryReader& reader);
	// void Read(vec3ui16& object, BinaryReader& reader);
	// void Read(vec3i32& object, BinaryReader& reader);
	// void Read(vec3ui32& object, BinaryReader& reader);
	// void Read(vec3i64& object, BinaryReader& reader);
	// void Read(vec3ui64& object, BinaryReader& reader);
	// void Read(vec3f32& object, BinaryReader& reader);
	// void Read(vec3f64& object, BinaryReader& reader);
	// 
	// void Read(vec4i8& object, BinaryReader& reader);
	// void Read(vec4ui8& object, BinaryReader& reader);
	// void Read(vec4i16& object, BinaryReader& reader);
	// void Read(vec4ui16& object, BinaryReader& reader);
	// void Read(vec4i32& object, BinaryReader& reader);
	// void Read(vec4ui32& object, BinaryReader& reader);
	// void Read(vec4i64& object, BinaryReader& reader);
	// void Read(vec4ui64& object, BinaryReader& reader);
	// void Read(vec4f32& object, BinaryReader& reader);
	// void Read(vec4f64& object, BinaryReader& reader);
	// 
	// void Read(std::string& out, BinaryReader& reader);
	/* END -- Functions to make the interface more uniform */
}