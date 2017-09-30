#ifndef AI_VENUSIAN_H
#define AI_VENUSIAN_H

#include "aikart.h"

class AIVenusian : public AIKart {
public:

	AIVenusian(	Level * level, const char *kartName, sf::Color *kartColor);

	sf::Color minimapColor();

protected:
	void initParameters();
	void initSprites();
};

#endif // AI_VENUSIAN_H
