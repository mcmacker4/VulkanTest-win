#include <iostream>
#include "Renderer.h"

int main() {
	try {
		Renderer renderer;
	}
	catch (char* msg) {
		std::cerr << msg << std::endl;
		return -1;
	}
	std::cin.get();
	return 0;
}
