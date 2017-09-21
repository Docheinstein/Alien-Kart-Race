#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include "game.h"
#include "level.h"
#include "playerkart.h"
#include "map.h"
#include "fileutil.h"
#include "matrixutil.h"
#include "resourceutil.h"
#include "geometryutil.h"
#include "logger.h"

#define CAN_LOG 1
#define LOG_TAG "{Map} "

// Number of tiles that will be rendered in each direction.
const int MATRIX_RENDERED_TILES_RADIUS = 32;

// Total amount of tiles rendered per axis.
const int MATRIX_RENDERED_TILES_DIAMETER = MATRIX_RENDERED_TILES_RADIUS * 2 + 1;

Map::Map() {
	mMatrix = NULL;

	const double OUT_OF_SCREEN_TILES_ESTIMATED_PERCENTAGE = 0.5;
	mRenderedTiles.setPrimitiveType(sf::Quads);
    mRenderedTiles.resize(static_cast<int>(MATRIX_RENDERED_TILES_DIAMETER * MATRIX_RENDERED_TILES_DIAMETER * 4 * OUT_OF_SCREEN_TILES_ESTIMATED_PERCENTAGE));

	/*
	mWheelBoundTex.loadFromFile(ResourceUtil::image("earth_wheel_bound.png"));
    mWheelBoundSprite.setTexture(mWheelBoundTex);
    mWheelBoundSprite.setPosition(100, 100);
	*/

	mMapTexture.loadFromFile(ResourceUtil::image("earth_full.png"));
}

Map::~Map() {
	MatrixUtil::deleteMatrix<Tile>(mMatrix, mRowCount);
}

Map::TileEvent Map::getTileEvent(int row, int col) {
	// d("Asking row", row, " col", col);
	if (row >= 0 && row < mRowCount && col >= 0 && col < mColCount)
		return mMatrix[row][col].event;
	return Unpassable;
}

void Map::mapMatrixFillFunction(int readValue, int row, int col) {
	int halfCol = col / 2;
	bool firstOfPair = (col % 2 == 0);
	firstOfPair ?
		mMatrix[row][halfCol].tilesetY = readValue :
		mMatrix[row][halfCol].tilesetX = readValue;
}

void Map::eventsMatrixFillFunction(int readValue, int row, int col) {
	mMatrix[row][col].event = static_cast<TileEvent>(readValue);
}

void Map::loadMap(const char *mapFilename, const char *tilesetFilename) {
	MatrixUtil::deleteMatrix<Tile>(mMatrix, mRowCount);

	// Map
	mTileset.loadFromFile(ResourceUtil::image(tilesetFilename));
	int fileRowCount, fileColCount;

	const char * mapPath = ResourceUtil::raw(mapFilename);
	FileUtil::getMatrixSize<int>(mapPath, fileRowCount, fileColCount);

	mRowCount = fileRowCount;
	mColCount = fileColCount / 2;

	MatrixUtil::initMatrix<Tile>(mMatrix, mRowCount, mColCount);

    void (Map::*mapMatrixFillFunctionPtr)(int, int, int) = &Map::mapMatrixFillFunction;

	FileUtil::loadStructureFromFileKnowningSize<int, Map>(
		mapPath, fileRowCount, fileColCount, this, mapMatrixFillFunctionPtr);

	d("Loaded map at path: ", mapPath, " of size: ", mRowCount, "x", mColCount);

	// Map events
	const char * eventsMapPath = ResourceUtil::raw("earth_events.txt");

	void (Map::*eventsMatrixFillFunctionPtr)(int, int, int) = &Map::eventsMatrixFillFunction;

	FileUtil::loadStructureFromFileKnowningSize<int, Map>(
			eventsMapPath, mRowCount, mColCount, this, eventsMatrixFillFunctionPtr);
	d("Loaded events at path: ", eventsMapPath, " of size: ", mRowCount, "x", mColCount);
}

void Map::update() {

}

