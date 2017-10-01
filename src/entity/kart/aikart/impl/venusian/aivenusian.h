#ifndef AI_VENUSIAN_H
#define AI_VENUSIAN_H

#include "aikart.h"

/** Implementation of an AI kart of type Venusian. */
class AIVenusian : public AIKart {
public:
	AIVenusian(	Level * level, const char *kartName,
				sf::Color *kartColor, const char *pathName);

protected:
	/** Initializes the kart's parameters. */
	void initParameters();

	/** Initializes the kart's sprites. */
	void initSprites();
};

#endif // AI_VENUSIAN_H
