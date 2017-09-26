#ifndef AI_VENUSIAN_H
#define AI_VENUSIAN_H

#include "aikart.h"

namespace sf {
	// class RenderWindow;
	// class Texture;
	// class Sprite;
	// class Color;
}

class AIVenusian : public AIKart {
public:

	AIVenusian(const char *kartName);


	// double col();
	// double row();
	// double direction();
    //
	// void setCol(double mapCol);
	// void setRow(double mapRow);
	// void setDirection(double dir);
	// void update();
	// virtual void draw();

	sf::Color minimapColor();

protected:
	void initParameters();
	void initSprites();
};

#endif // AI_VENUSIAN_H
