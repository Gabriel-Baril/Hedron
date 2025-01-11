#include "core/core.h"
#include "core/core_filesystem.h"


#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

void monitorFolder(const std::string& path) {
	// Store the last modification times of files in the directory
	std::unordered_map<std::string, std::filesystem::file_time_type> fileModificationTimes;

	while (true) {
		try {
			for (const auto& entry : fs::directory_iterator(path)) {
				const auto& filePath = entry.path().string();
				const auto& lastWriteTime = fs::last_write_time(entry);

				if (fileModificationTimes.find(filePath) == fileModificationTimes.end()) {
					// New file detected
					std::cout << "File created: " << filePath << "\n";
				}
				else if (fileModificationTimes[filePath] != lastWriteTime) {
					// File modified
					std::cout << "File modified: " << filePath << "\n";
				}

				fileModificationTimes[filePath] = lastWriteTime;
			}

			// Check for deleted files
			for (auto it = fileModificationTimes.begin(); it != fileModificationTimes.end();) {
				if (!fs::exists(it->first)) {
					std::cout << "File deleted: " << it->first << "\n";
					it = fileModificationTimes.erase(it);
				}
				else {
					++it;
				}
			}
		}
		catch (const fs::filesystem_error& e) {
			std::cerr << "Error: " << e.what() << "\n";
		}

		// Wait before re-checking (polling interval)
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
{
	using namespace hdn;
	Log_Init();

	std::string folderToMonitor = "D:/_DEV/_HEDRON/module/hdn.solution.playground/data";
	HINFO("Monitoring folder: {0}", folderToMonitor.c_str());
	
	monitorFolder(folderToMonitor);

	while (true) {}
}