void Map::draw() {

	mDebugGridImage.create(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, sf::Color(255, 255, 255, 0));

	updateRenderedTiles();

	Game::instance().window()->draw(mRenderedTiles, &mTileset);

	// drawMapObjects();
	// Game::instance().window()->draw(mWheelBoundSprite);


	// sf::Texture mapGridTexture;
	// mapGridTexture.loadFromImage(mDebugGridImage);
	//
	// sf::Sprite mapGridSprite;
	// mapGridSprite.setTexture(mapGridTexture);
	//
	// Game::instance().window()->draw(mapGridSprite);
}

int Map::colCount() {
	return mColCount;
}

int Map::rowCount() {
	return mRowCount;
}
/*
void Map::drawMapObjects() {
	return;
		const int WIDTH = Game::WINDOW_WIDTH;
		const int HEIGHT = Game::WINDOW_HEIGHT;

		// Y coordinate of the horizon line.
		const int HORIZON_LINE_Y = HEIGHT / 2;

		// Amount of pixel a single tile will occupy.
		const int RENDERED_TILE_SIZE = 75;

		// Size of the tile in the tileset.
		const int TILE_SIZE = 15;

	 	Kart *k = Game::instance().level()->kart();

		// Angle between 0 and 6.28 representing the direction of the kart.
		const double ANGLE = k->direction();

		const int INT_COL = k->col();
		const int INT_ROW = k->row();

		// Basepoint
		const double BASE_POINT_Y = HEIGHT;
		const double BASE_POINT_X = WIDTH / 2;

		// Vanish points
	 	const double VANISH_POINT_1_X = BASE_POINT_X - tan(M_PI / 2 - ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);
		const double VANISH_POINT_2_X = BASE_POINT_X + tan(ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);

		const Point bp = Point { BASE_POINT_X, BASE_POINT_Y };
		const Point vp1 = Point { VANISH_POINT_1_X, HORIZON_LINE_Y };
		const Point vp2 = Point { VANISH_POINT_2_X, HORIZON_LINE_Y };

		// Calculate the perspective points since tiles shares contact points.

		struct TwoPoint {
			Point perspectivePoint;
			Point constructionPointForVP1;
			Point constructionPointForVP2;
		} typedef TwoPoint;

		TwoPoint perspectivePoints[MATRIX_RENDERED_TILES_DIAMETER + 1][MATRIX_RENDERED_TILES_DIAMETER + 1];

		for (int r = 0; r < MATRIX_RENDERED_TILES_DIAMETER + 1; r++) {
			for (int c = 0; c < MATRIX_RENDERED_TILES_DIAMETER + 1; c++) {
				// Indexes of the considered tile (absolute).
				const int absoluteMatrixIndexCol = INT_COL - MATRIX_RENDERED_TILES_RADIUS + c;
				const int absoluteMatrixIndexRow = INT_ROW - MATRIX_RENDERED_TILES_RADIUS + r;

				// Offset that the considered tile has respect to the kart tile (relative).
				const double relativeToCenterCol = k->col() - absoluteMatrixIndexCol;
				const double relativeToCenterRow = k->row() - absoluteMatrixIndexRow;

				// Offsets of the projected points on the baseline relative to the center.
				const double colOffset = (relativeToCenterCol) / cos (ANGLE);
				const double rowOffset = (relativeToCenterRow) / cos(M_PI / 2 - ANGLE);

				const Point pc = { bp.x - colOffset * RENDERED_TILE_SIZE, bp.y };
				const Point pr = { bp.x + rowOffset * RENDERED_TILE_SIZE, bp.y };

				// Line that join projected points on the baseline to the vanish points.
				const Line pc_vp2 = GeometryUtil::lineForTwoPoints(pc, vp2);
				const Line pr_vp1 = GeometryUtil::lineForTwoPoints(pr, vp1);

				// Definitive perspective points
				const Point pp = GeometryUtil::intersectionForTwoLines(pc_vp2, pr_vp1);

				perspectivePoints[r][c] = TwoPoint {pp, pr, pc};
			}
		}

		for (int r = 0; r < MATRIX_RENDERED_TILES_DIAMETER; r++) {
			for (int c = 0; c < MATRIX_RENDERED_TILES_DIAMETER; c++) {

				// Indexes of the considered tile (absolute).
				const int absoluteMatrixIndexCol = INT_COL - MATRIX_RENDERED_TILES_RADIUS + c;
				const int absoluteMatrixIndexRow = INT_ROW - MATRIX_RENDERED_TILES_RADIUS + r;

				const Point ppUL = perspectivePoints[r][c].perspectivePoint;
				const Point ppUR = perspectivePoints[r][c + 1].perspectivePoint;
				const Point ppDL = perspectivePoints[r + 1][c].perspectivePoint;
				const Point ppDR = perspectivePoints[r + 1][c + 1].perspectivePoint;

				const Point cpULforVP1 = perspectivePoints[r][c].constructionPointForVP1;
				const Point cpURforVP1 = perspectivePoints[r][c + 1].constructionPointForVP1;
				const Point cpDLforVP1 = perspectivePoints[r + 1][c].constructionPointForVP1;
				const Point cpDRforVP1 = perspectivePoints[r + 1][c + 1].constructionPointForVP1;

				const Point cpULforVP2 = perspectivePoints[r][c].constructionPointForVP2;
				const Point cpURforVP2 = perspectivePoints[r][c + 1].constructionPointForVP2;
				const Point cpDLforVP2 = perspectivePoints[r + 1][c].constructionPointForVP2;
				const Point cpDRforVP2 = perspectivePoints[r + 1][c + 1].constructionPointForVP2;

				const bool insideScreenHeight =
					(ppUL.y < HEIGHT ||
					ppUR.y < HEIGHT ||
					ppDL.y < HEIGHT ||
					ppDR.y < HEIGHT);

				const bool insideScreenWidth =
					// Ensure the points are right to left border
					(ppUL.x > 0 ||
					ppUR.x > 0 ||
					ppDL.x > 0 ||
					ppDR.x > 0)
					 	&&
					// Ensure the points are left to right border
					(ppUL.x < WIDTH ||
					ppUR.x < WIDTH ||
					ppDL.x < WIDTH ||
					ppDR.x < WIDTH);

				const bool underHorizonLine =
					(ppUL.y > HORIZON_LINE_Y &&
					ppUR.y > HORIZON_LINE_Y &&
					ppDR.y > HORIZON_LINE_Y &&
					ppDL.y > HORIZON_LINE_Y);

				if (insideScreenHeight && insideScreenWidth && underHorizonLine &&
					absoluteMatrixIndexCol >= 0 && absoluteMatrixIndexCol < mColCount &&
					absoluteMatrixIndexRow >= 0 && absoluteMatrixIndexRow < mRowCount) {

					if (mObjMatrix[absoluteMatrixIndexRow][absoluteMatrixIndexCol].type == 11) {
						//if (absoluteMatrixIndexRow == 0 && absoluteMatrixIndexCol == 51) {

						Point middleConstructionPoint = {(cpULforVP2.x + cpDRforVP2.x) / 2, BASE_POINT_Y};

						Line ul_dr = GeometryUtil::lineForTwoPoints(ppUL, ppDR);
						Line dl_ur = GeometryUtil::lineForTwoPoints(ppDL, ppUR);

						Point middlePoint = GeometryUtil::intersectionForTwoLines(ul_dr, dl_ur);

						// cpULforVP1

						// d("Drawing obj");
						double distanceMiddle_VP = GeometryUtil::distanceBetweenTwoPoints(middlePoint, vp2);
						double distanceConstruction_VP = GeometryUtil::distanceBetweenTwoPoints(middleConstructionPoint, vp2);

						const int WHEEL_BOUND_SIZE = RENDERED_TILE_SIZE * 1.5;
						double perspectiveSize = distanceMiddle_VP / distanceConstruction_VP * WHEEL_BOUND_SIZE;

						sf::Sprite s;
						s.setTexture(mWheelBoundTex);
						double scaleForReachPerspectiveSize = perspectiveSize / s.getGlobalBounds().height;
						s.setScale(sf::Vector2f(scaleForReachPerspectiveSize, scaleForReachPerspectiveSize));


						s.setPosition(	middlePoint.x - s.getGlobalBounds().width / 2,
						 				middlePoint.y - s.getGlobalBounds().height);


						// d("distanceoPP_VP: ", distanceoPP_VP);
						// d("distanceoCP_VP: ", distanceoCP_VP);
						// d("perspectiveHeight: ", perspectiveHeight);
						// d("GlobalBoundHeight: ", s.getGlobalBounds().height);
						// d("scaleForReachHeight: ", scaleForReachHeight);


						drawPoint(&mDebugGridImage, middlePoint, sf::Color::Cyan, 1);

						Game::instance().window()->draw(s);
					//}
					}
				}
			}
		}
}
*/

