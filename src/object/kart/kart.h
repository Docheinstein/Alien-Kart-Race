#ifndef KART_H
#define KART_H

#include "mapobject.h"

namespace sf {
	class RenderWindow;
	class Texture;
	class Sprite;
}

class Kart : public MapObject {
public:
	Kart();
	~Kart();
	double col();
	double row();
	double direction();

	void setCol(double mapCol);
	void setRow(double mapRow);

	void update();
	void draw();
	void drawOnMiniMap();

private:
	double mDirection;
	void initSprite();
	sf::Sprite *mSprites;
	sf::Texture *mTextures;
};

#endif // KART_H
