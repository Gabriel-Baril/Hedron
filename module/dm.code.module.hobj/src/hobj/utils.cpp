#include "utils.h"

namespace hdn
{
	string get_data_path(const string& relativeDataPath)
	{
		std::string dataModulePath = config_get_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");
		return fmt::format("{0}\\{1}", dataModulePath, relativeDataPath);
	}
}
