#pragma once

#include <ostream>

#include "core/core.h"
#include "stream_memory.h"

namespace hdn
{
	class hostream
	{
	public:
		hostream()
		{
		}

		void reset()
		{
			m_Buffer.clear();
		}

		u64 size() const
		{
			return m_Buffer.size();
		}

		const void* data() const
		{
			return m_Buffer.data();
		}

		auto begin()
		{
			return m_Buffer.begin();
		}

		auto end()
		{
			return m_Buffer.end();
		}

		template<typename T>
		u64 write_pod(const T& object)
		{
			static_assert(std::is_trivial_v<T>, "T must be a trivial type");
			const u64 byteSize = sizeof(T);
			write(&object, byteSize);
			return byteSize;
		}

		template<typename T>
		u64 write_pod(const T* object, u64 count)
		{
			static_assert(std::is_trivial_v<T>, "T must be a trivial type");
			const u64 byteSize = sizeof(T) * count;
			write(object, byteSize);
			return byteSize;
		}

		u64 write(const void* data, u64 byteSize)
		{
			const u8* bytes = (const u8*)data;
			m_Buffer.insert(m_Buffer.end(), bytes, bytes + byteSize);
			return byteSize;
		}

		u64 write_at(const void* data, u64 byteSize, u64 offset)
		{
			if (offset + byteSize > m_Buffer.size())
			{
				HDN_WARNING_LOG("Failed to write {0} bytes to buffer", byteSize);
				return 0;
			}
			std::memcpy(m_Buffer.data() + offset, data, byteSize);
		}

		u64 marker() const
		{
			return size();
		}
	private:
		std::vector<u8>  m_Buffer;
	};

	template<typename T>
	hostream& operator<<(hostream& stream, const T& object)
	{
		stream.write_pod(object);
		return stream;
	}
}