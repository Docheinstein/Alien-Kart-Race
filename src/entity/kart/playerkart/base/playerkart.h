#ifndef PLAYER_KART_H
#define PLAYER_KART_H

#include "kart.h"
#include "mathentities.h"
#include "geometryentities.h"

/** Base entity representing a kart controller by the player. */
class PlayerKart : public Kart {
public:
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

	/**
	 * Returns the point the player kart relies on.
	 * @return the perspective point of the player kart.
	 */
	static Point perspectivePoint();

	/**
	 * Returns the scale of the player kart relative to its perspective point.
	 * @return the scale of the the player kart.
	 */
	static float perspectiveScale();

	/** Initializes the ranges used for the turning render. */
	void initRenderTurningRanges();

	/** Initializes the sprites used for the turning render. */
	void initSprites();

	const char *logTag();
	bool canLog();
};

#endif // PLAYER_KART_H