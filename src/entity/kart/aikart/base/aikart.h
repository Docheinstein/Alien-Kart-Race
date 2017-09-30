#ifndef AI_KART_H
#define AI_KART_H

#include "kart.h"

namespace sf {
	class Texture;
	class Sprite;
	class Color;
}

class Point;
class Level;

class AIKart : public Kart {
public:

	AIKart(Level *level, const char *kartName, sf::Color *kartColor);
	virtual ~AIKart();

    virtual void update();
    virtual void draw();

    int minimapSize();
protected:

    struct AIPath {
        Point *points;
        int pointCount;
    } typedef AIPath;

    AIPath mPath;
    std::string mPathFilename;
    int mNextPathPointIndex;

    const char *logTag();
    bool canLog();

	void pathArrayFillFunction(int readVal, int fileRow, int fileCol);
    void initAIPath();
};

#endif // AI_KART_H
