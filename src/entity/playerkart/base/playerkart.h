#ifndef PLAYER_KART_H
#define PLAYER_KART_H

#include "game.h"
#include "kart.h"
#include "mathutil.h"
#include "geometryutil.h"

class PlayerKart : public Kart {
public:
	static const Point PLAYER_KART_PERSPECTIVE_POINT;
	static const float PLAYER_KART_PERSPECTIVE_SCALE;
	static const int TURNING_RANGE_COUNT = 7;

    virtual void update();
    virtual void draw();

    int minimapSize();

protected:

    sf::Sprite *mSkidGasSprites;
    int mSkidGasSpriteCount;

	MathUtil::BoundRange mRenderTurningRanges[TURNING_RANGE_COUNT];

	void initRenderTurningRanges();
	void initSprites();
};

#endif // PLAYER_KART_H