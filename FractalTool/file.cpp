#include "framework.h"
#include "file.h"

std::vector<char> readBytes(std::string filename) {
	std::filesystem::path temp1 = std::filesystem::u8path(filename);
	std::filesystem::path temp2 = std::filesystem::absolute(temp1);
	std::string str = temp2.u8string();
	std::ifstream ifs(str.c_str(), std::ios::binary | std::ios::ate);

	std::streampos end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	size_t size = std::size_t(end - ifs.tellg());

	std::vector<char> buf(size);

	ifs.read(buf.data(), size);

	return buf;
}