#pragma once

#include <ostream>

#include "dynamic_memory_buffer.h"

namespace hdn
{
	class StreamWriter
	{
	public:
		StreamWriter( DynamicMemoryBuffer *buffer )
			: m_Stream{ buffer }
		{
		}

		void Write( const int &object );
		void Write( const size_t &object );
	private:
		std::ostream m_Stream;
	};

	StreamWriter &operator<<( StreamWriter &stream, const int &object );
	StreamWriter &operator<<( StreamWriter &stream, const size_t &object );
}