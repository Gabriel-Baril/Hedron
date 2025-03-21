#pragma once

#include "hobj_source.h"
#include "core/stl/unordered_map.h"

namespace hdn
{
	struct HObjectFilesystemData
	{
		string path;
	};

	bool IsObjectFile(const fspath& path);

	class FilesystemObjectSource : public IHObjectSource
	{
	public:
		FilesystemObjectSource(const string& sourcePath);
		virtual HObject* Get(huid_t id) override;

		void Load(const char* path, HObject* outObject);
		virtual void Load(huid_t id, HObject* outObject) override;

		void Unload(huid_t id);

		virtual bool Save(HObject* object, const void* userData, u64 userDataByteSize) override;
		virtual bool Delete(huid_t id) override;
		virtual bool Loaded(huid_t id) override;
		virtual void Populate(HObjectRegistry* registry) override;
		void ManifestCreateEntry(huid_t id, const fspath& path);
	private:
		string m_SourcePath;
		unordered_map<huid_t, HObject*> m_LoadedObjects{};
		unordered_map<huid_t, fspath> m_ObjectPaths{};
	};
}