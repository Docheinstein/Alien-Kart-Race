#ifndef MAP_H
#define MAP_H

#include <vector>
#include "loggerinterface.h"
#include "geometryentities.h"

namespace sf {
	class Image;
	class Color;
	class RenderWindow;
	class Texture;
	class Sprite;
	class VertexArray;
}

class Level;

/** Base class for the level's map. */
class Map : public LoggerInterface {
public:

	/**
	 * Type of the sector.
	 * Each sector have a type which specify how a kart
	 * should be considered within it (i.e. in a UpLeft
	 * sector a kart on the up-left corner is considered
	 * as further away in the lap respect to a kart on the
	 * bottom right corner).
	 */
	 enum SectorDirectionType {
		/**
		 * The further point of the sector is the up edge.
		 * In parity cases, the kart on the left is considered furter.
		 */
		UpLeft,

		/**
		 * The further point of the sector is the up edge.
		 * In parity cases, the kart on the right is considered furter.
		 */
		UpRight,

		/**
		 * The further point of the sector is the right edge.
		 * In parity cases, the kart nearest to the up edge is considered furter.
		 */
		RightUp,

		/**
		 * The further point of the sector is the up edge.
		 * In parity cases, the kart nearest to the bottom edge is considered furter.
		 */
		RightDown,

		/**
		 * The further point of the sector is the down edge.
		 * In parity cases, the kart on the right is considered furter.
		 */
		DownRight,

		/**
		 * The further point of the sector is the down edge.
		 * In parity cases, the kart on the left is considered furter.
		 */
		DownLeft,

		/**
		 * The further point of the sector is the left edge.
		 * In parity cases, the kart nearest to the up edge is considered furter.
		 */
		LeftUp,

		/**
		 * The further point of the sector is the left edge.
		 * In parity cases, the kart nearest to the down edge is considered furter.
		 */
		LeftDown
	};

	/** Represents a portion of map that have an unique sector direction. */
	struct Sector {
		/** Quad of the map's section. */
		Quad quad;

		/** Type of the sector. */
		SectorDirectionType type;

		/**
		 * Incremental index associated with sector.
		 * An higher index means that the sector is further in the map progress.
		 */
		int index;
	} typedef Sector;

	/** Action associated with a tile. */
	enum TileEvent {
		/** Tile can't be passed through. */
		Unpassable = 0,

		/** Tile can be passed through. */
		Passable = 1,

		/** Tile can be passed through but slow down the entities on it. */
		Slow = 2,

		/** Tile notifies the lap's end. It is intrinsically considered as passable. */
		LapFinished = 3
	};

    /**
     * Creates a new empty map.
     * @param window      the window this screen is encapsulated in.
     * @param keysManager the optional keyboard manager that will notify this
     *                    screen about async key pressure.
     */
	Map(sf::RenderWindow *window, Level *level);
	virtual ~Map();

	/** Loads the map. */
 	virtual void loadMap() = 0;

	/**
	 * Returns the point on the loaded map associated with the given race position.
	 * @param  racePosition the race position.
	 * @return              the point associated with the given race position.
	 */
	virtual Point startingPointForRacePosition(int racePosition) = 0;

	/**
	 * Returns the race position for the last kart.
	 * @return the race position for the last kart.
	 */
	virtual int lastRacePosition() = 0;

	/** Updates the logic of the map (if any). */
	void update();

	/** Draws the portion of visible map on the screen. */
	void draw();

	/**
	 * Returns the column count of the currently loaded map.
	 * @return the column count of the currently loaded map.
	 */
	int colCount();

	/**
	 * Returns the row count of the currently loaded map.
	 * @return the row count of the currently loaded map.
	 */
	int rowCount();

	/**
	 * Returns the sector associated with the given point.
	 * @param  p a point on the map.
	 * @return  the sector associated with the given point.
	 */
	Sector sector(const Point &p);

	/**
	 * Returns the sector count for the currently loaded map.
	 * @return the sector count for the currently loaded map.
	 */
	int sectorCount();

	/**
	 * Returns the event associated with the tile at the given point.
	 * @param  p a point on the map.
	 * @return  the event associated with tile that contains the given point.
	 */
	TileEvent tileEvent(const Point &p);


	/**
	 * Returns the filename of the AI path with the given index.
	 * If the index is out of range, a random path is returned.
	 * @param  index the index of the AI path.
	 * @return       the filename of the AI path at the given index.
	 */
	const char *aiPath(unsigned int index);

	/**
	 * Returns a random AI path.
	 * @return a random AI path.
	 */
	const char *randomAIPath();

