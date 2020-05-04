#pragma once
#include "../lib/zlib/cpp/zlib.h"

#include <vector>
#include <string>
#include <fstream>

const int MAXDECOMPRESSED = 0x1400000;

struct TextureTable {
	uint8_t filenameSize;
	std::wstring filename;
	uint32_t offset;
	uint32_t pad;
	uint32_t dataSize;
};

class Texture {
public:
	Texture(std::string file, std::string table);
	~Texture();

	void extract();
private:
	std::ifstream file;
	std::ifstream table;
	std::vector<TextureTable> fileInfo;

	void parseTable();
	void setFile(std::string file);
	void setTable(std::string table);
	int getFileSize(std::ifstream& file);
	char* decompress(char* data, int size, int* decompressedSize);
};