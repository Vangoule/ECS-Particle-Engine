#include <stdexcept>
#include <iostream>

#include "Game.h"


int main(int argc, char** argv) {

	Game game;

	try {
		game.Run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;

		return EXIT_FAILURE;
	}

//	system("pause");

	return EXIT_SUCCESS;
}