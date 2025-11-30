#include "config.h"
#include "core/core.h"
#include "core/ini/ini.h"

namespace dm
{
	struct ConfigurationGlob
	{
		INIReader *reader = nullptr;
		bool initialized = false;
	} s_Config;

	static bool config_initialized()
	{
		return s_Config.initialized;
	}

	void config_init()
	{
		if (config_initialized())
		{
			return;
		}

		static INIReader reader{config_get_root_config_path().string()};
		s_Config.reader = &reader;
		s_Config.initialized = true;
	}

	fspath config_get_root_config_path()
	{
		const char *root = std::getenv(DM_ROOT_ENV);
		const char *configName = std::getenv(DM_ROOT_CONFIG_NAME_ENV);
		fspath rootPath = root;
		return rootPath / configName;
	}

	string config_get_config_variable(const string &section, const string &name, const string &defaultValue)
	{
		DM_CORE_ASSERT(config_initialized(), "Config needs to be initialized");

		INIReader *reader = s_Config.reader;
		return ini_get_variable(*reader, section, name, defaultValue);
	}
}
