#include "binary_io.h"

namespace hdn::bin
{
	void Push(FGroupBufferWriter& writer, const char* semantic)
	{
		writer.Push(semantic);
	}

	void Pop(FGroupBufferWriter& writer)
	{
		writer.Pop();
	}

	void Write(FBufferWriter& writer, const std::string& object)
	{
		const auto size = object.size();
		writer.Write<size_t>(size + 1);
		writer.Write(object.c_str(), size + 1);
	}

	void Read(FBufferReader& reader, std::string& object)
	{
		size_t length = reader.Read<size_t>();
		object.reserve(length);

		const char* str = reader.Read<char>(length);
		object.append(str, length);
	}
}

