#pragma once

#include "core/stl/vector.h"

#include <stack>

#include "buffer_writer.h"

namespace hdn
{
	enum class KeyValueType : u8
	{
		INT32 = 0,
		UINT32 = 1,
		STRING = 2
	};

	class GroupData
	{
	public:
		GroupData() = default;

		void PushChildren(size_t childIndex)
		{
			Children.push_back(childIndex);
		}

		const vector<size_t>& GetChildren() const
		{
			return Children;
		}

		void SetSemantic(const char* semantic)
		{
			this->Semantic = semantic;
		}

		const char* GetSemantic() const
		{
			return Semantic;
		}

		inline u64 GetBeginByte() const { return BeginByte; }
		inline u64 GetEndByte() const { return EndByte; }
		inline u64 GetBeginByteMeta() const { return BeginByteMeta; }
		inline u64 GetEndByteMeta() const { return EndByteMeta; }

		void SetBeginByte(u64 beginByte)
		{
			this->BeginByte = beginByte;
		}

		void SetEndByte(u64 endByte)
		{
			this->EndByte = endByte;
		}

		void SetBeginByteMeta(u64 beginByteMeta)
		{
			this->BeginByteMeta = beginByteMeta;
		}

		void SetEndByteMeta(u64 endByteMeta)
		{
			this->EndByteMeta = endByteMeta;
		}

		u64 MetadataByteSize() const
		{
			HASSERT(EndByteMeta >= BeginByteMeta, "EndByteMeta < BeginByteMeta");
			return EndByteMeta - BeginByteMeta;
		}
	private:
		u64 BeginByte = 0;
		u64 EndByte = 0;
		u64 BeginByteMeta = 0;
		u64 EndByteMeta = 0;
		const char* Semantic = nullptr; // This field could more complex metadata if we want/need to
		vector<size_t> Children = {};
	};

	class FGroupBufferWriter : public FBufferWriter
	{
	public:
		FGroupBufferWriter(byte* buffer, byte* metaBuffer)
			: FBufferWriter{ buffer }, m_MetadataBuffer{ metaBuffer }, m_GroupDataVector {
		}
		{
		}

		void Push(const char* semantic)
		{
			GroupData data;
			data.SetBeginByte(BytesWritten());
			data.SetBeginByteMeta(m_MetadataBuffer.BytesWritten());
			data.SetSemantic(semantic);
			size_t newGroupIndex = m_GroupDataVector.size();
			m_GroupDataVector.push_back(data);
			if (!m_GroupDataStack.empty())
			{
				GroupData& parentGroup = m_GroupDataVector.at(m_GroupDataStack.top());
				parentGroup.PushChildren(newGroupIndex);
			}
			m_GroupDataStack.push(newGroupIndex);
		}

		template<typename T>
		void Meta(KeyValueType type, const char* key, const T& value)
		{
			static_assert(std::is_trivial_v<T>, "FGroupBufferWriter::Meta needs to take a trivial type");
			const size_t keyLen = strlen(key);

			// [type][key_len_byte][key][value_len_byte][value]
			m_MetadataBuffer.Write(Underlying(type));
			m_MetadataBuffer.Write(keyLen);
			m_MetadataBuffer.Write(key, keyLen);
			m_MetadataBuffer.Write(sizeof(value));
			m_MetadataBuffer.Write(value);
		}

		void Meta(KeyValueType type, const char* key, const char* value)
		{
			const size_t keyLen = strlen(key);
			const size_t valueLen = strlen(value);

			// [type][key_len_byte][key][value_len_byte][value]
			m_MetadataBuffer.Write(Underlying(type));
			m_MetadataBuffer.Write(keyLen);
			m_MetadataBuffer.Write(key, keyLen);
			m_MetadataBuffer.Write(valueLen);
			m_MetadataBuffer.Write(value, valueLen);
		}

		void Pop()
		{
			GroupData& group = m_GroupDataVector.at(m_GroupDataStack.top());
			group.SetEndByte(BytesWritten());
			group.SetEndByteMeta(m_MetadataBuffer.BytesWritten());
			m_GroupDataStack.pop();
		}

		const vector<GroupData>& GetGroups() const
		{
			return m_GroupDataVector;
		}

		virtual ~FGroupBufferWriter() = default;
	private:
		std::stack<size_t> m_GroupDataStack;
		vector<GroupData> m_GroupDataVector;
		FBufferWriter m_MetadataBuffer;
	};

	namespace bin
	{
		template<typename T>
		void Meta(FGroupBufferWriter& writer, KeyValueType type, const char* key, const T& value)
		{
			writer.Meta(type, key, value);
		}

		template<typename T>
		inline void Write(FGroupBufferWriter& writer, const T& object, const char* semantic)
		{
			writer.Push(semantic);
			Write(writer, object);
			writer.Pop();
		}

		void Push(FGroupBufferWriter& writer, const char* semantic);
		void Pop(FGroupBufferWriter& writer);
	}
}