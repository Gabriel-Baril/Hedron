#pragma once

#include <istream>

#include "core/core.h"
#include "stream_memory.h"

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
			m_Stream.read(reinterpret_cast<char*>(&object), sizeof(T));
		}

		template<typename T>
		void read_pod(T* object, u64 count)
		{
			static_assert(std::is_trivial_v<T>, "T must be a trivial type");
			for (int i = 0; i < count; i++)
			{
				m_Stream.read(reinterpret_cast<char*>(&object[i]), sizeof(T));
			}
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