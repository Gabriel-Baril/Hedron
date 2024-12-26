#pragma once
#include "inih/INIReader.h"
#include <filesystem>

namespace hdn
{
	class Configuration
	{
	private:
		Configuration(const std::filesystem::path& rootConfigPath);
	private:
		INIReader m_Reader;
	public:
		static Configuration& Get();
		static std::filesystem::path GetRootConfigPath();

		std::string GetRootConfigVariable(const std::string& section, const std::string& name, const std::string& defaultValue);
	};
}