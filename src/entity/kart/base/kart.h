#ifndef KART_H
#define KART_H

#include <list>
#include "mapmovingentity.h"
#include "loggerinterface.h"
#include "gametimer.h"

namespace sf {
	class Texture;
	class Sprite;
	class Color;
}

class Level;

class Kart : public MapMovingEntity, public LoggerInterface {
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


		double weight;
    };

	Kart(Level * level, const char *kartName, sf::Color *kartColor);
	virtual ~Kart();

	Point position();
	Angle direction();
	DirectionalPoint directionalPoint();
	const char *name();
	sf::Color color();
	sf::Sprite * leadeboardSprite();

	void setPosition(Point p);
	void setDirection(Angle a);
	void update(bool goForward, bool goBackward, bool goLeft, bool goRight);

	void crash(Kart *k);

	void bounceTimeFinished();

	virtual void update() = 0;
	virtual void draw() = 0;


	virtual int minimapSize() = 0;

protected:
    const char *logTag();
    bool canLog();

	void bounce();

	bool isSkidding();
	void advanceInCurrentDirection();

	Level *mLevel;

	DirectionalPoint mDirectionalPoint;

	KartParams mParams;

	sf::Sprite *mSprites;
	int mSpriteCount;

	sf::Sprite *mLeaderboardSprite;

	// Current angle of the steering wheel.
	double mWheelTurning; // Radians

	// Current speed.
	double mSpeed; // Tile / cycle

	bool mBouncing = false;

	bool mCrashing = false;

	const char *mName;

	sf::Color *mColor;

	GameTimer<Kart> mBounceTimer;

	struct KartCrashVector {
		Kart *k;
		Vector v;
	} typedef KartCrashVector;

	std::list<KartCrashVector> mCrashForces;
};

#endif // KART_H
