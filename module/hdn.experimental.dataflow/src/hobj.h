#pragma once

#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/hash.h"

#include "hkey.h"

#include "binary_io.h"
#include "buffer_writer.h"
#include "buffer_reader.h"
#include "random.h"

constexpr std::size_t strlen_ct(const char* str) {
	std::size_t length = 0;
	while (str[length] != '\0') {
		++length;
	}
	return length;
}

#define HOBJ_FILE_EXT ".ho"

namespace hdn
{
	static constexpr u64 HOBJ_FILE_MAGIC_NUMBER = 0x4A424F48;
	static constexpr u64 HOBJ_NULL_KEY = 0;

	enum class HObjectLoadState
	{
		Unloaded = 0,
		Virtualized = 1,
		Realized = 2
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectLoadState)

	enum class HObjectCreateFlags
	{
		Default = 0,
		GenerateUUID = (1 << 0),
		
		InitForLoad = Default,
		InitForCreate = GenerateUUID
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectCreateFlags)

	enum class HObjectLoadFlags
	{
		Default = 0,
		Realize = (1 << 0)
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectLoadFlags)

	enum class HObjectSaveFlags
	{
		Default = 0
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectSaveFlags)

	template<typename T> using HObjPtr = T*;

	class HObject
	{
	public:
		virtual void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default)
		{
			MAYBE_UNUSED(flags);
			archive.Advance<u64>(); // Skip magic number
			archive.Advance<hash64_t>(); // The first bytes always contains the serialized object type, since we don't need to them for loading, skip them
			bin::Read(archive, m_Key);
			bin::Read(archive, m_Path);
		}

		virtual void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default)
		{
			MAYBE_UNUSED(flags);
			u64 typeHash = GetTypeHash();
			bin::Write(archive, HOBJ_FILE_MAGIC_NUMBER);
			bin::Write(archive, typeHash);
			bin::Write(archive, m_Key);
			bin::Write(archive, m_Path);
		}

		inline virtual hash64_t GetTypeHash() const { return GenerateTypeHash<HObject>(); }

		virtual void Realize() {}

		hkey GetKey() const { return m_Key; }
		string GetPath() const { return m_Path; }

		virtual ~HObject()
		{
			HDEBUG("Freeing object '{0}'", m_Path.c_str());
		}
	protected:
		HObject()
			: m_Key{ HOBJ_NULL_KEY }, m_LoadState{ HObjectLoadState::Unloaded }
		{
		}
	private:
		void SetKey(hkey key)
		{
			m_Key = key;
		}

		void SetPath(const string& path)
		{
			m_Path = path;
		}

		void SetLoadState(HObjectLoadState state)
		{
			m_LoadState = state;
		}
	private:
		hkey m_Key = HOBJ_NULL_KEY;
		string m_Path = ""; // TODO: Make this field available only in debug mode?

		// Transient
		HObjectLoadState m_LoadState = HObjectLoadState::Unloaded;

		friend class HObjectUtil;
	};
}