void Map::updateRenderedTiles() {
	const int WIDTH = Game::WINDOW_WIDTH;
	const int HEIGHT = Game::WINDOW_HEIGHT;

	// Y coordinate of the horizon line.
	const int HORIZON_LINE_Y = HEIGHT / 2 + 20;

	// Amount of pixel a single tile will occupy.
	const int RENDERED_TILE_SIZE = 75 * WIDTH / 320;

	// Size of the tile in the tileset.
	const int TILE_SIZE = 15;

 	PlayerKart *k = Game::instance().level()->playerKart();

	// Angle between 0 and 6.28 representing the direction of the kart.
	const double ANGLE = k->direction();

	// The camera is 1 unit behind the kart
	const int CAMERA_TILE_BEHIND_PLAYER_COUNT = 1;
	const double cameraRow = k->row() + cos(ANGLE) * CAMERA_TILE_BEHIND_PLAYER_COUNT;
	const double cameraCol = k->col() - sin(ANGLE) * CAMERA_TILE_BEHIND_PLAYER_COUNT;

	const int INT_COL = cameraCol;
	const int INT_ROW = cameraRow;

	// Basepoint
	const double BASE_POINT_Y = HEIGHT + 20/* - Kart::MARGIN_FROM_BOTTOM */;
	const double BASE_POINT_X = WIDTH / 2;

	// Vanish points
 	const double VANISH_POINT_1_X = BASE_POINT_X - tan(M_PI / 2 + ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);
	const double VANISH_POINT_2_X = BASE_POINT_X + tan(-ANGLE) * (BASE_POINT_Y - HORIZON_LINE_Y);

	const Point bp = Point { BASE_POINT_X, BASE_POINT_Y };
	const Point vp1 = Point { VANISH_POINT_1_X, HORIZON_LINE_Y };
	const Point vp2 = Point { VANISH_POINT_2_X, HORIZON_LINE_Y };

	// d("bp ", bp);
	// d("vp1 ", vp1);
	// d("vp2 ", vp2);
	// d("ANGLE ", ANGLE);
	// d("k->col() ", k->col());
	// d("k->row() ", k->row());
	// d("");

	// Calculate the perspective points since tiles shares contact points.
	Point perspectivePoints[MATRIX_RENDERED_TILES_DIAMETER + 1][MATRIX_RENDERED_TILES_DIAMETER + 1];

	for (int r = 0; r < MATRIX_RENDERED_TILES_DIAMETER + 1; r++) {
		for (int c = 0; c < MATRIX_RENDERED_TILES_DIAMETER + 1; c++) {
			// Indexes of the considered tile (absolute).
			const int absoluteMatrixIndexCol = INT_COL - MATRIX_RENDERED_TILES_RADIUS + c;
			const int absoluteMatrixIndexRow = INT_ROW - MATRIX_RENDERED_TILES_RADIUS + r;

			// Offset that the considered tile has respect to the kart tile (relative).
			const double relativeToCenterCol = cameraCol - absoluteMatrixIndexCol;
			const double relativeToCenterRow = cameraRow - absoluteMatrixIndexRow;

			// Offsets of the projected points on the baseline relative to the center.
			const double colOffset = (relativeToCenterCol) / cos (-ANGLE);
			const double rowOffset = (relativeToCenterRow) / cos(M_PI / 2 + ANGLE);

			const Point pc = { bp.x - colOffset * RENDERED_TILE_SIZE, bp.y };
			const Point pr = { bp.x + rowOffset * RENDERED_TILE_SIZE, bp.y };

			// Line that join projected points on the baseline to the vanish points.
			const Line pc_vp2 = GeometryUtil::lineForTwoPoints(pc, vp2);
			const Line pr_vp1 = GeometryUtil::lineForTwoPoints(pr, vp1);

			// Definitive perspective points
			const Point pp = GeometryUtil::intersectionForTwoLines(pc_vp2, pr_vp1);

			perspectivePoints[r][c] = pp;

			// d("Perspective point at (", r, ", ", c, "): ", pp);
			// d("absoluteMatrixIndexRow ", absoluteMatrixIndexRow);
			// d("absoluteMatrixIndexCol ", absoluteMatrixIndexCol);
			// d("relativeToCenterRow ", relativeToCenterRow);
			// d("relativeToCenterCol ", relativeToCenterCol);
			// d("rowOffset ", rowOffset);
			// d("colOffset ", colOffset);
			// d("pc ", pc);
			// d("pr ", pr);
			// d("pc_vp2 ", pc_vp2);
			// d("pr_vp1 ", pr_vp1);
			// d("");
			// if (r == 0) {
			// 	drawPoint(&mDebugGridImage, pp, sf::Color::Red, 5);
			// }
			// else if (c == 0) {
			// 	drawPoint(&mDebugGridImage, pp, sf::Color::Black, 5);
			// }
			// else {
			// 	drawPoint(&mDebugGridImage, pp, sf::Color::Red, 3);
			// }
		}
	}

	int inScreenTiles = 0;
	mRenderedTiles.clear();

	for (int r = 0; r < MATRIX_RENDERED_TILES_DIAMETER; r++) {
		for (int c = 0; c < MATRIX_RENDERED_TILES_DIAMETER; c++) {

			// Indexes of the considered tile (absolute).
			const int absoluteMatrixIndexCol = INT_COL - MATRIX_RENDERED_TILES_RADIUS + c;
			const int absoluteMatrixIndexRow = INT_ROW - MATRIX_RENDERED_TILES_RADIUS + r;

			const Point ppUL = perspectivePoints[r][c];
			const Point ppUR = perspectivePoints[r][c + 1];
			const Point ppDL = perspectivePoints[r + 1][c];
			const Point ppDR = perspectivePoints[r + 1][c + 1];

			const bool insideScreenHeight =
				(ppUL.y < HEIGHT ||
				ppUR.y < HEIGHT ||
				ppDL.y < HEIGHT ||
				ppDR.y < HEIGHT);

			const bool insideScreenWidth =
				// Ensure the points are right to left border
				(ppUL.x > 0 ||
				ppUR.x > 0 ||
				ppDL.x > 0 ||
				ppDR.x > 0)
				 	&&
				// Ensure the points are left to right border
				(ppUL.x < WIDTH ||
				ppUR.x < WIDTH ||
				ppDL.x < WIDTH ||
				ppDR.x < WIDTH);

			const bool underHorizonLine =
				(ppUL.y > HORIZON_LINE_Y &&
				ppUR.y > HORIZON_LINE_Y &&
				ppDR.y > HORIZON_LINE_Y &&
				ppDL.y > HORIZON_LINE_Y);

			if (insideScreenHeight && insideScreenWidth && underHorizonLine &&
				absoluteMatrixIndexCol >= 0 && absoluteMatrixIndexCol < mColCount &&
				absoluteMatrixIndexRow >= 0 && absoluteMatrixIndexRow < mRowCount) {

				int tilesetY = mMatrix[absoluteMatrixIndexRow][absoluteMatrixIndexCol].tilesetY;
				int tilesetX = mMatrix[absoluteMatrixIndexRow][absoluteMatrixIndexCol].tilesetX;

				mRenderedTiles.append(sf::Vertex(
					sf::Vector2f(ppUL.x, ppUL.y),
					sf::Vector2f(tilesetX, tilesetY)
				));
				mRenderedTiles.append(sf::Vertex(
					sf::Vector2f(ppUR.x, ppUR.y),
					sf::Vector2f(tilesetX + TILE_SIZE, tilesetY)
				));
				mRenderedTiles.append(sf::Vertex(
					sf::Vector2f(ppDR.x, ppDR.y),
					sf::Vector2f(tilesetX + TILE_SIZE, tilesetY + TILE_SIZE)
				));
				mRenderedTiles.append(sf::Vertex(
					sf::Vector2f(ppDL.x, ppDL.y),
					sf::Vector2f(tilesetX, tilesetY + TILE_SIZE)
				));

				inScreenTiles++;


				drawPoint(&mDebugGridImage, ppUL, sf::Color::Blue, 1);
				drawPoint(&mDebugGridImage, ppUR, sf::Color::Blue, 1);
				drawPoint(&mDebugGridImage, ppDR, sf::Color::Blue, 1);
				drawPoint(&mDebugGridImage, ppDL, sf::Color::Blue, 1);
			}
			// else {
			// 	d("Drawing out of screen point at rc: (", r, ", ", c, "), abs matrix index(", absoluteMatrixIndexRow , ", ", absoluteMatrixIndexCol, ")");
			// 	drawPoint(&mDebugGridImage, ppUL, sf::Color::Blue, 3);
			// 	drawPoint(&mDebugGridImage, ppUR, sf::Color::Blue, 3);
			// 	drawPoint(&mDebugGridImage, ppDR, sf::Color::Blue, 3);
			// 	drawPoint(&mDebugGridImage, ppDL, sf::Color::Blue, 3);
			// }
		}
	}

	// d("In screen tiles:", inScreenTiles);
}
void Map::drawPoint(sf::Image * map, const Point &p, sf::Color color, int size = 7) {
	if (p.x < 0 + size / 2 || p.x >= Game::WINDOW_WIDTH - size / 2 || p.y < 0 + size / 2 || p.y >= Game::WINDOW_HEIGHT - size / 2)
		return;
	for (int x = - size / 2; x <= size / 2; x++) {
		for (int y = - size / 2; y <= size / 2; y++) {
			map->setPixel(p.x + x, p.y + y, color);

		}
	}
}

