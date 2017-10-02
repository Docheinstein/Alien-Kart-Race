#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "game.h"
#include "level.h"
#include "map.h"
#include "kart.h"
#include "logger.h"

#define LOG_TAG "{Kart} "
#define CAN_LOG 1

#define FLIGHT_MODE 0

const double COLLISION_BOUNCE_FACTOR = 0.7;
const double COLLISION_BOUNCE_REDUCTION_FACTOR = (double)1 / 120;

const double COLLISION_CRASH_FACTOR = 0.2;
const double COLLISION_CRASH_REDUCTION_FACTOR = (double)1 / (1 << 9);

const int MINI_MAP_KART_SIZE = 2;

Kart::Kart(Level * level, const char *kartName, sf::Color *kartColor) {
	mLevel = level;
	mName = kartName;
	mColor = kartColor;

	mDirectionalPoint.position = Point {0, 0};
	mDirectionalPoint.direction = Angle {(double)1 / (1 << 5)};
	mSpeed = 0;
	mWheelTurning = 0;
}

Kart::~Kart() {
	delete [] mSprites;
	delete mLeaderboardSprite;
	delete mColor;
}

// PUBLIC

Point Kart::position() {
	return mDirectionalPoint.position;
}

Angle Kart::direction() {
	return mDirectionalPoint.direction;
}

DirectionalPoint Kart::directionalPoint() {
	return mDirectionalPoint;
}

const char * Kart::name() {
	return mName;
}

sf::Color Kart::color() {
	return *mColor;
}

sf::Sprite *Kart::leadeboardSprite() {
	return mLeaderboardSprite;
}

void Kart::setPosition(const Point p) {
	mDirectionalPoint.position = p;
}

void Kart::setDirection(const Angle a) {
	mDirectionalPoint.direction = a;
}

