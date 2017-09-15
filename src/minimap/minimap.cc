#include <SFML/Graphics.hpp>
#include <fstream>
#include "minimap.h"
#include "fileutil.h"

const sf::Color COLOR_FULL = sf::Color(128, 128, 128);
const sf::Color COLOR_EMPTY = sf::Color::White;
Minimap::Minimap() {

}

Minimap::~Minimap() {
    //delete mMinimapImage;
}

void Minimap::loadFromFile(const char *minimapFilename) {
    /*
    bool **matrix;
    FileUtil::loadMatrix(minimapFilename, matrix, mRowCount, mColCount);

    mMinimapImage = new sf::Image();
	mMinimapImage->create(mColCount, mRowCount, COLOR_EMPTY);

	for (int y = 0; y < mRowCount; y++) {
		for (int x = 0; x < mColCount; x++) {
			if (matrix[y][x])
				mMinimapImage->setPixel(x, y, COLOR_FULL);
			//else
			//	mMiniMap->setPixel(x, y, sf::Color(0, 0, 0));
	  	}
	}
    */
}

void Minimap::draw() {

}