#pragma once
#include "config_internal_api.h"

#include "core/core_filesystem.h"

#include "inih/INIReader.h"

namespace hdn
{
	class HDN_MODULE_CONFIG_API Configuration
	{
	private:
		Configuration(const fspath& rootConfigPath);
	private:
		INIReader m_Reader;
	public:
		static Configuration& Get();
		static fspath GetRootConfigPath();

		string GetRootConfigVariable(const string& section, const string& name, const string& defaultValue);
	};
}