void Kart::update(bool goForward, bool goBackward, bool goLeft, bool goRight) {
#if FLIGHT_MODE
	double turning = 0;
	double speed = 0;

	if (goForward)
		speed = mParams.maxSpeed;
	else if (goBackward)
		speed = -mParams.maxSpeed;

	if (goLeft)
		turning = -0.01;
	else if (goRight)
		turning = 0.01;

	mDirectionalPoint.direction += turning;
	mDirectionalPoint.advance(speed);
#else
	Map::TileEvent nextTileEvent = mLevel->map()->tileEvent(position());
	bool skidding = isSkidding();

	double maxWheelTurning = mParams.maxWheelTurning;
	double maxSpeed = mParams.maxSpeed;
	double minSpeed = mParams.minSpeed;
	double accelerationFactor = mParams.accelerationFactor;
	double backAccelerationFactor = mParams.backwardAccelerationFactor;
	double turningFactor = skidding ?
		mParams.steeringWheelTurningFactorSkidding :
		mParams.steeringWheelTurningFactor;
	double returnFactor = mParams.steeringWheelReturnFactor;

	if (nextTileEvent == Map::TileEvent::Slow) {
		maxWheelTurning *= mParams.slownessFactor;
		maxSpeed *= mParams.slownessFactor;
		minSpeed *= mParams.slownessFactor;
		accelerationFactor *= mParams.slownessFactor;
		backAccelerationFactor *= mParams.slownessFactor;
		turningFactor *= mParams.slownessFactor;
		returnFactor *= mParams.slownessFactor;
	}

	// Left / Right
	// Ensure the wheer angle is under the maximum
	if (goRight) {
		mWheelTurning += turningFactor;
		mWheelTurning = std::min(mWheelTurning, maxWheelTurning);
	}
	else if (goLeft) {
		mWheelTurning -= turningFactor;
		mWheelTurning = std::max(mWheelTurning, -maxWheelTurning);
	}
	else {
		if (mWheelTurning != 0) {
			if (mWheelTurning > 0) {
				mWheelTurning -= returnFactor;
				mWheelTurning = std::max(mWheelTurning, (double) 0);
			}
			else {
				mWheelTurning += returnFactor;
				mWheelTurning = std::min(mWheelTurning, (double) 0);
			}
		}
		// else {
		// 	d("Steering wheel on its natural asset");
		// }
	}

	// Forward / Backward
	double previousSpeed = mSpeed;
	double previousRow = mDirectionalPoint.position.y;
	double previousCol = mDirectionalPoint.position.x;

	if (goForward) {
		// Accellerating

		// Was going faster than max speed (was on the road and is going to the grass)
		if (mSpeed > maxSpeed)
			mSpeed -= mParams.decelerationFactor;
		// Can go faster
		else if (mSpeed + accelerationFactor < maxSpeed)
			mSpeed += accelerationFactor;
	}
	else if (goBackward) {
		// Was going forward => brake
		if (mSpeed > 0) {
			mSpeed -= mParams.brakeFactor;
			mSpeed = std::max((double) 0, mSpeed);
		}
		// Was stationary => go backward
		else {
			// Was going faster than max speed (was on the road and is going to the grass)
			if (mSpeed < minSpeed)
				mSpeed += mParams.decelerationFactor;
			// Can go faster
			else if (mSpeed - backAccelerationFactor > minSpeed)
				mSpeed -= backAccelerationFactor;
		}
	}
	else {
		// Decellerating
		// Was going forward => decrease speed
		if (mSpeed > 0) {
			mSpeed -= mParams.decelerationFactor;
			// Ensure the speed is not below 0
			mSpeed = std::max(mSpeed, (double) 0);
		}
		// Was going backward => increase speed
		else {
			mSpeed += mParams.decelerationFactor;
			// Ensure the speed is not above 0
			mSpeed = std::min(mSpeed, (double) 0);
		}
	}

	double turningRadius = 1 / sin(mWheelTurning);
	double turningPerimeter = 2 * M_PI * turningRadius;
	double angularSpeed = mSpeed / turningPerimeter;
	double angularIncrement = angularSpeed * 2 * M_PI;

	if (skidding)
		angularIncrement *= 0.45;

	mDirectionalPoint += angularIncrement;

	advanceInCurrentDirection();

	if (nextTileEvent == Map::TileEvent::Unpassable) {
		mSpeed = previousSpeed;
		mDirectionalPoint.position.y = previousRow;
		mDirectionalPoint.position.x = previousCol;
		bounce();
	}

	// Ensure direction is not 0, just in case (not a good number for division and
	// trigonometric function)
	if (mDirectionalPoint.direction.rad == 0)
		mDirectionalPoint.direction.rad = 1 / (1 << 8); // Little value > 0

/*
	std::cout << "COL\t: " << mDirectionalPoint.position.x << std::endl;
	std::cout << "ROW\t: " << mDirectionalPoint.position.y << std::endl;
	std::cout << "Dir\t: " << mDirection << std::endl << std::endl;
*/
	// d("Kart [ROW: ", mPosition.y, ", COL: ", mPosition.x, "]");
	// Map *m = Game::instance().level()->map();
	// if (m->getTile(mPosition.y, mPosition.x) == 4) {
	// 	d("Over an event tile");
	// }
#endif

	// d("\n");
	// d("Direction: \t", direction().rad * 180 / M_PI, " deg");
	// d("Speed:\t\t", mSpeed);
	// d("MAX SPEED\t", mMaxSpeed);
	// d("Wheel turning:\t", mWheelTurning);
	// d("turningRadius:\t", turningRadius);
	// d("turningPerimeter:", turningPerimeter);
	// d("angularSpeed:\t", angularSpeed);
	// d("angularIncrement:", angularIncrement);
}

void Kart::update() {

}

