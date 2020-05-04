#include "sor4/texture/texture.h"
#include <iostream>

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "usage SOR4TEX.exe [texture file] [texture_table file]" << std::endl;
		return 0;
	}

	Texture tex(argv[1], argv[2]);
	tex.extract();
	return 0;
}