#include <SFML/Graphics.hpp>
#include <fstream>
#include "minimap.h"
#include "config.h"
#include "fileutil.h"
#include "resourceutil.h"

const sf::Color COLOR_FULL = sf::Color(128, 128, 128, 164);
const sf::Color COLOR_EMPTY = sf::Color::Transparent;

const int MINIMAP_X = Config::WINDOW_WIDTH * 0.6;
const int MINIMAP_Y = 0;

Minimap::Minimap(sf::RenderWindow *window, MapFactory::MapType mapType) {
    mWindow = window;
    loadFromFile(ResourceUtil::raw(MapFactory::minimapFilename(mapType)).c_str());
}

Minimap::~Minimap() {
    delete mMinimapSprite;
    delete mMinimapTexture;
}

// ------------------------
// PUBLIC -----------------
// ------------------------

void Minimap::draw() {
	mWindow->draw(*mMinimapSprite);
}

void Minimap::drawPoint(const Point &p, int size, sf::Color color) {
	sf::CircleShape circlePoint(size);
	circlePoint.setFillColor(color);

	circlePoint.setPosition(
		MINIMAP_X + (float) Config::MINIMAP_SIZE / mColCount * p.x - (float) size / 2,
		MINIMAP_Y + (float) Config::MINIMAP_SIZE / mRowCount * p.y - (float) size / 2
	);

	mWindow->draw(circlePoint);
}

// ------------------------
// PRIVATE ----------------
// ------------------------

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
		(double) Config::MINIMAP_SIZE / mColCount,
		(double) Config::MINIMAP_SIZE / mRowCount);
	mMinimapSprite->setPosition(MINIMAP_X, MINIMAP_Y);
}