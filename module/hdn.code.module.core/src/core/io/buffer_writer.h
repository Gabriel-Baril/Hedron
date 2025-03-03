#pragma once

#include "core/core.h"
#include "common.h"

namespace hdn
{

	class FBufferWriter
	{
	public:
		FBufferWriter(byte* buffer)
			: m_BufferBase{ buffer }, m_CurrentPtr{ buffer }
		{
		}

		FBufferWriter()
		{
			SetBase(nullptr);
		}
	public:
		void SetBase(byte* buffer)
		{
			m_BufferBase = buffer;
			m_CurrentPtr = m_BufferBase;
		}

		inline void Copy(const FBufferWriter& writer)
		{
			const auto size = writer.BytesWritten();
			memcpy(m_CurrentPtr, writer.m_BufferBase, size);
			m_CurrentPtr += size;
		}

		template<typename T>
		inline void Write(const T& value)
		{
			const auto size = sizeof(T);
			memcpy(m_CurrentPtr, &value, size);
			m_CurrentPtr += size;
		}

		template<typename T>
		inline T* Write(T* values, size_t count)
		{
			T* base = end<T>();
			const auto size = sizeof(T) * count;
			memcpy(m_CurrentPtr, values, size);
			m_CurrentPtr += size;
			return base;
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
		inline T* end()
		{
			return reinterpret_cast<T*>(m_CurrentPtr);
		}

		template<typename T>
		inline T* begin()
		{
			return reinterpret_cast<T*>(m_BufferBase);
		}

		inline u64 BytesWritten(const byte* startOffset = nullptr) const
		{
			if (startOffset == nullptr)
			{
				return m_CurrentPtr - m_BufferBase;
			}
			return m_CurrentPtr - startOffset;
		}

		inline bool ValidBase()
		{
			return m_BufferBase != nullptr;
		}

		virtual ~FBufferWriter() = default;
	protected:
		byte* m_BufferBase = nullptr;
		byte* m_CurrentPtr = nullptr;
	};

	namespace bin
	{
		template<Primitive T>
		inline void Write(FBufferWriter& writer, const T& object)
		{
			writer.Write(object);
		}

		void Write(FBufferWriter& writer, const char& object);
		void Write(FBufferWriter& writer, const string& object);
	}
}