#include "utils.h"
#include "core/config/config.h"

namespace hdn
{
	string get_data_path(const string& relativeDataPath)
	{
		std::string dataModulePath = Configuration::get().get_root_config_variable(CONFIG_SECTION_DATA, CONFIG_KEY_DATA_MODULE_PATH, "");
		return fmt::format("{0}\\{1}", dataModulePath, relativeDataPath);
	}

}
