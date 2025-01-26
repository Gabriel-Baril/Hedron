#pragma once

#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/common.h"
#include "core/io/buffer_writer.h"

namespace hdn
{
	class FDynamicBufferWriter
	{
	public:
		FDynamicBufferWriter(u64 tempBufferSize, u64 reservedSize = 16)
			: m_TempBufferSize{ tempBufferSize }
		{
			m_Vector.reserve(reservedSize);
			m_TempBuffer = new byte[m_TempBufferSize];
		}

		template<typename T>
		inline void Write(const T& value)
		{
			FBufferWriter writer(m_TempBuffer);
			bin::Write(writer, value);
			m_Vector.insert(m_Vector.end(), writer.begin<byte>(), writer.end<byte>());
		}

		template<typename T>
		inline T* Write(T* values, size_t count)
		{
			FBufferWriter writer(m_TempBuffer);
			writer.Write<T>(values, count);
			m_Vector.insert(m_Vector.end(), writer.begin<byte>(), writer.end<byte>());
		}

		template<typename T>
		inline T* end()
		{
			return m_Vector.data() + m_Vector.size();
		}

		template<typename T>
		inline T* begin()
		{
			return m_Vector.data();
		}

		void Reset()
		{
			m_Vector.clear();
		}

		~FDynamicBufferWriter()
		{
			delete[] m_TempBuffer;
		}
	private:
		vector<byte> m_Vector;
		byte* m_TempBuffer;
		u64 m_TempBufferSize;
	};

	namespace bin
	{
		template<typename T>
		inline void Write(FDynamicBufferWriter& writer, const T& object)
		{
			writer.Write(object);
		}
	}
}