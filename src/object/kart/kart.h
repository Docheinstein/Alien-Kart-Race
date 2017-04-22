#ifndef KART_H
#define KART_H

#include "mapobject.h"

namespace sf {
	class RenderWindow;
}

class Kart : public MapObject {
public:
	Kart();

	int col();
	int row();

	void setCol(double mapCol);
	void setRow(double mapRow);

	void update();
	void drawOnMiniMap();

private:
	double mDirection;
};

#endif // KART_H
