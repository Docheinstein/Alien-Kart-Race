#include "game.h"

/** Initializes the random seed. */
void initRandomizer() {
	srand(time(NULL));
}

int main() {
	initRandomizer();
	Game().start();
    return 0;
}
