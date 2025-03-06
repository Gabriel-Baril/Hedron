#pragma once

#include <ostream>

#include "dynamic_memory_buffer.h"

namespace hdn
{
	class hostream
	{
	public:
		hostream( DynamicMemoryBuffer *buffer )
			: m_stream{ buffer }
		{
		}

		template<typename T>
		void write_pod(const T& object)
		{
			static_assert(std::is_trivial_v<T>, "T must be a trivial type");
			m_stream.write(reinterpret_cast<const char*>(&object), sizeof(object));
		}
	private:
		std::ostream m_stream;
	};

	template<typename T>
	hostream& operator<<(hostream& stream, const T& object)
	{
		stream.write_pod(object);
		return stream;
	}
}