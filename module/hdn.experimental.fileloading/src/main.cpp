#include "core/core.h"
#include "core/core_filesystem.h"

int main()
{
	using namespace hdn;
	Log_Init();


	const std::string filename = "example.txt";  // Replace with your file
	const std::streampos offset = 8;  // Offset in bytes
	const std::size_t size = 10;  // Number of bytes to read

	std::ifstream file(filename, std::ios::binary);  // Open file in binary mode
	if (!file) {
		HERR("Error: Could not open file");
		return 1;
	}

	file.seekg(offset, std::ios::beg);  // Move read position to the offset
	if (!file) {
		HERR("Error: Seek failed");
		return 1;
	}

	std::vector<char> buffer(size);  // Allocate buffer
	file.read(buffer.data(), size);  // Read data into buffer
	if (!file) {
		HERR("Error: Read failed (may have reached EOF)");
	}

	file.close();

	// Print first few bytes as hex
	HINFO("Read {0} bytes:", file.gcount());
	buffer.push_back('\0');
	HINFO("Data: {0}", (char*)buffer.data())

	return 0;
}