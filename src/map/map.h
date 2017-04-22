#ifndef MAP_H
#define MAP_H

#include <list>
#include <SFML/Graphics.hpp>

class Map {
public:
	enum MapType {
		FirstMap
	};

	static const int MINI_MAP_X = 320;
	static const int MINI_MAP_Y = 240;
	static const int MINI_MAP_WIDTH = 200;
	static const int MINI_MAP_HEIGHT = 200;

	Map();
	~Map();

	void loadMap(MapType mapType);
	void update();
	void drawMap();
	void drawMiniMap();

	int colCount();
	int rowCount();

private:
	struct PerspectiveMatrixTile {
		sf::VertexArray mVertices; // VertexArray
		int mColFromCenter; // Distance of matrix cols from the center ( WIDTH / 2)
		int mRowFromBaseline; // Distance of matrix rows from the baseline ( HEIGHT )
	} typedef PerspectiveMatrixTile;

	int **mMatrix;
	int mColCount;
	int mRowCount;
	sf::Image *mMapGrid;
	sf::Image *mMiniMap;
	std::list<PerspectiveMatrixTile> mPerspectiveTiles;

	static const char * getMapFilePath(MapType mapType);
	void createMapGrid();
	void createMiniMap();
};

#endif // MAP_H
