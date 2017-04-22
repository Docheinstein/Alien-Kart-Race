#include <iostream>

#include "game.h"
#include "level.h"
#include "kart.h"
#include "map.h"
#include "fileutil.h"
#include "matrixutil.h"

enum MapTile {
	Empty,
	Filled,
	Start
};

Map::Map() {
	mMatrix = NULL;
}

Map::~Map() {
	MatrixUtil::deleteMatrix(mMatrix, mRowCount);
}

void Map::loadMap(MapType mapType) {
	MatrixUtil::deleteMatrix(mMatrix, mRowCount);

	const char * mapPath = Map::getMapFilePath(mapType);
	mColCount = FileUtil::getIntCountInFirstRow(mapPath);
	mRowCount = FileUtil::getConsecutiveRowCount(mapPath);

	std::cout << "Map file path: " << mapPath << std::endl;
	std::cout << "Map cols: " << mColCount << std::endl;
	std::cout << "Map rows: " << mRowCount << std::endl;

	FileUtil::loadIntMatrix(mapPath, mMatrix, mRowCount, mColCount);
	// MatrixUtil::printMatrix(mMatrix, mRowCount, mColCount);

	createMapGrid();
	createMiniMap();
}

void Map::update() {
	// Update the perspective tile system
	for (std::list<PerspectiveMatrixTile>::iterator iter = mPerspectiveTiles.begin();
		iter != mPerspectiveTiles.end();
		iter++
	) {
		Kart *k = Game::instance().level()->kart();
		int interpolatedMatrixRow = k->row() + (*iter).mRowFromBaseline;
		int interpolatedMatrixCol = k->col() + (*iter).mColFromCenter;
		if (mMatrix[interpolatedMatrixRow][interpolatedMatrixCol] == 1) {
			(*iter).mVertices[0].color = sf::Color::Red;
			(*iter).mVertices[1].color = sf::Color::Red;
			(*iter).mVertices[2].color = sf::Color::Red;
			(*iter).mVertices[3].color = sf::Color::Red;
		}
		else {
			(*iter).mVertices[0].color = sf::Color::Black;
			(*iter).mVertices[1].color = sf::Color::Black;
			(*iter).mVertices[2].color = sf::Color::Black;
			(*iter).mVertices[3].color = sf::Color::Black;
		}
	}
}

void Map::drawMap() {
	sf::Texture mapGridTexture;
	mapGridTexture.loadFromImage(*mMapGrid);

	sf::Sprite mapGridSprite;
	mapGridSprite.setTexture(mapGridTexture);

	Game::instance().window()->draw(mapGridSprite);

	for (std::list<PerspectiveMatrixTile>::const_iterator iter = mPerspectiveTiles.begin();
		iter != mPerspectiveTiles.end();
		iter++
	) {
		Game::instance().window()->draw((*iter).mVertices);
	}
}

void Map::drawMiniMap() {
	sf::Texture miniMapTexture;
	miniMapTexture.loadFromImage(*mMiniMap);

	sf::Sprite miniMapSprite;
	miniMapSprite.setTexture(miniMapTexture);

	miniMapSprite.scale(
		(double) MINI_MAP_WIDTH / mColCount,
		(double) MINI_MAP_HEIGHT / mRowCount);
	miniMapSprite.setPosition(320, 240);

	Game::instance().window()->draw(miniMapSprite);
}

int Map::colCount() {
	return mColCount;
}

int Map::rowCount() {
	return mRowCount;
}

const char * Map::getMapFilePath(MapType mapType) {
	switch (mapType) {
	case FirstMap:
		return  "./res/raw/map.txt";
	}
	return "";
}

