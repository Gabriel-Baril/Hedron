#include "config.h"
#include "core/core.h"

#include <regex>

namespace hdn
{
	Configuration::Configuration(const std::filesystem::path& rootConfigPath)
		: m_Reader{ rootConfigPath.string() }
	{
	}

	Configuration& Configuration::get()
	{
		static Configuration s_Instance{ get_root_config_path() };
		return s_Instance;
	}

	std::filesystem::path Configuration::get_root_config_path()
	{
		const char* root = std::getenv(HDN_ROOT_ENV);
		const char* configName = std::getenv(HDN_ROOT_CONFIG_NAME_ENV);
		std::filesystem::path rootPath = root;
		return rootPath / configName;
	}

	string Configuration::get_root_config_variable(const string& section, const string& name, const string& defaultValue)
	{
		string result = m_Reader.Get(section, name, defaultValue);
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
				string rep = get_root_config_variable(matchedSection, matchedKey, "");
				HASSERT(rep != "", "Configuration Variable '[{0}]{1}' not found while parsing '{2}'", matchedSection.c_str(), matchedKey.c_str(), result.c_str());
				replacement = rep;
			}
			result.replace(match.position(0), fullMatch.length(), replacement);
		}
		return result;
	}
}
