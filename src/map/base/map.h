#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "game.h"
#include "geometryutil.h"

class Map : public LoggerInterface {
public:
	enum TileEvent {
		Unpassable = 0,
		Passable = 1,
		Slow = 2
	};

	Map();
	~Map();

 	virtual void loadMap() = 0;

	void update();
	void draw();

	int colCount();
	int rowCount();

	TileEvent getTileEvent(int row, int col);

protected:
	 void loadMap(const char * rawFilename, const char *tilesetFilename);

private:
	struct Tile {
		int tilesetY;
		int tilesetX;
		TileEvent event;
	} typedef Tile;

	Tile **mMatrix;

	// MapObject **mObjMatrix;

	int mColCount;
	int mRowCount;

	sf::Image mDebugGridImage;
	sf::Texture mMapTexture;

	sf::VertexArray mRenderedTiles;
	sf::Texture mTileset;

	// sf::Texture mWheelBoundTex;
	// sf::Sprite mWheelBoundSprite;

	void updateRenderedTiles();
	void drawMapObjects();

	void drawPoint(sf::Image * map, const Point &p, sf::Color color, int size);
	void drawPoint(sf::Image * map, const IPoint &p, sf::Color color, int size);
	void drawLine(double x1, double y1, double x2, double y2, double thickness);
	void mapMatrixFillFunction(int readVal, int fileRow, int fileCol);
	void eventsMatrixFillFunction(int readVal, int fileRow, int fileCol);

    const char *logTag();
    bool canLog();

};

#endif // MAP_H
