#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include "game.h"
#include "level.h"
#include "map.h"
#include "kart.h"
#include "resourceutil.h"
#include "log.h"

const int MINI_MAP_KART_SIZE = 2;

Kart::Kart() {
	mDirection = (double) 1 / (1 << 8);
	initSprite();
}

Kart::~Kart() {
	delete [] mSprites;
	delete [] mTextures;
}

double Kart::col() {
	return mCol;
}

double Kart::row() {
	return mRow;
}

double Kart::direction() {
	return mDirection;
}

void Kart::setCol(double col) {
	mCol = col;
}

void Kart::setRow(double row) {
	mRow = row;
}

void Kart::draw() {
	Game::instance().window()->draw(mSprites[0]);
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
	// 1 / 1 << 4 = 0.06 radians => 50 different angles
	// 1 / 1 << 3 = 0.06 radians => 25 different angles
	const double RADIUS_INCREMENT_PER_CYCLE = (double) 1 / (double) (1 << 4);
	const double SPEED = (double) 1 / (double) (1 << 3);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
	   mRow -= cos(-mDirection) * SPEED;
	   mCol += sin(-mDirection) * SPEED;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
	   mRow += cos(-mDirection) * SPEED;
	   mCol -= sin(-mDirection) * SPEED;
	}

	// Left / Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		mDirection -= RADIUS_INCREMENT_PER_CYCLE;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		mDirection += RADIUS_INCREMENT_PER_CYCLE;
	mDirection = fmod(mDirection + M_PI * 2, M_PI * 2);

/*
	std::cout << "COL\t: " << mCol << std::endl;
	std::cout << "ROW\t: " << mRow << std::endl;
	std::cout << "Dir\t: " << mDirection << std::endl << std::endl;
*/
	// d("Kart [ROW: ", mRow, ", COL: ", mCol, "]");
	Map *m = Game::instance().level()->map();
	// if (m->getTile(mRow, mCol) == 4) {
	// 	d("Over an event tile");
	// }
}

// PRIVATE

void Kart::initSprite() {
	d("Initializing sprite");
	mTextures = new sf::Texture[1];
	mSprites = new sf::Sprite[1];
	mTextures[0].loadFromFile(ResourceUtil::image("alien_center.png"));
	mSprites[0].setTexture(mTextures[0]);
	mSprites[0].setPosition(
		(Game::WINDOW_WIDTH - mSprites[0].getLocalBounds().width) / 2,
		Game::WINDOW_HEIGHT - MARGIN_FROM_BOTTOM -mSprites[0].getLocalBounds().height);
}
