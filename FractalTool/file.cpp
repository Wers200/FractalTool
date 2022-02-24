#include "framework.h"
#include "file.h"

std::vector<char> readBytes(std::string filename) {
	std::ifstream ifs(std::filesystem::absolute(std::filesystem::u8path(filename)).u8string().c_str(), std::ios::binary | std::ios::ate);

	std::streampos end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	size_t size = std::size_t(end - ifs.tellg());

	std::vector<char> buf(size);

	ifs.read(buf.data(), size);

	return buf;
}