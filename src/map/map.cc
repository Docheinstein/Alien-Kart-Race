#include <iostream>
#include <cmath>
#include "game.h"
#include "level.h"
#include "kart.h"
#include "map.h"
#include "fileutil.h"
#include "matrixutil.h"
#include "resourceutil.h"
#include "geometryutil.h"
#include "log.h"

const int DIRECTIONS_CALCULATED = 80;
const int MATRIX_RENDERED_TILES_RADIUS = 20;
const int POINTS_CALCULATED_REDUCE_FACTOR = 3;

const int TILE_SIZE = 48;

const int MATRIX_RENDERED_TILES_DIAMETER = MATRIX_RENDERED_TILES_RADIUS * 2 + 1;

const double HORIZON_LINE_Y = Game::WINDOW_HEIGHT / 2;

const int POINTS_CALCULATED_PER_TILE_DIMENSION = TILE_SIZE / POINTS_CALCULATED_REDUCE_FACTOR;

// [DIRS][INTERNAL][INTERNAL][MATRIX_RENDERED][MATRIX_RENDERED]
// const int ARRAY_FACTOR_MATRIX_RENDERED_TILES_COL = 1;

const int ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW = MATRIX_RENDERED_TILES_DIAMETER;

const int ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR =
	ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW * MATRIX_RENDERED_TILES_DIAMETER;

const int ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR =
	ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR * POINTS_CALCULATED_PER_TILE_DIMENSION;

const int ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR =
	ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR * POINTS_CALCULATED_PER_TILE_DIMENSION;

const int SIZE_OF_PERSPECTIVE_CACHE =
	DIRECTIONS_CALCULATED *
	POINTS_CALCULATED_PER_TILE_DIMENSION *
	POINTS_CALCULATED_PER_TILE_DIMENSION *
	MATRIX_RENDERED_TILES_DIAMETER *
	MATRIX_RENDERED_TILES_DIAMETER;

IPoint* Map::sCachedPerspectivePoints = new IPoint[SIZE_OF_PERSPECTIVE_CACHE];

sf:: Texture greenSolidTexture;
sf:: Texture streetSolidTexture;
sf:: Texture redSolidTexture;

Map::Map() {
	mMatrix = NULL;
	greenSolidTexture.loadFromFile(ResourceUtil::image("green_solid.png"));
	graySolidTexture.loadFromFile(ResourceUtil::image("gray_solid.png"));
	redSolidTexture.loadFromFile(ResourceUtil::image("red_solid.png"));
}

Map::~Map() {
	MatrixUtil::deleteMatrix(mMatrix, mRowCount);
}

void Map::loadMap(MapType mapType) {
	MatrixUtil::deleteMatrix(mMatrix, mRowCount);

	std::string mapPath = Map::getMapFilePath(mapType);
	mColCount = FileUtil::getIntCountInFirstRow(mapPath.c_str());
	mRowCount = FileUtil::getConsecutiveRowCount(mapPath.c_str());

	std::cout << "Map file path: " << mapPath << std::endl;
	std::cout << "Map cols: " << mColCount << std::endl;
	std::cout << "Map rows: " << mRowCount << std::endl;

	FileUtil::loadIntMatrix(mapPath.c_str(), mMatrix, mRowCount, mColCount);
	// MatrixUtil::printMatrix(mMatrix, mRowCount, mColCount);

	//createMapGrid();
	createMiniMap();
}

int Map::getTile(int row, int col) {
	return mMatrix[row][col];
}

