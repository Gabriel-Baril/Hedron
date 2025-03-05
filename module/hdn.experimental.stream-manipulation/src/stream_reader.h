#pragma once

#include <istream>

#include "dynamic_memory_buffer.h"

namespace hdn
{
	class StreamReader {
	public:
		StreamReader( DynamicMemoryBuffer *buffer )
			: m_Stream{ buffer }
		{
		}

		void Read( int &object );
		void Read( size_t &object );
	private:
		std::istream m_Stream;
	};

	StreamReader &operator>>( StreamReader &stream, int &object );
	StreamReader &operator>>( StreamReader &stream, size_t &object );
}