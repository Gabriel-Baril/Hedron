#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/unordered_map.h"

#include <chrono>
#include <thread>

namespace fs = std::filesystem;

namespace hdn
{
	void monitorFolder(const string& path) {
		// Store the last modification times of files in the directory
		unordered_map<string, std::filesystem::file_time_type> fileModificationTimes;

		while (true) {
			try {
				for (const auto& entry : fs::directory_iterator(path)) {
					const auto& filePath = entry.path().string();
					const auto& lastWriteTime = fs::last_write_time(entry);

					if (fileModificationTimes.find(filePath) == fileModificationTimes.end()) {
						// New file detected
						HWARN("File Created: {0}", filePath);
					}
					else if (fileModificationTimes[filePath] != lastWriteTime) {
						// File modified
						HWARN("File Modified: {0}", filePath);
					}

					fileModificationTimes[filePath] = lastWriteTime;
				}

				// Check for deleted files
				for (auto it = fileModificationTimes.begin(); it != fileModificationTimes.end();) {
					if (!fs::exists(it->first)) {
						HWARN("File Deleted: {0}", it->first);
						it = fileModificationTimes.erase(it);
					}
					else {
						++it;
					}
				}
			}
			catch (const fs::filesystem_error& e) {
				HERR("Error: {0}", e.what());
			}

			// Wait before re-checking (polling interval)
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

int main()
{
	using namespace hdn;
	Log_Init();

	string folderToMonitor = "D:/CLOUD/OneDrive/DEV/HEDRON/module/hdn.solution.playground/data";
	HINFO("Monitoring folder: {0}", folderToMonitor.c_str());
	
	monitorFolder(folderToMonitor);

	while (true) {}
}