	/**
	 * Returns the number of AI path available for the currently loaded map.
	 * @return the number of AI path available for the currently loaded map.
	 */
	int aiPathCount();

protected:
	/** Color used for the background. */
	sf::Color 	*mBackgroundFillColor;

	/**
	 * Loads the map from the raw file with the given name.
	 * @param mapFilename the name of the raw file containing the map.
	 */
	void loadMap(const char *mapFilename);

	/**
	 * Loads the events associated with the map from the raw file with the given name.
	 * @param eventsFilename the name of the raw file containing the events.
	 */
	void loadEvents(const char *eventsFilename);

	/**
	 * Loads the tileset to use for the map from the image file with the given name.
	 * @param tilesetFilename the name of the image file containing the tileset.
	 */
	void loadTileset(const char *tilesetFilename);

	/**
	 * Loads the sectors from the raw file with the given name.
	 * @param sectorsFilename the name of the raw file containing the sectors.
	 */
	void loadSectors(const char *sectorsFilename);

	/** Loads the background sprite for the given background fill color. */
	void loadBackground(sf::Color *backgroundFillColor);

	/**
	 * Adds an AI path to the collection of paths available for AI.
	 * @param aiPathFilename the filename of the ai path.
	 */
	void addAIPath(const char *aiPathFilename);

private:
	/**
	 * Represents a tile on the map, which consinst in the coordinate of
	 * the image in the tileset and the associated event.
	 */
	struct Tile {
		/** The Y position of the image of this tile in the tileset. */
		int tilesetY;


		/** The X position of the image of this tile in the tileset. */
		int tilesetX;

		/** The event associated with this tile. */
		TileEvent event;
	} typedef Tile;

	/** Window this map will be rendered to. */
	sf::RenderWindow *mWindow;

	/** Level this map is involved in. */
	Level *mLevel;

	/** Matrix of tiles, representing the currently loaded map. */
	Tile **mMatrix;

	/** Number of columns of the currently loaded map. */
	int mColCount;

	/** Number of rows of the currently loaded map. */
	int mRowCount;

	/** Contains the perspective coordinate of the map's tile. */
	sf::VertexArray *mRenderedTiles;

	/** Tileset from which the images for the tiles will be taken. */
	sf::Texture *mTileset;

	/** Sectors of the loaded map. */
	std::vector<Sector> mSectors;

	/** List of available AI path. */
	std::vector<const char *> mAIPathsFilename;

	/** Image used for draw the perspective points (debug purpose). */
	sf::Image 	*mDebugGridImage;

	/** Texture used for draw the perspective points (debug purpose). */
	sf::Texture *mDebugGridTexture;

	/** Sprite used for draw the perspective points (debug purpose) */
	sf::Sprite 	*mDebugGridSprite;

	/** Texture used for draw the background of the map. */
	sf::Texture *mBackgroundFillColorTexture;

	/** Sprite used for draw the background of the map. */
	sf::Sprite 	*mBackgroundFillColorSprite;

	/**
	 * Updates the rendered tiles by filling the vertex array
	 * with the appropriate coordinate and tiles.
	 * The tiles are drawn once in order to optimaze the impact on the GPU.
	 */
	void updateRenderedTiles();

	/**
	 * Function used for fill the matrix with the tiles loaded from the file.
	 * @param readVal the value read from the file.
	 * @param fileRow the row of the read value.
	 * @param fileCol the column of the read value.
	 */
	void mapMatrixFillFunction(int readVal, int fileRow, int fileCol);

	/**
	 * Function used for fill the matrix with the events loaded from the file.
	 * @param readVal the value read from the file.
	 * @param fileRow the row of the read value.
	 * @param fileCol the column of the read value.
	 */
	void eventsMatrixFillFunction(int readVal, int fileRow, int fileCol);

	/**
	 * Function used for fill the matrix with the sectors loaded from the file.
	 * @param readVal the value read from the file.
	 * @param fileRow the row of the read value.
	 * @param fileCol the column of the read value.
	 */
	void sectorsFillFunction(int readVal, int fileRow, int fileCol);

	// DEBUG METHODS

	/**
	 * Draws a point on the map with the given color and size.
	 * @param map   the map's image.
	 * @param p     the point.
	 * @param color the color.
	 * @param size  the size.
	 */
	void drawPoint(sf::Image * map, const Point &p, int size);

	/**
	 * Draws a line on the map for the given coordinates and thickness.
	 * @param x1        the left coordinate.
	 * @param y1        the top coordinate.
	 * @param x2        the right coordinate.
	 * @param y2        the bottom coordinate.
	 * @param thickness the thickness of the line.
	 */
	void drawLine(double x1, double y1, double x2, double y2, double thickness);

	// END DEBUG METHODS

    const char *logTag();
    bool canLog();

};

#endif // MAP_H
