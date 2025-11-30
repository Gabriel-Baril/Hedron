#pragma once
#include "config_internal_api.h"

#include "core/core_filesystem.h"

#include "inih/INIReader.h"

namespace hdn
{
	inline constexpr const char* HDN_ROOT_ENV = "HDN_ROOT";
	inline constexpr const char* HDN_ROOT_CONFIG_NAME_ENV = "HDN_ROOT_CONFIG_NAME";

	inline constexpr const char* CONFIG_SECTION_PATH = "path";
	inline constexpr const char* CONFIG_KEY_SCRIPT_FOLDER_PATH = "ScriptFolderPath";
	inline constexpr const char* CONFIG_KEY_MODULE_FOLDER_PATH = "ModuleFolderPath";

	inline constexpr const char* CONFIG_SECTION_TEST = "test";
	inline constexpr const char* CONFIG_KEY_TEST_SOLUTION_PATH = "TestSolutionPath";
	inline constexpr const char* CONFIG_KEY_BUILD_TEST_PROJECT_SCRIPT_PATH = "BuildTestProjectScriptPath";
	inline constexpr const char* CONFIG_KEY_EXECUTABLE_LIST_FILENAME = "ExecutableListFile";
	inline constexpr const char* CONFIG_KEY_EXECUTABLE_LIST_PATH = "ExecutableListFilePath";

	inline constexpr const char* CONFIG_SECTION_PIPELINE = "pipeline";
	inline constexpr const char* CONFIG_KEY_CACHE_MODULE_PATH = "CacheModulePath";
	inline constexpr const char* CONFIG_KEY_CACHE_FOLDER_NAME = "ObjectCacheFolderName";
	inline constexpr const char* CONFIG_KEY_CACHE_PATH = "ObjectCachePath";

	inline constexpr const char* CONFIG_SECTION_DATA = "data";
	inline constexpr const char* CONFIG_KEY_DATA_MODULE_PATH = "DataModulePath";

	void config_init();
	fspath config_get_root_config_path();
	string config_get_config_variable(const string& section, const string& name, const string& defaultValue);
}