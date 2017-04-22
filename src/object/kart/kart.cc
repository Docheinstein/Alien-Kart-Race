#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "game.h"
#include "level.h"
#include "map.h"
#include "kart.h"

const int MINI_MAP_KART_SIZE = 2;

Kart::Kart() {
	mDirection = 0;
}

int Kart::col() {
	return static_cast<int>(mCol);
}

int Kart::row() {
	return static_cast<int>(mRow);
}

void Kart::setCol(double col) {
	mCol = col;
}

void Kart::setRow(double row) {
	mRow = row;
}

void Kart::drawOnMiniMap() {
	sf::CircleShape miniKartCircle(MINI_MAP_KART_SIZE);
	miniKartCircle.setFillColor(sf::Color(100, 250, 50));
	Map *m = Game::instance().level()->map();
	miniKartCircle.setPosition(
		//Map::MINI_MAP_X + m->colCount() / (double) Map::MINI_MAP_WIDTH * mCol,
		//Map::MINI_MAP_Y + m->rowCount() / (double) Map::MINI_MAP_WIDTH * mRow
		Map::MINI_MAP_X + (double) Map::MINI_MAP_WIDTH / m->colCount() * mCol
			- MINI_MAP_KART_SIZE / 2,
		Map::MINI_MAP_Y + (double) Map::MINI_MAP_HEIGHT / m->rowCount() * mRow
			- MINI_MAP_KART_SIZE / 2
	);
	Game::instance().window()->draw(miniKartCircle);
}

void Kart::update() {
	const double RADIUS_INCREMENT_PER_CYCLE = (double) 1 / (double) 32;
	const double SPEED = 5;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
	   mRow -= cos(mDirection) * SPEED;
	   mCol += sin(mDirection) * SPEED;
	}

	// Left / Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		mDirection += RADIUS_INCREMENT_PER_CYCLE;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		mDirection -= RADIUS_INCREMENT_PER_CYCLE;


	std::cout << "COL\t: " << mCol << std::endl;
	std::cout << "ROW\t: " << mRow << std::endl;
	std::cout << "Dir\t: " << mDirection << std::endl << std::endl;
}
