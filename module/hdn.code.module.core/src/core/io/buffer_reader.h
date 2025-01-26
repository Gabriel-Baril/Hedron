#pragma once

#include "core/core.h"
#include "common.h"

namespace hdn
{
	class FBufferReader
	{
	public:
		FBufferReader(const byte* buffer);

		// Same as read but does not increment the read pointer
		template<typename T>
		inline T Look()
		{
			const auto size = sizeof(T);
			const T* out = reinterpret_cast<const T*>(m_CurrentPtr);
			return *out;
		}

		template<typename T>
		inline void Read(T* out)
		{
			const auto size = sizeof(T);
			*out = *reinterpret_cast<const T*>(m_CurrentPtr);
			m_CurrentPtr += size;
		}

		template<typename T>
		inline T Read()
		{
			const auto size = sizeof(T);
			const T* out = reinterpret_cast<const T*>(m_CurrentPtr);
			m_CurrentPtr += size;
			return *out;
		}

		template<typename T>
		inline const T* Read(size_t count)
		{
			const auto size = sizeof(T) * count;
			const T* out = reinterpret_cast<const T*>(m_CurrentPtr);
			m_CurrentPtr += size;
			return out;
		}

		template<typename T>
		inline void Backtrack()
		{
			const auto size = sizeof(T);
			m_CurrentPtr -= size;
		}

		template<typename T>
		inline void Advance()
		{
			const auto size = sizeof(T);
			m_CurrentPtr += size;
		}

		template<typename T>
		inline void Advance(u32 count)
		{
			const auto size = sizeof(T) * count;
			m_CurrentPtr += size;
		}

		template<typename T>
		inline const T* Get()
		{
			return reinterpret_cast<const T*>(m_CurrentPtr);
		}

		template<typename T>
		inline const T* Base()
		{
			return reinterpret_cast<const T*>(m_BufferBase);
		}

		inline bool ValidBase()
		{
			return m_BufferBase != nullptr;
		}

		~FBufferReader() = default;
	private:
		const byte* m_BufferBase = nullptr;
		const byte* m_CurrentPtr = nullptr;
	};

	namespace bin
	{
		template<Primitive T>
		inline void Read(FBufferReader& reader, T& object)
		{
			reader.Read(&object);
		}

		void Read(FBufferReader& reader, string& object);
	}
}