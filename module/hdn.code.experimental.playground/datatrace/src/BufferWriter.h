#pragma once

#include "core/Core.h"

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
		inline void Write(T* values, uint32 count)
		{
			const auto size = sizeof(T) * count;
			memcpy(m_CurrentPtr, values, size);
			m_CurrentPtr += size;
		}

		template<typename T>
		inline void Advance()
		{
			const auto size = sizeof(T);
			m_CurrentPtr += size;
		}

		template<typename T>
		inline void Advance(uint32 count)
		{
			const auto size = sizeof(T) * count;
			m_CurrentPtr += size;
		}

		template<typename T>
		inline T* Get()
		{
			return reinterpret_cast<T*>(m_CurrentPtr);
		}

		template<typename T>
		inline T* Base()
		{
			return reinterpret_cast<T*>(m_BufferBase);
		}

		inline uint64 BytesWritten(const byte* startOffset = nullptr) const
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
}