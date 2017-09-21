#include <SFML/Graphics.hpp>
#include <fstream>
#include "minimap.h"
#include "game.h"
#include "fileutil.h"

const sf::Color COLOR_FULL = sf::Color(128, 128, 128, 164);
const sf::Color COLOR_EMPTY = sf::Color::Transparent;

const int MINIMAP_X = Game::WINDOW_WIDTH * 0.5;
const int MINIMAP_Y = 0;

Minimap::Minimap() {

}

Minimap::~Minimap() {
    delete mMinimapSprite;
    delete mMinimapTexture;
}

void Minimap::drawPoint(double row, double col, int size, sf::Color color) {
	sf::CircleShape circlePoint(size);
	circlePoint.setFillColor(color);

	circlePoint.setPosition(
		MINIMAP_X + (double) MINIMAP_SIZE / mColCount * col - size / 2,
		MINIMAP_Y + (double) MINIMAP_SIZE / mRowCount * row - size / 2
	);

	Game::instance().window()->draw(circlePoint);

}

void Minimap::loadFromFile(const char *minimapFilename) {
    bool **matrix;
    FileUtil::loadMatrix<bool>(minimapFilename, matrix, mRowCount, mColCount);

    sf::Image minimapImage;
    minimapImage.create(mColCount, mRowCount, COLOR_EMPTY);

	for (int y = 0; y < mRowCount; y++) {
		for (int x = 0; x < mColCount; x++) {
			if (matrix[y][x])
				minimapImage.setPixel(x, y, COLOR_FULL);
	  	}
	}

    mMinimapTexture = new sf::Texture();
	mMinimapTexture->loadFromImage(minimapImage);

	mMinimapSprite = new sf::Sprite();
	mMinimapSprite->setTexture(*mMinimapTexture);

	mMinimapSprite->scale(
		(double) MINIMAP_SIZE / mColCount,
		(double) MINIMAP_SIZE / mRowCount);
	mMinimapSprite->setPosition(MINIMAP_X, MINIMAP_Y);
}

void Minimap::draw() {
	Game::instance().window()->draw(*mMinimapSprite);
}