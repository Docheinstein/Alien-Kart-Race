#ifndef KART_H
#define KART_H

#include "mapentity.h"
#include "loggerinterface.h"

namespace sf {
	class RenderWindow;
	class Texture;
	class Sprite;
	class Color;
}

class Kart : public MapEntity, public LoggerInterface {
public:

    struct KartParams {
		double maxSpeed; // Tile / cycle

		// Minimum speed going backward (negative number)
		double minSpeed; // Tile / cycle

		// Maximum angle of the steering wheel.
		double maxWheelTurning; // Radians

		double accelerationFactor; // Tile / cycle^2
		double decelerationFactor; // Tile / cycle^2

		double backwardAccelerationFactor; // Tile / cycle^2

		double brakeFactor; // Tile / cycle^2

		// Factor of increment for the steering wheel to reach its maximum turn.
		double steeringWheelTurningFactor;

		// Factor of increment for the steering wheel to reach its maximum turn while skidding.
		double steeringWheelTurningFactorSkidding;

		// Factor of decrement for the steering wheel to return to its natural position.
		// Since when no direction is chosen by the user, the steering wheel
		// take some time to return to its natural position.
		double steeringWheelReturnFactor;

		// At which percentage the cars begins skidding (loses control)
		double wheelTurningSkidPoint;


		// The speed that will be taken by the car when bounce.
		// (Must be positive).
		double bounceSpeedInitialSpeed;

		// How fast does the bounce end.
		double bounceDecellerationFactor;
    };

	Kart();
	~Kart();

	double col();
	double row();
	double direction();

	void setCol(double mapCol);
	void setRow(double mapRow);
	void setDirection(double dir);
	void update(bool goForward, bool goBackward, bool goLeft, bool goRight);

	virtual void update() = 0;
	virtual void draw() = 0;

	virtual sf::Color minimapColor() = 0;
	virtual int minimapSize() = 0;

protected:
    const char *logTag();
    bool canLog();

	void bounce();

	bool isSkidding();
	void advanceInCurrentDirection();

	KartParams mParams;

	// Current angle of the steering wheel.
	double mWheelTurning; // Radians

	// Current angle of the car.
	double mDirection; // Radians

	// Current speed.
	double mSpeed; // Tile / cycle

	bool mBouncing = false;
	// Do with timer...
	int mBounceCurrentTicks;

	sf::Sprite *mSprites;
	sf::Texture *mTextures;
};

#endif // KART_H