void Map::update() {
	#if 0
	// Update the perspective tile system
	for (std::list<PerspectiveMatrixTile>::iterator iter = mPerspectiveTiles.begin();
		iter != mPerspectiveTiles.end();
		iter++
	) {
		Kart *k = Game::instance().level()->kart();

		double rowOrientedOffset;
		double colOrientedOffset;
		double kartDirection = k->direction();
		//std::cout << "DIR: " << kartDirection << std::endl;
		/*
		if (kartDirection < 3.1415 / 2 && kartDirection > -3.1415 / 2) {
			std::cout << "Going up" << std::endl;
			rowOrientedOffset = (*iter).rowFromBaseline;
			colOrientedOffset = (*iter).colFromCenter;
		}
		else {
			std::cout << "Going down" << std::endl;
			rowOrientedOffset = - (*iter).rowFromBaseline;
			colOrientedOffset = - (*iter).colFromCenter;
		}
		*/


		colOrientedOffset = (*iter).colFromCenter * cos(kartDirection ) -
							(*iter).rowFromBaseline	* sin(kartDirection) ;
		rowOrientedOffset = (*iter).rowFromBaseline * cos(kartDirection ) +
						 	(*iter).colFromCenter	* sin(kartDirection) ;

		int interpolatedMatrixRow = k->row() + rowOrientedOffset;
		int interpolatedMatrixCol = k->col() + colOrientedOffset;
		if (mMatrix[interpolatedMatrixRow][interpolatedMatrixCol] == Street) {
			(*iter).tile = Street;

			/*
			(*iter).vertices[0].color = sf::Color::Red;
			(*iter).vertices[1].color = sf::Color::Red;
			(*iter).vertices[2].color = sf::Color::Red;
			(*iter).vertices[3].color = sf::Color::Red;
			*/

			(*iter).vertices[0].texCoords = sf::Vector2f(31, 31);
			(*iter).vertices[1].texCoords = sf::Vector2f(0, 31);
			(*iter).vertices[2].texCoords = sf::Vector2f(0, 0);
			(*iter).vertices[3].texCoords = sf::Vector2f(31, 0);


		}
		else
			//(*iter).tile = Empty;
			{
				(*iter).tile = Empty;

				/*
				(*iter).vertices[0].color = sf::Color::Red;
				(*iter).vertices[1].color = sf::Color::Red;
				(*iter).vertices[2].color = sf::Color::Red;
				(*iter).vertices[3].color = sf::Color::Red;
				*/

				(*iter).vertices[0].texCoords = sf::Vector2f(31, 31);
				(*iter).vertices[1].texCoords = sf::Vector2f(0, 31);
				(*iter).vertices[2].texCoords = sf::Vector2f(0, 0);
				(*iter).vertices[3].texCoords = sf::Vector2f(31, 0);
			}
		/*
		if (mMatrix[interpolatedMatrixRow][interpolatedMatrixCol] == Empty) {
			(*iter).tile = Empty;
			(*iter).vertices[0].color = sf::Color::Black;
			(*iter).vertices[1].color = sf::Color::Black;
			(*iter).vertices[2].color = sf::Color::Black;
			(*iter).vertices[3].color = sf::Color::Black;
		}
		if (mMatrix[interpolatedMatrixRow][interpolatedMatrixCol] == Empty2) {
			(*iter).tile = Empty;
			(*iter).vertices[0].color = sf::Color::Green;
			(*iter).vertices[1].color = sf::Color::Green;
			(*iter).vertices[2].color = sf::Color::Green;
			(*iter).vertices[3].color = sf::Color::Green;
		}
		*/
	}
	#endif

	// createMapGrid();
	createMapGridOnDemand();
}

void Map::drawMapGrid() {
	sf::Texture mapGridTexture;
	mapGridTexture.loadFromImage(*mMapGrid);

	sf::Sprite mapGridSprite;
	mapGridSprite.setTexture(mapGridTexture);

	Game::instance().window()->draw(mapGridSprite);
}

