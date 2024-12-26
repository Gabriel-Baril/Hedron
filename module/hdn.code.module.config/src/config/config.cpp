#include "config.h"
#include "core/core.h"

#include <regex>

namespace hdn
{
	Configuration::Configuration(const std::filesystem::path& rootConfigPath)
		: m_Reader{ rootConfigPath.string() }
	{
	}

	Configuration& Configuration::Get()
	{
		static Configuration s_Instance{ GetRootConfigPath() };
		return s_Instance;
	}

	std::filesystem::path Configuration::GetRootConfigPath()
	{
		const char* root = std::getenv("HDN_ROOT");
		const char* configName = std::getenv("HDN_ROOT_CONFIG_NAME");
		std::filesystem::path rootPath = root;
		return rootPath / configName;
	}

	std::string Configuration::GetRootConfigVariable(const std::string& section, const std::string& name, const std::string& defaultValue)
	{
		std::string result = m_Reader.Get(section, name, defaultValue);
		HASSERT(result != defaultValue, "Configuration Variable '[{0}]{1}' not found", section.c_str(), name.c_str());
		std::regex variablePattern(R"(\$\{([^:}]+)(?::([^}]+))?\})"); // Regex to match ${prefix:key} or ${key} (environment variable)
		std::smatch match;
		while (std::regex_search(result, match, variablePattern)) {
			std::string fullMatch = match[0].str();
			std::string section = match[1].str(); // Prefix or key if no colon
			std::string key = match[2].str();    // Key (optional)
			std::string replacement;
			if (key.empty())
			{
				// No prefix: This is an environment variable
				const char* envValue = std::getenv(section.c_str());
				HASSERT(envValue != nullptr, "Environment variable '{0}' not found while parsing '{1}'", section.c_str(), result.c_str());
				replacement = envValue;
			}
			else
			{
				std::string rep = GetRootConfigVariable(section, key, "");
				HASSERT(rep != "", "Configuration Variable '[{0}]{1}' not found while parsing '{2}'", section.c_str(), key.c_str(), result.c_str());
				replacement = rep;
			}
			result.replace(match.position(0), fullMatch.length(), replacement);
		}
		return result;
	}
}
