#pragma once

#include "hobj_source.h"
#include "core/stl/unordered_map.h"

namespace hdn
{
	bool IsObjectFile(const fspath& path)
	{
		return FileSystem::Extension(path) == HOBJ_FILE_EXT;
	}

	struct HObjectFilesystemData
	{
		string path;
	};

	class FilesystemObjectSource : public IHObjectSource
	{
	public:
		FilesystemObjectSource(const string& sourcePath);
		virtual HObject* Get(huid_t id) override;
		virtual void Load(huid_t id, HObject* outObject) override;
		virtual bool Save(HObject* object, const void* userData, u64 userDataByteSize) override;
		virtual bool Delete(huid_t id) override;
		virtual bool Loaded(huid_t id) override;
		virtual void Walk(HObjectRegistry* registry) override;
		void AddToManifest(huid_t id, const fspath& path);
	private:
		string m_SourcePath;
		unordered_map<huid_t, HObject*> m_LoadedObjects{};
		unordered_map<huid_t, fspath> m_ObjectPaths{};
	};
}