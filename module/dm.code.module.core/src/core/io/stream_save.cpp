#include "stream_save.h"

namespace hdn
{
	bool save_binary_to_file(const char* savePath, const void* data, u64 size)
	{
		filesystem_touch(savePath);

		std::ofstream outFile(savePath, std::ios::binary);
		if (!outFile)
		{
			HDN_ERROR_LOG("Could not open file '{0}' for writing", savePath);
			return false;
		}

		outFile.write(reinterpret_cast<const char*>(data), size);
		outFile.close();
		if (outFile.fail())
		{
			HDN_ERROR_LOG("Failed to write to file '{0}'", savePath);
			return false;
		}
		return true;
	}
}
