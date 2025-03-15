#pragma once

#include <ostream>

#include "core/core.h"
#include "dynamic_memory_buffer.h"

namespace hdn
{
	class hostream
	{
	public:
		hostream()
			: m_stream{ &m_Buffer }
		{
		}

		DynamicMemoryBuffer* get_stream_buffer()
		{
			return &m_Buffer;
		}

		void reset()
		{
			m_Buffer.getBuffer().clear();
		}

		u64 size() const
		{
			return m_Buffer.getBuffer().size();
		}

		const void* data() const
		{
			return m_Buffer.getBuffer().data();
		}

		auto begin()
		{
			return m_Buffer.getBuffer().begin();
		}

		auto end()
		{
			return m_Buffer.getBuffer().end();
		}

		template<typename T>
		u64 write_pod(const T& object)
		{
			static_assert(std::is_trivial_v<T>, "T must be a trivial type");
			const u64 byteSize = sizeof(T);
			m_stream.write(reinterpret_cast<const char*>(&object), byteSize);
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
			const byte* bytes = reinterpret_cast<const byte*>(data);
			m_stream.write(reinterpret_cast<const char*>(data), byteSize);
			return byteSize;
		}
	private:
		DynamicMemoryBuffer m_Buffer;
		std::ostream m_stream;
	};

	template<typename T>
	hostream& operator<<(hostream& stream, const T& object)
	{
		stream.write_pod(object);
		return stream;
	}
}