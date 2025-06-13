#include "scene_load.h"
#include <iosfwd>
#include <vector>
#include "core/io/stream_read.h"

namespace hdn
{

	static void parse_transform_component()
	{

	}

	// TODO: Move to hobj system
	static void load_scene_file(const char* scenePath, flecs::world ecsWorld)
	{
		std::ifstream inFile(scenePath, std::ios::binary | std::ios::ate);
		if (!inFile) {
			HERR("Could not open file '{0}' for reading", scenePath);
			return;
		}

		// Get the file size
		std::streamsize fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Create a buffer of the appropriate size
		std::vector<char> buffer(fileSize);

		// Read the file into the buffer
		if (!inFile.read(buffer.data(), fileSize)) {
			HERR("Failed to read the file", scenePath);
			return;
		}
		inFile.close();
		DynamicMemoryBuffer dynamicBuffer{ buffer };
		dynamicBuffer.reset();
		histream stream{ &dynamicBuffer };

		
	}

	void load_scene(flecs::world ecsWorld)
	{
		load_scene_file("scene.pfb", ecsWorld);
	}
}
