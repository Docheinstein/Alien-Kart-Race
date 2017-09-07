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

Map::Map() {
	mMatrix = NULL;
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

	createMapGrid();
}

void Map::drawMapGrid() {
	sf::Texture mapGridTexture;
	mapGridTexture.loadFromImage(*mMapGrid);

	sf::Sprite mapGridSprite;
	mapGridSprite.setTexture(mapGridTexture);

	Game::instance().window()->draw(mapGridSprite);
/*
	int WIDTH = Game::WINDOW_WIDTH;
	int HEIGHT = Game::WINDOW_HEIGHT;

	double VANISH_POINT_HEIGHT = (double) HEIGHT * ((double) 3 / 4);
	double VANISH_POINT_X = WIDTH / 2;
	double VANISH_POINT_Y = HEIGHT - VANISH_POINT_HEIGHT;

	sf::CircleShape vanishPointCircle(5);
	vanishPointCircle.setFillColor(sf::Color(100, 250, 50));
	vanishPointCircle.setPosition(
		VANISH_POINT_X, VANISH_POINT_Y
	);

	Game::instance().window()->draw(vanishPointCircle);
*/
}

void Map::drawMap() {
	drawMapGrid();
	for (std::list<PerspectiveMatrixTile>::const_iterator iter = mPerspectiveTiles.begin();
		iter != mPerspectiveTiles.end();
		iter++
	) {
		if ((*iter).tile == Empty || (*iter).tile == Empty2) {
			sf:: Texture texture;
			texture.loadFromFile(ResourceUtil::image("green_solid.png"));
			Game::instance().window()->draw((*iter).vertices, &texture);
		}
		else if ((*iter).tile == Street) {
			sf:: Texture texture;
			texture.loadFromFile(ResourceUtil::image("street_solid.png"));
			Game::instance().window()->draw((*iter).vertices, &texture);
		}
	}

/*
	drawMapGrid();

	Kart *k = Game::instance().level()->kart();
	double kartDirection = k->direction();

	for (std::list<PerspectiveMatrixTile>::const_iterator iter = mPerspectiveTiles.begin();
		iter != mPerspectiveTiles.end();
		iter++
	) {

		if ((*iter).tile == Street) {
			sf:: Texture texture;
			texture.loadFromFile(ResourceUtil::image("street_circle.png"));

			//sf::Sprite sprite;
			//sprite.setTexture(texture);

			Game::instance().window()->draw((*iter).vertices, &texture);
		}
		else if ((*iter).tile == Empty) {
			sf:: Texture texture;
			texture.loadFromFile(ResourceUtil::image("green.png"));
			texture.rotate(kartDirection);

			//sf::Sprite sprite;
			//sprite.setTexture(texture);

			Game::instance().window()->draw((*iter).vertices, &texture);

		}

	}
	*/
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

void drawPoint(sf::Image * map, const Point &p, sf::Color color, int size = 7) {
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
	d("Drawing line at", x1, y1, x2, y2);
	Game::instance().window()->draw(lineThick);
}


void Map::createMapGrid() {
	int WIDTH = Game::WINDOW_WIDTH;
	int HEIGHT = Game::WINDOW_HEIGHT;

	Kart *k = Game::instance().level()->kart();
	double ANGLE = k->direction(); //1.10;
	double COL = k->col();
	double ROW = k->row();

	mMapGrid = new sf::Image();
	mMapGrid->create(WIDTH, HEIGHT, sf::Color(255, 255, 255));
	mPerspectiveTiles.clear();
/*
	Point c { 100, 100};
	Point p1 { 50, 20};
	Point p2 { 150, 220};

	Point p1_rotated = GeometryUtil::rotatePoint(p1, c, ANGLE);
	Point p2_rotated = GeometryUtil::rotatePoint(p2, c, ANGLE);

	drawPoint(mMapGrid, p1, sf::Color::Yellow);
	drawPoint(mMapGrid, p2, sf::Color::Green);
	drawPoint(mMapGrid, c, sf::Color::Black);
*/
/*
	d(
		"ROW [",  k->row(), "] \n",
		"COL [",  k->col(), "] \n",
		"ANGLE: ", ANGLE, "\n"
	);
	*/

	const int MATRIX_RENDERED_TILES_RADIUS = 20;
	const int TILE_SIZE = 48;
	//Point center { m}
	const int iCOL = k->col();
	const int iROW = k->row();
	// Point matrixCenter { k->col(), k->row() };
	// Rotate, Scale, Translate

	double HORIZON_LINE_Y = HEIGHT / 2;

	// Basepoint
	//double BASE_POINT_Y = HEIGHT  * 4 / 3;
	double BASE_POINT_Y = HEIGHT;
	double BASE_POINT_X = WIDTH / 2;

	// Vanish points
 	double VANISH_POINT_1_X = BASE_POINT_X - tan(M_PI / 2 - ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);
	double VANISH_POINT_2_X = BASE_POINT_X + tan(ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);

	Point bp = Point {BASE_POINT_X, BASE_POINT_Y };
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
/*
			d(	"[", r, "]",
				"[", c, "]",
			 	"\tMP (", mp.x, ", ", mp.y,")");
*/
			Point pcUL = { bp.x - centerOffset[UL].colOffset * TILE_SIZE, bp.y };
			Point prUL = { bp.x + centerOffset[UL].rowOffset * TILE_SIZE, bp.y };
			Point pcDR = { bp.x - centerOffset[DR].colOffset * TILE_SIZE, bp.y };
			Point prDR = { bp.x + centerOffset[DR].rowOffset * TILE_SIZE, bp.y };

/*
			drawPoint(mMapGrid, pcUL, sf::Color::Red);
			drawPoint(mMapGrid, prUL, sf::Color::Green);
			drawPoint(mMapGrid, pcDR, sf::Color::Blue);
			drawPoint(mMapGrid, prDR, sf::Color::Yellow);
*/
/*
			d("miCOL: ", miCOL);
			d("miROW: ", miROW);
			d("px1: ", px1.x);
			d("px2: ", px2.x);
*/

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
	//	GeometryUtil::rotatePoint()
	}
	drawPoint(mMapGrid, bp, sf::Color::Black);
	drawPoint(mMapGrid, vp1, sf::Color::Magenta);
	drawPoint(mMapGrid, vp2, sf::Color::Green);
/*
	// VP1 ....... MP2 ..... MP1 ..... VP2


	//					BP
	double HORIZON_LINE_Y = HEIGHT / 2;
	// Basepoint
	//double BASE_POINT_Y = HEIGHT  * 4 / 3;
	double BASE_POINT_Y = HEIGHT  * 2 / 3;
	double BASE_POINT_X = WIDTH / 2;

	double BASELINE_TILE_WIDTH = 32;
	// Vanish points
 	double VANISH_POINT_1_X = BASE_POINT_X - tan(M_PI / 2 - ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);
	double VANISH_POINT_2_X = BASE_POINT_X + tan(ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);

	// Meause points
	double MEASURE_POINT_1_X = VANISH_POINT_1_X + GeometryUtil::hypot(
		BASE_POINT_Y - HORIZON_LINE_Y,
		BASE_POINT_X - VANISH_POINT_1_X);
	double MEASURE_POINT_2_X = VANISH_POINT_2_X - GeometryUtil::hypot(
		BASE_POINT_Y - HORIZON_LINE_Y,
		VANISH_POINT_2_X - BASE_POINT_X);

	Point vp1 = Point { VANISH_POINT_1_X, HORIZON_LINE_Y };
	Point vp2 = Point { VANISH_POINT_2_X, HORIZON_LINE_Y };
	Point mp1 = Point { MEASURE_POINT_1_X, HORIZON_LINE_Y };
	Point mp2 = Point { MEASURE_POINT_2_X, HORIZON_LINE_Y };
	Point bp = 	Point {BASE_POINT_X, BASE_POINT_Y };

	Line bp_vp2 = GeometryUtil::lineForTwoPoints(bp, vp2);
	Line bp_vp1 = GeometryUtil::lineForTwoPoints(bp, vp1);

	const int ENSURE_SCREEN_FULL_OF_TILE_FACTOR = 2;
	int baselineTiles = Game::WINDOW_WIDTH / BASELINE_TILE_WIDTH;
	int renderedPerspectiveTiles = baselineTiles * ENSURE_SCREEN_FULL_OF_TILE_FACTOR;

	Point baselinePointsRx[renderedPerspectiveTiles];
	Point baselinePointsSx[renderedPerspectiveTiles];

	// Baseline points
	for (int 	baselineTileIndex = 0;
				baselineTileIndex < renderedPerspectiveTiles;
				baselineTileIndex++) {
		baselinePointsSx[baselineTileIndex].x =
		 	BASE_POINT_X - BASELINE_TILE_WIDTH * baselineTileIndex;
		baselinePointsSx[baselineTileIndex].y = BASE_POINT_Y;

		baselinePointsRx[baselineTileIndex].x =
			BASE_POINT_X + BASELINE_TILE_WIDTH * baselineTileIndex;
		baselinePointsRx[baselineTileIndex].y = BASE_POINT_Y;
	}

	// Perspective points on <basepoint to vanish point> line
	//int halfRenderedPerspectiveTiles = renderedPerspectiveTiles / 2;
	const int QUADRANTS_0_90_INDEX = 0;
	const int QUADRANTS_90_180_INDEX = 1;
	const int QUADRANTS_180_270_INDEX = 2;
	const int QUADRANTS_270_360_INDEX = 3;
	const int QUADRANTS = 4;
	Point perspectivePoints[4][renderedPerspectiveTiles][renderedPerspectiveTiles];

	for (int q = 0; q < QUADRANTS; q++)
		perspectivePoints[q][0][0] = bp;

	// perspectivePoints[halfRenderedPerspectiveTiles][halfRenderedPerspectiveTiles] is BP
	for (int 	baselineTileIndex = 1;
				baselineTileIndex < renderedPerspectiveTiles;
				baselineTileIndex++) {
		Line pisx_mp1 = GeometryUtil::lineForTwoPoints(baselinePointsSx[baselineTileIndex], mp1);
		Line pisx_mp2 = GeometryUtil::lineForTwoPoints(baselinePointsSx[baselineTileIndex], mp2);
		Line pirx_mp1 = GeometryUtil::lineForTwoPoints(baselinePointsRx[baselineTileIndex], mp1);
		Line pirx_mp2 = GeometryUtil::lineForTwoPoints(baselinePointsRx[baselineTileIndex], mp2);

		perspectivePoints[QUADRANTS_0_90_INDEX][baselineTileIndex][0] =
		perspectivePoints[QUADRANTS_90_180_INDEX][baselineTileIndex][0] =
			GeometryUtil::intersectionForTwoLines(bp_vp2, pirx_mp2);

		perspectivePoints[QUADRANTS_0_90_INDEX][0][baselineTileIndex] =
		perspectivePoints[QUADRANTS_270_360_INDEX][0][baselineTileIndex] =
			GeometryUtil::intersectionForTwoLines(bp_vp1, pisx_mp1);


		perspectivePoints[QUADRANTS_180_270_INDEX][baselineTileIndex][0] =
		perspectivePoints[QUADRANTS_270_360_INDEX][baselineTileIndex][0] =
				GeometryUtil::intersectionForTwoLines(bp_vp2, pisx_mp2);

		perspectivePoints[QUADRANTS_90_180_INDEX][0][baselineTileIndex] =
		perspectivePoints[QUADRANTS_180_270_INDEX][0][baselineTileIndex] =
						GeometryUtil::intersectionForTwoLines(bp_vp1, pirx_mp1);
	}

	drawPoint(mMapGrid, perspectivePoints[QUADRANTS_0_90_INDEX][0][1], sf::Color::Red);
	drawPoint(mMapGrid, perspectivePoints[QUADRANTS_90_180_INDEX][1][0], sf::Color::Blue);
	drawPoint(mMapGrid, perspectivePoints[QUADRANTS_180_270_INDEX][0][1], sf::Color::Green);
	drawPoint(mMapGrid, perspectivePoints[QUADRANTS_270_360_INDEX][1][0], sf::Color::Yellow);

	drawPoint(mMapGrid, bp, sf::Color::Black);

	enum MatrixOffsetsFlag {
		NONE_FLAG = 0,
		RIGHT_OF_BP_FLAG = 1,
		LEFT_OF_BP_FLAG = 1 << 1,
		TOP_OF_BP_FLAG = 1 << 2,
		DOWN_OF_BP_FLAG = 1 << 3
	};

	struct MatrixOffsets {
		double angleThreshold;
		int rowOffsetFactor;
		int colOffsetFactor;
		int ensureFlags;
	} typedef MatrixOffsets;

	const int ANGLE_0_90_INDEX = 0;
	const int ANGLE_90_180_INDEX = 1;
	const int ANGLE_180_270_INDEX = 2;
	const int ANGLE_270_360_INDEX = 3;

	// Rows are indexed by quadrants and cols by angles

	const double ANGLE_0_90_THRESHOLD = M_PI / 2;
	const double ANGLE_90_180_THRESHOLD = M_PI;
	const double ANGLE_180_270_THRESHOLD = M_PI * 3 / 2;
	const double ANGLE_270_360_THRESHOLD = M_PI * 2;

	MatrixOffsets offsets[QUADRANTS][QUADRANTS];

	offsets[QUADRANTS_0_90_INDEX][ANGLE_0_90_INDEX] =
		{ ANGLE_0_90_THRESHOLD, 	-1,	   -1,	NONE_FLAG };
	offsets[QUADRANTS_0_90_INDEX][ANGLE_90_180_INDEX] =
		{ ANGLE_90_180_THRESHOLD, 	 1,    -1,	NONE_FLAG };
	offsets[QUADRANTS_0_90_INDEX][ANGLE_180_270_INDEX] =
	 	{ ANGLE_180_270_THRESHOLD,	 1, 	1,	NONE_FLAG };
	offsets[QUADRANTS_0_90_INDEX][ANGLE_270_360_INDEX] =
		{ ANGLE_270_360_THRESHOLD, 	-1, 	1,	NONE_FLAG };

	offsets[QUADRANTS_90_180_INDEX][ANGLE_0_90_INDEX] =
		{ ANGLE_0_90_THRESHOLD, 	-1,	     1,	RIGHT_OF_BP_FLAG };
	offsets[QUADRANTS_90_180_INDEX][ANGLE_90_180_INDEX] =
		{ ANGLE_90_180_THRESHOLD, 	 1,      1, LEFT_OF_BP_FLAG };
	offsets[QUADRANTS_90_180_INDEX][ANGLE_180_270_INDEX] =
	 	{ ANGLE_180_270_THRESHOLD,	 1, 	-1, RIGHT_OF_BP_FLAG };
	offsets[QUADRANTS_90_180_INDEX][ANGLE_270_360_INDEX] =
		{ ANGLE_270_360_THRESHOLD, 	-1, 	-1, LEFT_OF_BP_FLAG };

	offsets[QUADRANTS_180_270_INDEX][ANGLE_0_90_INDEX] =
		{ ANGLE_0_90_THRESHOLD, 	 1,	     1,	DOWN_OF_BP_FLAG };
	offsets[QUADRANTS_180_270_INDEX][ANGLE_90_180_INDEX] =
		{ ANGLE_90_180_THRESHOLD, 	-1,      1, DOWN_OF_BP_FLAG };
	offsets[QUADRANTS_180_270_INDEX][ANGLE_180_270_INDEX] =
		{ ANGLE_180_270_THRESHOLD,	-1, 	-1, DOWN_OF_BP_FLAG };
	offsets[QUADRANTS_180_270_INDEX][ANGLE_270_360_INDEX] =
		{ ANGLE_270_360_THRESHOLD, 	1, 		-1, DOWN_OF_BP_FLAG };

	offsets[QUADRANTS_270_360_INDEX][ANGLE_0_90_INDEX] =
		{ ANGLE_0_90_THRESHOLD, 	 1,	    -1,	LEFT_OF_BP_FLAG };
	offsets[QUADRANTS_270_360_INDEX][ANGLE_90_180_INDEX] =
		{ ANGLE_90_180_THRESHOLD, 	-1,     -1, RIGHT_OF_BP_FLAG };
	offsets[QUADRANTS_270_360_INDEX][ANGLE_180_270_INDEX] =
		{ ANGLE_180_270_THRESHOLD,	-1, 	 1, LEFT_OF_BP_FLAG };
	offsets[QUADRANTS_270_360_INDEX][ANGLE_270_360_INDEX] =
		{ ANGLE_270_360_THRESHOLD, 	 1, 	 1, RIGHT_OF_BP_FLAG };

	for (int q = QUADRANTS_0_90_INDEX; q < QUADRANTS; q++) {

		for (int r = 1; r < renderedPerspectiveTiles; r++) {
			for (int c = 1; c < renderedPerspectiveTiles; c++) {
				Line pirx_vp1 = GeometryUtil::lineForTwoPoints(
						perspectivePoints[q][r][0], vp1);
				Line picx_vp2 = GeometryUtil::lineForTwoPoints(
						perspectivePoints[q][0][c], vp2);
				perspectivePoints[q][r][c] =
					GeometryUtil::intersectionForTwoLines(pirx_vp1, picx_vp2);
				PerspectiveMatrixTile matrixTile;
				for (int a = ANGLE_0_90_INDEX; a < QUADRANTS; a++) {
					if (ANGLE < offsets[q][a].angleThreshold) {

						// Tests
						if (offsets[q][a].ensureFlags & RIGHT_OF_BP_FLAG) {
							if (!(perspectivePoints[q][r][c].x >= bp.x &&
								perspectivePoints[q][r - 1][c].x >= bp.x &&
								perspectivePoints[q][r][c - 1].x >= bp.x &&
								perspectivePoints[q][r - 1][c - 1].x >= bp.x))
									break;
						}
						if (offsets[q][a].ensureFlags & LEFT_OF_BP_FLAG) {
							if (!(perspectivePoints[q][r][c].x <= bp.x &&
								perspectivePoints[q][r - 1][c].x <= bp.x &&
								perspectivePoints[q][r][c - 1].x <= bp.x &&
								perspectivePoints[q][r - 1][c - 1].x <= bp.x))
									break;
						}
						if (offsets[q][a].ensureFlags & DOWN_OF_BP_FLAG) {
							if (!(perspectivePoints[q][r][c].y >= bp.y &&
								perspectivePoints[q][r - 1][c].y >= bp.y &&
								perspectivePoints[q][r][c - 1].y >= bp.y &&
								perspectivePoints[q][r - 1][c - 1].y >= bp.y))
									break;
						}

						if (offsets[q][a].ensureFlags & TOP_OF_BP_FLAG) {
							if (!(perspectivePoints[q][r][c].y <= bp.y &&
								perspectivePoints[q][r - 1][c].y <= bp.y &&
								perspectivePoints[q][r][c - 1].y <= bp.y &&
								perspectivePoints[q][r - 1][c - 1].y <= bp.y))
									break;
						}

						int rowAfterOffset = (int)ROW + r * offsets[q][a].rowOffsetFactor;
						int colAfterOffset = (int)COL + c * offsets[q][a].colOffsetFactor;
						matrixTile.tile = static_cast<MapTile>(
							mMatrix[rowAfterOffset][colAfterOffset]
						);
						matrixTile.rowOffset = rowAfterOffset;
						matrixTile.colOffset = colAfterOffset;

						matrixTile.vertices.setPrimitiveType(sf::Quads);
						matrixTile.vertices.clear();
						matrixTile.vertices.append(
							sf::Vertex( sf::Vector2f(	perspectivePoints[q][r][c].x,
														perspectivePoints[q][r][c].y))
						);
						matrixTile.vertices.append(
							sf::Vertex( sf::Vector2f(	perspectivePoints[q][r][c - 1].x,
														perspectivePoints[q][r][c - 1].y))
						);
						matrixTile.vertices.append(
							sf::Vertex( sf::Vector2f(	perspectivePoints[q][r - 1][c - 1].x,
														perspectivePoints[q][r - 1][c - 1].y))
						);
						matrixTile.vertices.append(
							sf::Vertex( sf::Vector2f(	perspectivePoints[q][r - 1][c].x,
														perspectivePoints[q][r - 1][c].y))
						);

						matrixTile.vertices[0].texCoords = sf::Vector2f(0, 0);
						matrixTile.vertices[1].texCoords = sf::Vector2f(15, 0);
						matrixTile.vertices[2].texCoords = sf::Vector2f(15, 15);
						matrixTile.vertices[3].texCoords = sf::Vector2f(0, 15);

						mPerspectiveTiles.push_front(matrixTile);
						break;
					}
				}
			}
		}
	}
		*/
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
