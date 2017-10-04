#ifndef AI_KART_H
#define AI_KART_H

#include "kart.h"

class Point;

/** Base entity representing a kart controller by the CPU. */
class AIKart : public Kart {
public:
	/**
	 * Creates a new AI kart.
	 * @param level     the level this karts will be part of.
	 * @param kartName  the name that will be given to this kart
	 *                  (and will be shown on the leaderboard).
	 * @param kartColor the color that will be given to this kart
	 *                  (used for leaderboard and minimap).
	 * @param pathName  the filename of the path that will
	 *                   be used by this kart.
	 */
	AIKart(	Level *level, const char *kartName,
			sf::Color *kartColor, const char *pathName);
	virtual ~AIKart();

    virtual void update();
    virtual void draw();

    int minimapSize();

protected:
	/**
	 * Represents a path that will be followed by the kart.
	 * A path basically consist in a bunch of consecutive points.
	 */
    struct AIPath {
        Point *points;
        int pointCount;
    } typedef AIPath;

	/** The path followed by this kart. */
    AIPath mPath;

	/**
	 *  Name of the file that contains the information about the path's points,
	 *  in the appropriate format.
	 */
    std::string mPathFilename;

	/** Index of next point this kart should go to. */
    int mNextPathPointIndex;

	/**
	 *  Initializes the path used by this kart.
	 *  This should be called only after the filename of the path has been set.
	 */
    void initAIPath();

	/**
	 * Utility function for load the path's points.
	 * @param readVal value read from the file.
	 * @param fileRow row of the read value in the file matrix.
	 * @param fileCol col of the read value in the file matrix.
	 */
	void pathArrayFillFunction(int readVal, int fileRow, int fileCol);

    const char *logTag();
    bool canLog();
};

#endif // AI_KART_H
