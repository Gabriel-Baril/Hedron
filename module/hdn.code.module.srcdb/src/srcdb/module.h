#pragma once

#include "core/core.h"

// A module refer to a folder in \module containing a .module file. This file offer reflection mechanisms on modules.

namespace hdn
{
	using hmod_t = u64;

	struct Module
	{
		hstring name;
		hstring author;
		u64 version;
		ModuleProvenance provenance;
		ModuleKind kind;
		fspath basePath;
	};

	void module_init(const char* modulePath);
	void module_load(const char* name);

	Module* module_get(const char* name);
	Module* module_get(hmod_t mod);
	const fspath& module_base_path(hmod_t mod);

}