void Kart::crash(Kart *k) {
	DirectionalPoint kartsDirPoint(k->position(), position());

	mCollisionForces.push_back(
		CollisionVector {
			Crash,
		 	Vector  {kartsDirPoint.direction, k->mSpeed * k->mParams.weight  *
											COLLISION_CRASH_FACTOR / mParams.weight}
		}
	);
}

void Kart::bounce() {
	if (!isBouncing()) {
		d2("New bounce!");

		DirectionalPoint oppositeDirPoint(position(), Angle {(direction().rad + M_PI) });
		double bounceMagnitude = mSpeed * COLLISION_BOUNCE_FACTOR / mParams.weight;
		Vector oppositeVector {oppositeDirPoint.direction, bounceMagnitude};

		while (mLevel->map()->tileEvent(position()) == Map::TileEvent::Unpassable)
			mDirectionalPoint.advance(oppositeVector);

		mSpeed = 0;
		mCollisionForces.push_back(CollisionVector { Bounce, oppositeVector });
	}
}

bool Kart::isSkidding() {
	return fabs(mWheelTurning) > fabs(mParams.wheelTurningSkidPoint);
}

bool Kart::isBouncing() {
	bool bouncing = false;
    for (std::list<CollisionVector>::iterator i = mCollisionForces.begin();
        i != mCollisionForces.end() && !bouncing;
		i++) {
		bouncing |= ((*i).type == Bounce);
	}
	return bouncing;
}

void Kart::advanceInCurrentDirection() {
	double realSpeed = mSpeed;

	if (isSkidding()) {
		// d2("Is Skidding!");
		double maxDeSkid = mParams.maxWheelTurning - mParams.wheelTurningSkidPoint;
		double deSkid = fabs(mWheelTurning) - mParams.wheelTurningSkidPoint;
		double skidPercentage = deSkid / maxDeSkid;

		double lowestSpeedBoundDueSkid = 0.8;
		double highestSpeedBoundDueSkid = 1.0;

		double maxDeSpeedBoundForSKid = highestSpeedBoundDueSkid - lowestSpeedBoundDueSkid;
		double speedBoundForSkid = lowestSpeedBoundDueSkid + maxDeSpeedBoundForSKid * (1 - skidPercentage);

		realSpeed *= speedBoundForSkid;
		// d2("mSpeed", mSpeed);
		// d2("maxDeSkid", maxDeSkid);
		// d2("deSkid", deSkid);
		// d2("skidPercentage", skidPercentage);
		// d2("speedBoundForSkid", speedBoundForSkid);
		// d2("realSpeed", realSpeed);
	}

	Vector finalForce;
	finalForce.direction = direction();
	finalForce.magnitude = realSpeed;

    for (std::list<CollisionVector>::iterator collisionIter = mCollisionForces.begin();
        collisionIter != mCollisionForces.end();
		/* Do not increment by default */) {

		// d2("Force: ", (*collisionIter).v, " will contribuite to final force");
		finalForce += (*collisionIter).v;

		double forceDecrement = mParams.weight;

		if ((*collisionIter).type == Bounce)
			forceDecrement *= COLLISION_BOUNCE_REDUCTION_FACTOR;
		else if ((*collisionIter).type == Crash)
			forceDecrement *= COLLISION_CRASH_REDUCTION_FACTOR;

		bool sign = (*collisionIter).v.magnitude > 0;
		(*collisionIter).v.magnitude +=
			((*collisionIter).v.magnitude < 0 ? 1 : -1 ) * forceDecrement;
		bool newSign = (*collisionIter).v.magnitude > 0;

		if (sign != newSign || (*collisionIter).v.magnitude == 0) {
			// d2("Erasing force: ", (*collisionIter).v);
			collisionIter = mCollisionForces.erase(collisionIter);
		}
		else {
			collisionIter++;
		}
	}

	mDirectionalPoint.advance(finalForce);
}

const char * Kart::logTag() {
	return LOG_TAG;
}

bool Kart::canLog() {
	return CAN_LOG;
}