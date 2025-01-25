#include "buffer_reader.h"

namespace hdn
{
	FBufferReader::FBufferReader(const byte* buffer)
	{
		m_BufferBase = buffer;
		m_CurrentPtr = m_BufferBase;
	}
}
