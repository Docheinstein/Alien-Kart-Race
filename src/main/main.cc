#include "game.h"

void initRandomizer() {
	srand(time(NULL));
}

int main() {
	initRandomizer();
	Game().start();
    return 0;
}
