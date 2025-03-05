#include "stream_reader.h"

namespace hdn
{
	void StreamReader::Read( int &object )
	{
		m_Stream.read( reinterpret_cast<char *>(&object), sizeof( object ) );
	}

	void StreamReader::Read( size_t &object )
	{
		m_Stream.read( reinterpret_cast<char *>(&object), sizeof( object ) );
	}

	StreamReader &operator>>( StreamReader &stream, int &object )
	{
		stream.Read( object );
		return stream;
	}

	StreamReader &operator>>( StreamReader &stream, size_t &object )
	{
		stream.Read( object );
		return stream;
	}
}
