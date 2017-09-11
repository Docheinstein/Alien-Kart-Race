#ifndef MAP_H
#define MAP_H

#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "game.h"
#include "geometryutil.h"

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

	// DEBUG
	int getTile(int row, int col);
private:
	// Y coordinate of the horizon line.
	static const double HORIZON_LINE_Y;

	enum MapTileType {
		Invalid = -1,
		Empty = 0,
		Fulfilled = 1,
		Event = 4,


		EarthStreet1 = 11,
		EarthStreet2 = 12,
		EarthStreet3 = 13,
		EarthStreet4 = 14,
		EarthStreet5 = 15,
		EarthStreet6 = 16,

		EarthGrass1 = 21,
		EarthGrass2 = 22,
		EarthGrass3 = 23,
	};

	struct PerspectiveMatrixTile {
		MapTileType tile;
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
	void drawHorizonBackground();
	void drawPoint(sf::Image * map, const Point &p, sf::Color color, int size);
	void drawPoint(sf::Image * map, const IPoint &p, sf::Color color, int size);
	void drawLine(double x1, double y1, double x2, double y2);

	sf::Texture * textureForTileType(MapTileType MapTileType);

	sf::Texture mEarthGrass1Texture;
	sf::Texture mEarthGrass2Texture;
	sf::Texture mEarthGrass3Texture;

	sf::Texture mEarthStreet1Texture;
	sf::Texture mEarthStreet2Texture;
	sf::Texture mEarthStreet3Texture;
	sf::Texture mEarthStreet4Texture;
	sf::Texture mEarthStreet5Texture;
	sf::Texture mEarthStreet6Texture;

	sf::Sprite mHorizonBackgroundSprite;
};

#endif // MAP_H
