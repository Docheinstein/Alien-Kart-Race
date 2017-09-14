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
	static const int MARGIN_FROM_BOTTOM = 20;

	Kart();
	~Kart();

	double col();
	double row();
	double direction();

	void setCol(double mapCol);
	void setRow(double mapRow);
	void setDirection(double dir);

	void update();
	void draw();
	void drawOnMiniMap();

protected:
	double mMaxSpeed; // Tile / cycle

	// Minimum speed going backward.
	double mMinSpeed; // Tile / cycle

	// Maximum angle of the steering wheel.
	double mMaxWheelTurning; // Radians

	double mAccelerationFactor; // Tile / cycle^2
	double mDecelerationFactor; // Tile / cycle^2

	double mBackwardAccelerationFactor; // Tile / cycle^2

	double mBrakeFactor; // Tile / cycle^2

	// Factor of increment for the steering wheel to reach its maximum turn.
	double mSteeringWheelTurningFactor;

	// Factor of decrement for the steering wheel to return to its natural position.
	// Since when no direction is chosen by the user, the steering wheel
	// take some time to return to its natural position.
	double mSteeringWheelReturnFactor;

	// Current values

	// Current angle of the steering wheel.
	double mWheelTurning; // Radians

	// Current angle of the car.
	double mDirection; // Radians

	// Current speed.
	double mSpeed; // Tile / cycle

	sf::Sprite *mSprites;
	sf::Texture *mTextures;
};

#endif // KART_H