void Map::drawPoint(sf::Image * map, const IPoint &p, sf::Color color, int size = 7) {
	if (p.x < 0 + size / 2 || p.x >= Game::WINDOW_WIDTH - size / 2 || p.y < 0 + size / 2 || p.y >= Game::WINDOW_HEIGHT - size / 2)
		return;
	for (int x = - size / 2; x <= size / 2; x++) {
		for (int y = - size / 2; y <= size / 2; y++) {
			map->setPixel(p.x + x, p.y + y, color);
		}
	}
}

void Map::drawLine(double x1, double y1, double x2, double y2, double thickness) {
	sf::VertexArray lineThick(sf::Quads, 4);

	lineThick[0].position = sf::Vector2f(x1 - thickness / 2, y1 - thickness / 2);
	lineThick[1].position = sf::Vector2f(x2 + thickness / 2, y2 - thickness / 2);
	lineThick[2].position = sf::Vector2f(x2 + thickness / 2, y2 + thickness / 2);
	lineThick[3].position = sf::Vector2f(x1 - thickness / 2, y1 + thickness / 2);

	lineThick[0].color = sf::Color::Red;
	lineThick[1].color = sf::Color::Red;
	lineThick[2].color = sf::Color::Red;
	lineThick[3].color = sf::Color::Red;
	Game::instance().window()->draw(lineThick);
}

const char * Map::logTag() {
	return LOG_TAG;
}

bool Map::canLog() {
	return CAN_LOG;
}
