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

	inline constexpr const char* CONFIG_SECTION_CACHE = "cache";
	inline constexpr const char* CONFIG_KEY_CACHE_FOLDER_PATH = "CacheFolderPath";

	inline constexpr const char* CONFIG_SECTION_PIPELINE = "pipeline";
	inline constexpr const char* CONFIG_KEY_PIPELINE_MODULE_PATH = "PipelineModulePath";
	inline constexpr const char* CONFIG_KEY_OBJECTS_FOLDER_NAME = "ObjectsFolderName";
	inline constexpr const char* CONFIG_KEY_OBJECTS_PATH = "ObjectsPath";

	class HDN_MODULE_CONFIG_API Configuration
	{
	private:
		Configuration(const fspath& rootConfigPath);
	private:
		INIReader m_Reader;
	public:
		static Configuration& get();
		static fspath get_root_config_path();

		string get_root_config_variable(const string& section, const string& name, const string& defaultValue);
	};
}