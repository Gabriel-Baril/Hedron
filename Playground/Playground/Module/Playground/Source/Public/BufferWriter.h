#pragma once

#include <stack>
#include <vector>

using byte = char;
using uint32 = unsigned int;
using uint64 = unsigned long long;

struct GroupMetadata
{
	enum Type
	{
		INT32 = 0,
		UINT32 = 1,
		STRING = 2
	};

	template<typename T>
	void Meta(Type type, const char* key, const T& value)
	{
		const uint keySize = strlen(key);
		writer.Write(keySize);
		writer.Write(key, keySize);
		writer.Write(type);
		writer.Write(sizeof(T)); // Not really necessary but can be useful
		writer.Write(value);
	}

	BufferWriter writer; // TODO: Fix
};

class GroupData
{
public:
	GroupData() = default;

	void PushChildren(uint32 childIndex)
	{
		Children.push_back(childIndex);
	}

	const std::vector<uint32>& GetChildren() const
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

	void SetBeginByte(uint64 beginByte)
	{
		this->BeginByte = beginByte;
	}

	void SetEndByte(uint64 endByte)
	{
		this->EndByte = endByte;
	}
private:
	uint64 BeginByte = 0;
	uint64 EndByte = 0;
	const char* Semantic = nullptr; // This field could more complex metadata if we want/need to
	GroupMetadata meta;
	std::vector<uint32> Children = {};
};

// Create SimpleBufferWriter with the Push/Pop logic
class BufferWriter
{
public:
	BufferWriter(byte* buffer)
		: m_BufferBase{ buffer }, m_CurrentPtr{ buffer }, m_GroupDataVector{}
	{
	}
	
	void PushGroup(const char* semantic)
	{
		GroupData data;
		data.SetBeginByte(BytesWritten());
		data.SetSemantic(semantic);
		int newGroupIndex = m_GroupDataVector.size();
		m_GroupDataVector.push_back(data);
		if (!m_GroupDataStack.empty())
		{
			GroupData& parentGroup = m_GroupDataVector.at(m_GroupDataStack.top());
			parentGroup.PushChildren(newGroupIndex);
		}
		m_GroupDataStack.push(newGroupIndex);
	}

	void PopGroup()
	{
		GroupData& group = m_GroupDataVector.at(m_GroupDataStack.top());
		group.SetEndByte(BytesWritten());
		m_GroupDataStack.pop();
	}

	const std::vector<GroupData>& GetGroups()
	{
		return m_GroupDataVector;
	}

	void SetBase(byte* buffer)
	{
		m_BufferBase = buffer;
		m_CurrentPtr = m_BufferBase;
	}

	inline void Copy(const BufferWriter& writer)
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

	~BufferWriter() = default;
private:
	byte* m_BufferBase = nullptr;
	byte* m_CurrentPtr = nullptr;

	std::stack<uint32> m_GroupDataStack;
	std::vector<GroupData> m_GroupDataVector;
};