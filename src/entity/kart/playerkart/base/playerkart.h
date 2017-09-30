#ifndef PLAYER_KART_H
#define PLAYER_KART_H

#include "kart.h"
#include "mathutil.h"
#include "geometryutil.h"

class PlayerKart : public Kart {
public:
	static const Point PLAYER_KART_PERSPECTIVE_POINT;
	static const float PLAYER_KART_PERSPECTIVE_SCALE;

	PlayerKart(Level * level, const char *kartName, sf::Color *kartColor);
    virtual void update();
    virtual void draw();

    int minimapSize();

protected:
	static const int TURNING_RANGE_COUNT = 11;
    sf::Sprite *mSkidGasSprites;
    int mSkidGasSpriteCount;

	MathUtil::BoundRange mRenderTurningRanges[TURNING_RANGE_COUNT];

	void initRenderTurningRanges();
	void initSprites();
};

#endif // PLAYER_KART_H