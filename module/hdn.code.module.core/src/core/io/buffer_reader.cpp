#include "buffer_reader.h"

namespace hdn
{
	FBufferReader::FBufferReader(const byte* buffer)
	{
		m_BufferBase = buffer;
		m_CurrentPtr = m_BufferBase;
	}

	namespace bin
	{
		void Read(FBufferReader& reader, string& object)
		{
			size_t length = reader.Read<size_t>();
			object.reserve(length);

			const char* str = reader.Read<char>(length);
			object.append(str, length);
		}
	}
}
