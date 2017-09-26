#ifndef AI_KART_H
#define AI_KART_H

#include "kart.h"

namespace sf {
	// class RenderWindow;
	class Texture;
	class Sprite;
	class Color;
}

class Point;

class AIKart : public Kart {
public:

	AIKart(const char *kartName);
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
