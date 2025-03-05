#include "stream_writer.h"

namespace hdn
{
	void StreamWriter::Write( const int &object )
	{
		m_Stream.write( reinterpret_cast<const char *>(&object), sizeof( object ) );
	}

	void StreamWriter::Write( const size_t &object )
	{
		m_Stream.write( reinterpret_cast<const char *>(&object), sizeof( object ) );
	}

	StreamWriter &operator<<( StreamWriter &stream, const int &object )
	{
		stream.Write( object );
		return stream;
	}

	StreamWriter &operator<<( StreamWriter &stream, const size_t &object )
	{
		stream.Write( object );
		return stream;
	}
}