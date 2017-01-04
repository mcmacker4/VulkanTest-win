#include <iostream>
#include "Renderer.h"

int main() {
	try {
		Renderer renderer;
	}
	catch (char* msg) {

#ifdef WIN32
		MessageBox(NULL, msg, "ERROR", 0);
#endif

		std::cerr << msg << std::endl;
		std::cin.get();
		return -1;
	}
	std::cin.get();
	return 0;
}
