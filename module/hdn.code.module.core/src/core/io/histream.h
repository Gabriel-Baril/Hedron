#pragma once

#include <istream>

#include "dynamic_memory_buffer.h"

namespace hdn
{
	class histream {
	public:
		histream( DynamicMemoryBuffer *buffer )
			: m_Stream{ buffer }
		{
		}

		template<typename T>
		void read_pod(T& object)
		{
			static_assert(std::is_trivial_v<T>, "T must be a trivial type");
			m_Stream.read(reinterpret_cast<char*>(&object), sizeof(object));
		}
	private:
		std::istream m_Stream;
	};

	template<typename T>
	histream& operator>>(histream& stream, T& object)
	{
		stream.read_pod(object);
		return stream;
	}
}