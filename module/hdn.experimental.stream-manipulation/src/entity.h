#pragma once

#include "stream_reader.h"
#include "stream_writer.h"

namespace hdn
{
	class HEntity {
	public:
		friend StreamReader &operator>>( StreamReader &stream, HEntity &object ) {
			stream >> object.m_NumComponent;
			return stream;
		}

		friend StreamWriter &operator<<( StreamWriter &stream, const HEntity &object ) {
			stream << object.m_NumComponent;
			return stream;
		}
		int m_NumComponent;
	};
}