void Map::drawMap() {
	drawMapGrid();

	for (std::list<PerspectiveMatrixTile>::const_iterator iter = mPerspectiveTiles.begin();
		iter != mPerspectiveTiles.end();
		iter++
	) {

		if ((*iter).tile == Empty || (*iter).tile == Empty2) {
			Game::instance().window()->draw((*iter).vertices, &greenSolidTexture);
		}
		else if ((*iter).tile == Street) {
			Game::instance().window()->draw((*iter).vertices, &streetSolidTexture);
		}
		else if ((*iter).tile == Event) {
			Game::instance().window()->draw((*iter).vertices, &redSolidTexture);
		}
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
	miniMapSprite.setPosition(MINI_MAP_X, MINI_MAP_Y);

	Game::instance().window()->draw(miniMapSprite);
}

int Map::colCount() {
	return mColCount;
}

int Map::rowCount() {
	return mRowCount;
}

std::string Map::getMapFilePath(MapType mapType) {
	switch (mapType) {
	case FirstMap:
		return ResourceUtil::map("map.txt");
	}
	return "";
}


void Map::generatePerspectiveSystemCache() {
	return;
	sf::Clock clock;

	d("Generating perspective system cache, array dimension: ", SIZE_OF_PERSPECTIVE_CACHE);
	d("Total structure size: ", SIZE_OF_PERSPECTIVE_CACHE * sizeof(IPoint) / (1000), " KB");
	// d("Using array location factors of");
	//
	// d("ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW ", ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW);
	// d("ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR ", ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR);
	// d("ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR  ", ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR);
	// d("ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR ", ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR);

	clock.restart();

	int calculatedPoints = 0;
	const int WIDTH = Game::WINDOW_WIDTH;
	const int HEIGHT = Game::WINDOW_HEIGHT;

	const double BASE_POINT_Y = HEIGHT - Kart::MARGIN_FROM_BOTTOM;
	const double BASE_POINT_X = WIDTH / 2;
	const Point bp = Point { BASE_POINT_X, BASE_POINT_Y };

	// Calculated the radians of the directions only once
	const double RADIANS_OFFSET_BETWEEN_DIRECTIONS = M_PI * 2 / DIRECTIONS_CALCULATED;
	const double SMALL_DOUBLE = (double) 1 / (1 << 7);
	double *directionsPrecalculatedRadians = new double[DIRECTIONS_CALCULATED];
	for (int i = 0; i < DIRECTIONS_CALCULATED; i++) {
		directionsPrecalculatedRadians[i] = RADIANS_OFFSET_BETWEEN_DIRECTIONS * i + SMALL_DOUBLE;
		d("directionsPrecalculatedRadians [", i, "]: ",  directionsPrecalculatedRadians[i]);
	}

	// Variables, defined out of the scope for performance reasons
	double vanishPoint1x, vanishPoint2x;

	Point vp1 {0, HORIZON_LINE_Y};
	Point vp2 {0, HORIZON_LINE_Y};

	double tilePointR, tilePointC;
	double rowOffset, colOffset;
	double matrixCenterRelativeOffsettedRow, matrixCenterRelativeOffsettedCol;

	for (int iDir = 0; iDir < DIRECTIONS_CALCULATED; iDir++) {
		const double currentDir = directionsPrecalculatedRadians[iDir];
		vanishPoint1x =
			BASE_POINT_X -
			tan(M_PI / 2 - currentDir)
			* (BASE_POINT_Y - HORIZON_LINE_Y);

		vanishPoint2x =
			BASE_POINT_X +
			tan(currentDir)
			* (BASE_POINT_Y - HORIZON_LINE_Y);

		for (int iInternalTilePointR = 0; iInternalTilePointR < POINTS_CALCULATED_PER_TILE_DIMENSION; iInternalTilePointR ++) {
		for (int iInternalTilePointC = 0; iInternalTilePointC < POINTS_CALCULATED_PER_TILE_DIMENSION; iInternalTilePointC ++) {

		vp1.x = vanishPoint1x;
		vp2.x = vanishPoint2x;

		// tilePointR = iInternalTilePointR * POINTS_CALCULATED_REDUCE_FACTOR;
		// tilePointC = iInternalTilePointC * POINTS_CALCULATED_REDUCE_FACTOR;

		tilePointR = (iInternalTilePointR == 0 ? 0 : (double) iInternalTilePointR / POINTS_CALCULATED_PER_TILE_DIMENSION);
		tilePointC = (iInternalTilePointC == 0 ? 0 : (double) iInternalTilePointC / POINTS_CALCULATED_PER_TILE_DIMENSION);

		// matrixCenterRelativeOffsettedRow = tilePointR + MATRIX_RENDERED_TILES_RADIUS;

		for (int r = 0; r < MATRIX_RENDERED_TILES_DIAMETER; r++, matrixCenterRelativeOffsettedRow--) {
			// matrixCenterRelativeOffsettedCol = tilePointC + MATRIX_RENDERED_TILES_RADIUS;

		for (int c = 0; c < MATRIX_RENDERED_TILES_DIAMETER; c++, matrixCenterRelativeOffsettedCol--) {

			matrixCenterRelativeOffsettedRow = tilePointR + MATRIX_RENDERED_TILES_RADIUS - r;
			matrixCenterRelativeOffsettedCol = tilePointC + MATRIX_RENDERED_TILES_RADIUS - c;

			rowOffset = (matrixCenterRelativeOffsettedRow) / sin (currentDir); //cos (M_PI / 2 - currentDir);
			colOffset = (matrixCenterRelativeOffsettedCol) / cos (currentDir);

			Point pr = { bp.x + rowOffset * TILE_SIZE, bp.y };
			Point pc = { bp.x - colOffset * TILE_SIZE, bp.y };

			Line pr_vp1 = GeometryUtil::lineForTwoPoints(pr, vp1);
			Line pc_vp2 = GeometryUtil::lineForTwoPoints(pc, vp2);

			Point pp = GeometryUtil::intersectionForTwoLines(pr_vp1, pc_vp2);

			// d("\nCalculated a point: ", calculatedPoints++, " of ", SIZE_OF_PERSPECTIVE_CACHE);
			// d("For params: iDir: ", iDir, " internalR: ",  iInternalTilePointR, " internalC: ", iInternalTilePointC, " r: ", r, " c: ", c);
			//
			// d("matrixCenterRelativeOffsettedRow ", matrixCenterRelativeOffsettedRow);
			// d("matrixCenterRelativeOffsettedCol ", matrixCenterRelativeOffsettedCol);
			// d("rowOffset ", rowOffset);
			// d("colOffset ", colOffset);
			// d("pp ", pp);

			setCachedMapPerspectivePoint(r, c, iInternalTilePointR, iInternalTilePointC, iDir, GeometryUtil::toIPoint(pp));
		}
		}
	}
	}
	}

	long generationMillis = clock.getElapsedTime().asMilliseconds();
	d("Millis taken for map cache generation: ", generationMillis);
}

IPoint Map::getCachedMapPerspectivePoint(int matrixRowIndex, int matrixColIndex,
										int tileInternalRowIndex, int tileInternalColIndex,
										int dirIndex) {
		const int i =
			dirIndex * ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR +
			tileInternalRowIndex * ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR +
			tileInternalColIndex * ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR +
			matrixRowIndex * ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW +
			matrixColIndex /* * 1 */;
		// d("Getting pp at ", i);
		return Map::sCachedPerspectivePoints[i];
}

void Map::setCachedMapPerspectivePoint(	int matrixRowIndex,	int matrixColIndex,
										int tileInternalRowIndex, int tileInternalColIndex,
										int dirIndex, const IPoint &p) {
	const int i =
		dirIndex * ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR +
		tileInternalRowIndex * ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR +
		tileInternalColIndex * ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR +
		matrixRowIndex * ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW +
		matrixColIndex /* * 1 */;

	// d("Setting perspective point in cache at ", i);
	// d("dirIndex * ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR ", dirIndex * ARRAY_FACTOR_DIRECTION_ARRAY_FACTOR);
	// d("tileInternalRowIndex * ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR ", tileInternalRowIndex * ARRAY_FACTOR_INTERNAL_TILE_ROW_ARRAY_FACTOR);
	// d("tileInternalColIndex * ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR ", tileInternalColIndex * ARRAY_FACTOR_INTERNAL_TILE_COL_ARRAY_FACTOR);
	// d("matrixRowIndex * ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW ", matrixRowIndex * ARRAY_FACTOR_MATRIX_RENDERED_TILES_ROW);
	// d("matrixColIndex ", matrixColIndex);

	Map::sCachedPerspectivePoints[i] = p;
}

void Map::createMapGridOnDemand() {
	int WIDTH = Game::WINDOW_WIDTH;
	int HEIGHT = Game::WINDOW_HEIGHT;

	Kart *k = Game::instance().level()->kart();
	double ANGLE = k->direction(); //1.10;

	mMapGrid = new sf::Image();
	mMapGrid->create(WIDTH, HEIGHT, sf::Color(255, 255, 255));
	mPerspectiveTiles.clear();

	const int MATRIX_RENDERED_TILES_RADIUS = 8;
	const int TILE_SIZE = 48;
	//Point center { m}
	const int iCOL = k->col();
	const int iROW = k->row();
	// Point matrixCenter { k->col(), k->row() };
	// Rotate, Scale, Translate

	double HORIZON_LINE_Y = HEIGHT / 2;

	// Basepoint
	//double BASE_POINT_Y = HEIGHT  * 4 / 3;
	double BASE_POINT_Y = HEIGHT - Kart::MARGIN_FROM_BOTTOM;
	double BASE_POINT_X = WIDTH / 2;

	// Vanish points
 	double VANISH_POINT_1_X = BASE_POINT_X - tan(M_PI / 2 - ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);
	double VANISH_POINT_2_X = BASE_POINT_X + tan(ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);

	Point bp = Point { BASE_POINT_X, BASE_POINT_Y };
	Point vp1 = Point { VANISH_POINT_1_X, HORIZON_LINE_Y };
	Point vp2 = Point { VANISH_POINT_2_X, HORIZON_LINE_Y };

	for (int r = 0; r < MATRIX_RENDERED_TILES_RADIUS * 2 + 1; r++) {
		for (int c = 0; c < MATRIX_RENDERED_TILES_RADIUS * 2 + 1; c++) {
			int matrixOffsetCOL = iCOL - MATRIX_RENDERED_TILES_RADIUS + c;
				int matrixOffsetROW = iROW - MATRIX_RENDERED_TILES_RADIUS + r;

				const double mdCOL = k->col() - matrixOffsetCOL;
				const double mdROW = k->row() - matrixOffsetROW;

				struct MatrixOffsets {
					double rowOffset;
					double colOffset;
				} typedef MatrixOffsets;

				MatrixOffsets centerOffset[4];
				const int UL = 0;
				const int UR = 1;
				const int DR = 2;
				const int DL = 3;

				centerOffset[UL].colOffset = (mdCOL) / cos (ANGLE);
				centerOffset[UL].rowOffset = (mdROW) / cos(M_PI / 2 - ANGLE);

				centerOffset[UR].colOffset = (mdCOL - 1) / cos (ANGLE);
				centerOffset[UR].rowOffset = (mdROW) / cos(M_PI / 2 - ANGLE);

				centerOffset[DR].colOffset = (mdCOL - 1) / cos (ANGLE);
				centerOffset[DR].rowOffset = (mdROW - 1) / cos(M_PI / 2 - ANGLE);

				centerOffset[DL].colOffset = (mdCOL) / cos (ANGLE);
				centerOffset[DL].rowOffset = (mdROW - 1) / cos(M_PI / 2 - ANGLE);

				Point pcUL = { bp.x - centerOffset[UL].colOffset * TILE_SIZE, bp.y };
				Point prUL = { bp.x + centerOffset[UL].rowOffset * TILE_SIZE, bp.y };
				Point pcDR = { bp.x - centerOffset[DR].colOffset * TILE_SIZE, bp.y };
				Point prDR = { bp.x + centerOffset[DR].rowOffset * TILE_SIZE, bp.y };


				Line pcUL_vp2 = GeometryUtil::lineForTwoPoints(
						pcUL, vp2);
				Line prUL_vp1 = GeometryUtil::lineForTwoPoints(
						prUL, vp1);
				Line pcDR_vp2 = GeometryUtil::lineForTwoPoints(
						pcDR, vp2);
				Line prDR_vp1 = GeometryUtil::lineForTwoPoints(
						prDR, vp1);

				Point ppUL = GeometryUtil::intersectionForTwoLines(
								pcUL_vp2, prUL_vp1);
				Point ppUR = GeometryUtil::intersectionForTwoLines(
								pcDR_vp2, prUL_vp1);
				Point ppDR = GeometryUtil::intersectionForTwoLines(
								pcDR_vp2, prDR_vp1);
				Point ppDL = GeometryUtil::intersectionForTwoLines(
								pcUL_vp2, prDR_vp1);

				PerspectiveMatrixTile matrixTile;

				matrixTile.tile = static_cast<MapTile>(
					mMatrix[matrixOffsetROW][matrixOffsetCOL]
				);

				matrixTile.vertices.setPrimitiveType(sf::Quads);
				matrixTile.vertices.clear();
				matrixTile.vertices.append(
					sf::Vertex( sf::Vector2f(	ppUL.x,
												ppUL.y))
				);
				matrixTile.vertices.append(
					sf::Vertex( sf::Vector2f(	ppUR.x,
												ppUR.y))
				);
				matrixTile.vertices.append(
					sf::Vertex( sf::Vector2f(	ppDR.x,
												ppDR.y))
				);
				matrixTile.vertices.append(
					sf::Vertex( sf::Vector2f(	ppDL.x,
												ppDL.y))
				);

				matrixTile.vertices[0].texCoords = sf::Vector2f(0, 0);
				matrixTile.vertices[1].texCoords = sf::Vector2f(15, 0);
				matrixTile.vertices[2].texCoords = sf::Vector2f(15, 15);
				matrixTile.vertices[3].texCoords = sf::Vector2f(0, 15);

				if (ppUL.y > HORIZON_LINE_Y &&
					ppUR.y > HORIZON_LINE_Y &&
					ppDR.y > HORIZON_LINE_Y &&
					ppDL.y > HORIZON_LINE_Y) {
						/*
						drawPoint(mMapGrid, ppUL, sf::Color::Red, 3);
						drawPoint(mMapGrid, ppUR, sf::Color::Red, 3);
						drawPoint(mMapGrid, ppDR, sf::Color::Red, 3);
						drawPoint(mMapGrid, ppDL, sf::Color::Red, 3);
						*/
						mPerspectiveTiles.push_front(matrixTile);
					}
		}
	}
}

void Map::createMapGrid() {
	Kart *k = Game::instance().level()->kart();

	const int INT_ROW = k->row();
	const int INT_COL = k->col();
	const double ANGLE = k->direction();

	const double internalRowOffset = k->row() - INT_ROW;
	const double internalColOffset = k->col() - INT_COL;

	const int APPROSSIMATED_INTERNAL_TILE_POSITION_ROW =
		internalRowOffset * POINTS_CALCULATED_PER_TILE_DIMENSION;

	const int APPROSSIMATED_INTERNAL_TILE_POSITION_COL =
			internalColOffset * POINTS_CALCULATED_PER_TILE_DIMENSION;

	const int APPROSSIMATED_DIRECTION_INDEX =
		static_cast<int>((ANGLE * DIRECTIONS_CALCULATED) / (M_PI * 2));

	mMapGrid = new sf::Image();
	mMapGrid->create(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, sf::Color(255, 255, 255));
	mPerspectiveTiles.clear();

	for (int r = 0; r < MATRIX_RENDERED_TILES_DIAMETER - 1; r++) {
		for (int c = 0; c < MATRIX_RENDERED_TILES_DIAMETER - 1; c++) {
			// d("\nReal ANGLE: ", ANGLE);
			// d("APPROSSIMATED_DIRECTION_INDEX: ", APPROSSIMATED_DIRECTION_INDEX);
			//
			// d("k->row(): ", k->row());
			// d("APPROSSIMATED_INTERNAL_TILE_POSITION_ROW: ", APPROSSIMATED_INTERNAL_TILE_POSITION_ROW);
			//
			// d("k->col(): ", k->col());
			// d("APPROSSIMATED_INTERNAL_TILE_POSITION_COL: ", APPROSSIMATED_INTERNAL_TILE_POSITION_COL);

			// d("Asking perspective points");


			IPoint ippUL = getCachedMapPerspectivePoint(r, c,
				APPROSSIMATED_INTERNAL_TILE_POSITION_ROW,
				APPROSSIMATED_INTERNAL_TILE_POSITION_COL,
				APPROSSIMATED_DIRECTION_INDEX
			);


			IPoint ippUR = getCachedMapPerspectivePoint(r, c + 1,
				APPROSSIMATED_INTERNAL_TILE_POSITION_ROW,
				APPROSSIMATED_INTERNAL_TILE_POSITION_COL,
				APPROSSIMATED_DIRECTION_INDEX
			);

			IPoint ippDL = getCachedMapPerspectivePoint(r + 1, c,
				APPROSSIMATED_INTERNAL_TILE_POSITION_ROW,
				APPROSSIMATED_INTERNAL_TILE_POSITION_COL,
				APPROSSIMATED_DIRECTION_INDEX
			);

			IPoint ippDR = getCachedMapPerspectivePoint(r + 1, c + 1,
				APPROSSIMATED_INTERNAL_TILE_POSITION_ROW,
				APPROSSIMATED_INTERNAL_TILE_POSITION_COL,
				APPROSSIMATED_DIRECTION_INDEX
			);

			Point ppUL { ippUL.x, ippUL.y };
			Point ppUR { ippUR.x, ippUR.y };
			Point ppDR { ippDR.x, ippDR.y };
			Point ppDL { ippDL.x, ippDL.y };

			int matrixOffsetCol = INT_COL - MATRIX_RENDERED_TILES_RADIUS + c;
			int matrixOffsetRow = INT_ROW - MATRIX_RENDERED_TILES_RADIUS + r;

			PerspectiveMatrixTile matrixTile;

			matrixTile.tile = static_cast<MapTile>(
				mMatrix[matrixOffsetRow][matrixOffsetCol]
			);

			matrixTile.vertices.setPrimitiveType(sf::Quads);
			matrixTile.vertices.clear();
			matrixTile.vertices.append(
				sf::Vertex( sf::Vector2f(	ppUL.x,
											ppUL.y))
			);
			matrixTile.vertices.append(
				sf::Vertex( sf::Vector2f(	ppUR.x,
											ppUR.y))
			);
			matrixTile.vertices.append(
				sf::Vertex( sf::Vector2f(	ppDR.x,
											ppDR.y))
			);
			matrixTile.vertices.append(
				sf::Vertex( sf::Vector2f(	ppDL.x,
											ppDL.y))
			);

			matrixTile.vertices[0].texCoords = sf::Vector2f(0, 0);
			matrixTile.vertices[1].texCoords = sf::Vector2f(15, 0);
			matrixTile.vertices[2].texCoords = sf::Vector2f(15, 15);
			matrixTile.vertices[3].texCoords = sf::Vector2f(0, 15);

			if (ppUL.y > HORIZON_LINE_Y &&
				ppUR.y > HORIZON_LINE_Y &&
				ppDR.y > HORIZON_LINE_Y &&
				ppDL.y > HORIZON_LINE_Y) {
					// drawPoint(mMapGrid, ppUL, sf::Color::Red, 3);
					// drawPoint(mMapGrid, ppUR, sf::Color::Red, 3);
					// drawPoint(mMapGrid, ppDR, sf::Color::Red, 3);
					// drawPoint(mMapGrid, ppDL, sf::Color::Red, 3);

					// d("ppUL: ", ppUL);
					// d("ppUR ", ppUR);
					// d("ppDR: ", ppDR);
					// d("ppDL ", ppDL, "\n");

					mPerspectiveTiles.push_front(matrixTile);
				}

		}
	}
}

void Map::createMiniMap() {
	mMiniMap = new sf::Image();
	mMiniMap->create(mColCount, mRowCount, sf::Color(255, 255, 255));

	for (int y = 0; y < mRowCount; y++) {
		for (int x = 0; x < mColCount; x++) {
			if (mMatrix[y][x] == Street)
				mMiniMap->setPixel(x, y, sf::Color(128, 128, 128));
			//else
			//	mMiniMap->setPixel(x, y, sf::Color(0, 0, 0));
	  	}
	}
}


void drawPoint(sf::Image * map, const Point &p, sf::Color color, int size = 7) {
	if (p.x < 0 || p.x >= Game::WINDOW_WIDTH || p.y < 0 || p.y >= Game::WINDOW_HEIGHT)
		return;
	for (int x = - size / 2; x <= size / 2; x++) {
		for (int y = - size / 2; y <= size / 2; y++) {
			map->setPixel(p.x + x, p.y + y, color);
		}
	}
}

void drawPoint(sf::Image * map, const IPoint &p, sf::Color color, int size = 7) {
	if (p.x < 0 || p.x >= Game::WINDOW_WIDTH || p.y < 0 || p.y >= Game::WINDOW_HEIGHT)
		return;
	for (int x = - size / 2; x <= size / 2; x++) {
		for (int y = - size / 2; y <= size / 2; y++) {
			map->setPixel(p.x + x, p.y + y, color);
		}
	}
}

void drawLine(double x1, double y1, double x2, double y2) {
	sf::VertexArray lineThick(sf::Quads, 4);

	lineThick[0].position = sf::Vector2f(10, 10);
	lineThick[1].position = sf::Vector2f(20, 10);
	lineThick[2].position = sf::Vector2f(100, 100);
	lineThick[3].position = sf::Vector2f(80, 100);

	lineThick[0].color = sf::Color::Red;
	lineThick[1].color = sf::Color::Red;
	lineThick[2].color = sf::Color::Red;
	lineThick[3].color = sf::Color::Red;
	// d("Drawing line at", x1, y1, x2, y2);
	Game::instance().window()->draw(lineThick);
}