void Map::createMapGrid() {
	int WIDTH = 640;
	int HEIGHT = 480;

	mMapGrid = new sf::Image();
	mMapGrid->create(WIDTH, HEIGHT, sf::Color(255, 255, 255));

	// Y Lines

	int yLineCount;
	double yLines[100];
	yLines[0] = 0; // Baseline

	double CY = HEIGHT * ((double) 2 / 3); // Height of the camera
	double CZ = -CY; // Depth of the camera

	// Depth of the first rendered tile starting from the bottom
	double TILE_REAL_DEPTH = 100;
	double MINIMUM_RENDERED_TILE_DEPTH = 5;

	double TILES_IN_BASELINE = 10;
	double VANISH_POINT_HEIGHT = (double) HEIGHT * ((double) 3 / 4);

	int i = 1;
	double previousY = 0;
	double renderedY = 0;
	do {
		previousY = renderedY;
		renderedY = CY + (CZ*CY) / (i * TILE_REAL_DEPTH - CZ);
		yLines[i] = renderedY;
		std::cout << "Rendered Y for i=" << i << ": " << renderedY << std::endl;
		i++;
	} while (renderedY - previousY > MINIMUM_RENDERED_TILE_DEPTH);
	yLineCount = i - 1;

	for (int i = 0; i < yLineCount; i++) {
		for (int x = 0; x < WIDTH; x++) {
			mMapGrid->setPixel(x, HEIGHT - yLines[i], sf::Color(128, 64, 192));
		}
	}

	// X Points on Y Lines

	for (int i = 0; i < yLineCount - 1; i += 2) {
		std::cout << "m: " << WIDTH / VANISH_POINT_HEIGHT << std::endl;
		double widthOfLine =
			WIDTH / VANISH_POINT_HEIGHT * (VANISH_POINT_HEIGHT - yLines[i]);
			std::cout << "Width of line " << i << ": " << widthOfLine << std::endl;
		double tileOfLineWidth = widthOfLine / TILES_IN_BASELINE;

		double widthOfNextLine =
			WIDTH / VANISH_POINT_HEIGHT * (VANISH_POINT_HEIGHT - yLines[i + 1]);
			std::cout << "Width of line " << i << ": " << widthOfLine << std::endl;
		double tileOfNextLineWidth = widthOfNextLine / TILES_IN_BASELINE;

		int xTileCount = 0;
		// Left X Points
		for (double
			lineX = WIDTH / 2 - tileOfLineWidth,
			nextLineX = WIDTH / 2 - tileOfNextLineWidth;
			nextLineX > 0;
			lineX -= tileOfLineWidth,
			nextLineX -= tileOfNextLineWidth,
			xTileCount++) {

			mMapGrid->setPixel(lineX + tileOfLineWidth - 1, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth + 1, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth - 1, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth + 1, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth - 1, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + tileOfLineWidth + 1, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));

			mMapGrid->setPixel(lineX - 1, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + 1, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX - 1, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + 1, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX - 1, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(lineX + 1, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));

			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth - 1, HEIGHT - yLines[i + 1] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth, HEIGHT - yLines[i + 1] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth + 1, HEIGHT - yLines[i + 1] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth - 1, HEIGHT - yLines[i + 1], sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth, HEIGHT - yLines[i + 1], sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth + 1, HEIGHT - yLines[i + 1], sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth - 1, HEIGHT - yLines[i + 1] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth, HEIGHT - yLines[i + 1] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + tileOfNextLineWidth + 1, HEIGHT - yLines[i + 1] + 1, sf::Color(0, 0, 0));

			mMapGrid->setPixel(nextLineX - 1, HEIGHT - yLines[i + 1] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX, HEIGHT - yLines[i + 1] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + 1, HEIGHT - yLines[i + 1] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX - 1, HEIGHT - yLines[i + 1], sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX, HEIGHT - yLines[i + 1], sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + 1, HEIGHT - yLines[i + 1], sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX - 1, HEIGHT - yLines[i + 1] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX, HEIGHT - yLines[i + 1] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(nextLineX + 1, HEIGHT - yLines[i + 1] + 1, sf::Color(0, 0, 0));

			std::cout << "Adding vertex " << std::endl << std::flush;
			PerspectiveMatrixTile perspectiveTile;

			perspectiveTile.mRowFromBaseline = i / 2;
			perspectiveTile.mColFromCenter = xTileCount;

			perspectiveTile.mVertices.setPrimitiveType(sf::Quads);
			perspectiveTile.mVertices.append(
				sf::Vertex(
					sf::Vector2f(lineX + tileOfLineWidth, HEIGHT -yLines[i]),
					sf::Color::Red)
				);

			perspectiveTile.mVertices.append(
				sf::Vertex(
					sf::Vector2f(lineX, HEIGHT - yLines[i]),
					sf::Color::Blue)
				);

			perspectiveTile.mVertices.append(
				sf::Vertex(
					sf::Vector2f(nextLineX, HEIGHT - yLines[i + 1]),
					sf::Color::Green)
				);

			perspectiveTile.mVertices.append(
				sf::Vertex(
					sf::Vector2f(nextLineX + tileOfNextLineWidth, HEIGHT - yLines[i + 1]),
					sf::Color::Green)
				);
/*
			sf::VertexArray perspectiveTile(sf::Quads, 4);

			perspectiveTile[0].position = sf::Vector2f(lineX + tileOfLineWidth, HEIGHT -yLines[i]);
			perspectiveTile[1].position = sf::Vector2f(lineX, HEIGHT - yLines[i]);
			perspectiveTile[2].position = sf::Vector2f(nextLineX, HEIGHT - yLines[i + 1]);
			perspectiveTile[3].position = sf::Vector2f(nextLineX + tileOfNextLineWidth, HEIGHT - yLines[i + 1]);

			perspectiveTile[0].color = sf::Color::Red;
			perspectiveTile[1].color = sf::Color::Blue;
			perspectiveTile[2].color = sf::Color::Green;
			perspectiveTile[3].color = sf::Color::Green;
*/
			mPerspectiveTiles.push_front(perspectiveTile);
		}

/*
		// Right X Points
		for (double d = WIDTH / 2; d < WIDTH; d += tileOfLineWidth) {
			mMapGrid->setPixel(d - 1, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(d, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(d + 1, HEIGHT - yLines[i] - 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(d - 1, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(d, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(d + 1, HEIGHT - yLines[i], sf::Color(0, 0, 0));
			mMapGrid->setPixel(d - 1, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(d, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
			mMapGrid->setPixel(d + 1, HEIGHT - yLines[i] + 1, sf::Color(0, 0, 0));
		}
*/
	}
}

void Map::createMiniMap() {
	mMiniMap = new sf::Image();
	mMiniMap->create(mColCount, mRowCount, sf::Color(255, 255, 255));

	for (int y = 0; y < mRowCount; y++) {
		for (int x = 0; x < mColCount; x++) {
			if (mMatrix[y][x] == Filled)
				mMiniMap->setPixel(x, y, sf::Color(128, 128, 128));
	  	}
	}
}
