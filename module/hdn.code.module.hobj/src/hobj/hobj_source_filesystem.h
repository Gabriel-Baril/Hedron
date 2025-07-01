#pragma once

#include "hobj_source.h"
#include "core/stl/unordered_map.h"

namespace hdn
{
	struct HObjectFilesystemData
	{
		string path;
	};

	bool is_object_file(const fspath& path);

	class FilesystemObjectSource : public IHObjectSource
	{
	public:
		FilesystemObjectSource(const string& sourcePath);
		virtual HObject* get(uuid64 id) override;

		void load(const char* path, HObject* outObject);
		virtual void load(uuid64 id, HObject* outObject) override;

		void unload(uuid64 id);

		virtual bool save(HObject* object, const void* userData, u64 userDataByteSize) override;
		virtual bool del(uuid64 id) override;
		virtual bool loaded(uuid64 id) override;
		virtual void populate(HObjectRegistry* registry) override;
		void manifest_create_entry(uuid64 id, const fspath& path);
	private:
		string m_SourcePath;
		unordered_map<uuid64, HObject*> m_LoadedObjects{};
		unordered_map<uuid64, fspath> m_ObjectPaths{};
	};
}