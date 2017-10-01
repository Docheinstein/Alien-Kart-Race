#ifndef PLAYER_KART_H
#define PLAYER_KART_H

#include "kart.h"
#include "mathentities.h"
#include "geometryentities.h"

/** Base entity representing a kart controller by the player. */
class PlayerKart : public Kart {
public:
	/**
	  * Point on the screen in which the player kart is
	  * rendered (depends on the camera point).
	 */
	static const Point PERSPECTIVE_POINT;

	/** Scale of the player kart in relation to its perspective point. */
	static const float PERSPECTIVE_SCALE;

	PlayerKart(Level * level, const char *kartName, sf::Color *kartColor);

    virtual void update();
    virtual void draw();

    int minimapSize();

protected:
	/**
	 * Number of range in which the turning is divided (this also represents
	 * the number of sprite used for the turning animation).
	 */
	static const int TURNING_RANGE_COUNT = 11;

	/** Sprites used for the skid effect. */
    sf::Sprite *mSkidGasSprites;

	/** Number of sprite used for the skid effect */
    int mSkidGasSpriteCount;

	/** Ranges representing the different sprite shown when turning. */
	BoundRange mRenderTurningRanges[TURNING_RANGE_COUNT];

	/** Initializes the ranges used for the turning render. */
	void initRenderTurningRanges();

	/** Initializes the sprites used for the turning render. */
	void initSprites();
};

#endif // PLAYER_KART_H