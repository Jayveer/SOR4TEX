#include "texture.h"
#include <windows.h>

Texture::Texture(std::string file, std::string table) {
	setFile(file);
	setTable(table);
	parseTable();
}

Texture::~Texture() {
	this->file.close();
	this->table.close();
}

int Texture::getFileSize(std::ifstream& file) {
	int size;
	int cur = file.tellg();

	file.seekg(0, SEEK_END);
	size = file.tellg();

	file.seekg(cur, SEEK_SET);
	return size;
}

void Texture::setFile(std::string file) {
	this->file.open(file, std::ios::binary);
}

void Texture::setTable(std::string table) {
	this->table.open(table, std::ios::binary);
}

void Texture::parseTable() {
	int filesize = getFileSize(table);

	while (table.tellg() < filesize) {
		TextureTable row;
		table.read((char*)&row.filenameSize, 1);

		if (row.filenameSize > 0x7F) {
			uint8_t unk = 0;
			table.read((char*)&unk, 1);
		}

		char* filename = new char[row.filenameSize];
		table.read(filename, row.filenameSize);

		std::wstring wstr((wchar_t*)filename, row.filenameSize / sizeof(wchar_t));
		row.filename = wstr;
		delete[] filename;

		table.read((char*)&row.offset, 4);
		table.read((char*)&row.pad, 4);
		table.read((char*)&row.dataSize, 4);

		this->fileInfo.push_back(row);
	}

}

char* Texture::decompress(char* data, int size, int* decompressedSize) {
	Zlib zlib;
	char* decompressed = new char[MAXDECOMPRESSED];
	zlib.zinflate(data, decompressed, size, MAXDECOMPRESSED, decompressedSize, 0);
	return decompressed;
}

void createdir(std::wstring path) {
	unsigned int pos = 0;
	do
	{
		pos = path.find_first_of(L"\\/", pos + 1);
		CreateDirectory(path.substr(0, pos).c_str(), NULL);
	} while (pos != std::string::npos);
}

std::wstring getFilePath(std::wstring filename) {
	return filename.substr(0, filename.find_last_of(L"/\\") + 1);
}

void Texture::extract() {
	for (int i = 0; i < this->fileInfo.size(); i++) {
		std::ofstream output;
		std::wstring filepath = getFilePath(fileInfo[i].filename);

		createdir(filepath);

		output.open(fileInfo[i].filename + L".xnb");
		char* compressedData = new char[fileInfo[i].dataSize];

		file.seekg(fileInfo[i].offset, SEEK_SET);
		file.read(compressedData, fileInfo[i].dataSize);

		int decompressedSize = 0;
		char* decompressedData = decompress(compressedData, fileInfo[i].dataSize, &decompressedSize);

		output.write(decompressedData, decompressedSize);

		delete[] compressedData;
		delete[] decompressedData;
		output.close();
	}
}