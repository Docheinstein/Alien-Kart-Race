#ifndef MAP_H
#define MAP_H

#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "game.h"

class Map {
public:
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

private:
	enum MapTile {
		Empty = 0,
		Street = 1,
		Empty2 = 3
	};

	struct PerspectiveMatrixTile {
		MapTile tile;
		sf::VertexArray vertices; // VertexArray
	} typedef PerspectiveMatrixTile;

	int **mMatrix;
	int mColCount;
	int mRowCount;
	sf::Image *mMapGrid;
	sf::Image *mMiniMap;
	std::list<PerspectiveMatrixTile> mPerspectiveTiles;
	PerspectiveMatrixTile aTile;

	std::string getMapFilePath(MapType mapType);
	void createMapGrid();
	void createMiniMap();

	void drawMapGrid();
};

#endif // MAP_H
