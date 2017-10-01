#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "loggerinterface.h"
#include "geometryutil.h"

namespace sf {
	class RenderWindow;
}

class Level;

class Map : public LoggerInterface {
public:
	enum SectorDirectionType {
		UpLeft,
		UpRight,
		RightUp,
		RightDown,
		DownRight,
		DownLeft,
		LeftUp,
		LeftDown
	};

	struct Sector {
		Quad quad;
		SectorDirectionType type;
		int index;
	} typedef Sector;

	enum TileEvent {
		Unpassable = 0,
		Passable = 1,
		Slow = 2,
		LapFinished = 3
	};

	Map(sf::RenderWindow *window, Level *level);
	virtual ~Map();

 	virtual void loadMap() = 0;
	virtual Point startingPointForRacePosition(int racePosition) = 0;
	virtual int lastRacePosition() = 0;

	void update();
	void draw();

	int colCount();
	int rowCount();

	TileEvent tileEvent(const Point &p);
	Sector sector(const Point &p);

	int sectorCount();

	int aiPathCount();
	const char *aiPath(unsigned int index);
	const char *randomAIPath();

protected:
	 void loadMap(const char *mapFilename);
	 void loadEvents(const char *eventsFilename);
 	 void loadTileset(const char *tilesetFilename);
  	 void loadSectors(const char *sectorsFilename);
	 void addAIPath(const char *aiPathFilename);

private:
	struct Tile {
		int tilesetY;
		int tilesetX;
		TileEvent event;
	} typedef Tile;

	sf::RenderWindow *mWindow;
	Level *mLevel;

	Tile **mMatrix;

	// MapObject **mObjMatrix;

	int mColCount;
	int mRowCount;

	sf::Image mDebugGridImage;

	sf::VertexArray mRenderedTiles;
	sf::Texture mTileset;

	std::vector<Sector> mSectors;

	std::vector<const char *> mAIPathsFilename;

	void updateRenderedTiles();
	void drawMapObjects();

	void drawPoint(sf::Image * map, const Point &p, sf::Color color, int size);
	void drawPoint(sf::Image * map, const IPoint &p, sf::Color color, int size);
	void drawLine(double x1, double y1, double x2, double y2, double thickness);
	void mapMatrixFillFunction(int readVal, int fileRow, int fileCol);
	void eventsMatrixFillFunction(int readVal, int fileRow, int fileCol);
	void sectorsFillFunction(int readVal, int fileRow, int fileCol);

    const char *logTag();
    bool canLog();

};

#endif // MAP_H
