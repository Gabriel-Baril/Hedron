#include "config.h"
#include "core/core.h"

#include <regex>

namespace hdn
{
	struct ConfigurationGlob
	{
		INIReader* reader = nullptr;
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

		static INIReader reader{ config_get_root_config_path().string() };
		s_Config.reader = &reader;
		s_Config.initialized = true;
	}

	fspath config_get_root_config_path()
	{
		const char* root = std::getenv(HDN_ROOT_ENV);
		const char* configName = std::getenv(HDN_ROOT_CONFIG_NAME_ENV);
		fspath rootPath = root;
		return rootPath / configName;
	}

	string config_get_root_config_variable(const string& section, const string& name, const string& defaultValue)
	{
		HASSERT(config_initialized(), "Config needs to be initialized");

		INIReader* reader = s_Config.reader;

		string result = reader->Get(section, name, defaultValue);
		HASSERT(result != defaultValue, "Configuration Variable '[{0}]{1}' not found", section.c_str(), name.c_str());
		std::regex variablePattern(R"(\$\{([^:}]+)(?::([^}]+))?\})"); // Regex to match ${prefix:key} or ${key} (environment variable)
		std::smatch match;
		while (std::regex_search(result, match, variablePattern)) {
			string fullMatch = match[0].str();
			string matchedSection = match[1].str(); // Prefix or key if no colon
			string matchedKey = match[2].str();    // Key (optional)
			string replacement;
			if (matchedKey.empty())
			{
				// No prefix: This is an environment variable
				const char* envValue = std::getenv(matchedSection.c_str());
				HASSERT(envValue != nullptr, "Environment variable '{0}' not found while parsing '{1}'", matchedSection.c_str(), result.c_str());
				replacement = envValue;
			}
			else
			{
				string rep = config_get_root_config_variable(matchedSection, matchedKey, "");
				HASSERT(rep != "", "Configuration Variable '[{0}]{1}' not found while parsing '{2}'", matchedSection.c_str(), matchedKey.c_str(), result.c_str());
				replacement = rep;
			}
			result.replace(match.position(0), fullMatch.length(), replacement);
		}
		return result;
	}
}
