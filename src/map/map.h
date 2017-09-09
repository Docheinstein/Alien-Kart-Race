#ifndef MAP_H
#define MAP_H

#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "game.h"
#include "geometryutil.h"

class Map {
public:
	// Test
	static void generatePerspectiveSystemCache();

	enum MapType {
		FirstMap
	};

	static const int MINI_MAP_X = Game::WINDOW_WIDTH / 2;
	static const int MINI_MAP_Y = 10;
	static const int MINI_MAP_WIDTH = Game::WINDOW_WIDTH / 4;
	static const int MINI_MAP_HEIGHT = Game::WINDOW_HEIGHT / 4;

	Map();
	~Map();

	void loadMap(MapType mapType);
	void update();
	void drawMap();
	void drawMiniMap();

	int colCount();
	int rowCount();

	// DEBUG
	int getTile(int row, int col);
private:
	enum MapTile {
		Empty = 0,
		Street = 1,
		Empty2 = 3,
		Event = 4
	};

	struct PerspectiveMatrixTile {
		MapTile tile;
		sf::VertexArray vertices; // VertexArray
	} typedef PerspectiveMatrixTile;


	static IPoint * sCachedPerspectivePoints;

	int **mMatrix;
	int mColCount;
	int mRowCount;
	sf::Image *mMapGrid;
	sf::Image *mMiniMap;
	std::list<PerspectiveMatrixTile> mPerspectiveTiles;
	PerspectiveMatrixTile aTile;

	static IPoint getCachedMapPerspectivePoint(
		int matrixRowIndex,
		int matrixColIndex,
		int tileInternalRowIndex,
		int tileInternalColIndex,
		int dirIndex
	);

	static void setCachedMapPerspectivePoint(
		int matrixRowIndex,
		int matrixColIndex,
		int tileInternalRowIndex,
		int tileInternalColIndex,
		int dirIndex,
		const IPoint &p
	);

	std::string getMapFilePath(MapType mapType);
	void createMapGrid();
	void createMapGridOnDemand();
	void createMiniMap();

	void drawMapGrid();
};

#endif // MAP_H
