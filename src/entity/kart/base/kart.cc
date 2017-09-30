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

const double CRASH_REDUCTION_FACTOR = (double)1 / (1 << 9);
const double CRASH_FACTOR = 0.2;


const int MINI_MAP_KART_SIZE = 2;
const int BOUNCE_TIME_MS = 250;

Kart::Kart(Level * level, const char *kartName, sf::Color *kartColor) {
	mLevel = level;
	mName = kartName;
	mColor = kartColor;

	mDirectionalPoint.position = Point {0, 0};
	mDirectionalPoint.direction = Angle {(double)1 / (1 << 5)};
	mSpeed = 0;
	mWheelTurning = 0;

	void (Kart::*eventFunc)() = &Kart::bounceTimeFinished;
	mBounceTimer.initialize(BOUNCE_TIME_MS, this, eventFunc);
}

Kart::~Kart() {
	delete [] mSprites;
	delete mLeaderboardSprite;
	delete mColor;
}

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
	mDirectionalPoint.position = Point(p);
}

void Kart::setDirection(const Angle a) {
	mDirectionalPoint.direction = Angle(a);
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
	if (mBouncing) {
		bounce();
		return;
	}

	// Left / Right
	// Ensure the wheer angle is under the maximum

	double turningFactor = mParams.steeringWheelTurningFactor;
	bool skidding = isSkidding();

	turningFactor = skidding ?
		mParams.steeringWheelTurningFactorSkidding :
		mParams.steeringWheelTurningFactor;

	if (goRight) {
		mWheelTurning += turningFactor;
		mWheelTurning = std::min(mWheelTurning, mParams.maxWheelTurning);

	}
	else if (goLeft) {
		mWheelTurning -= turningFactor;
		mWheelTurning = std::max(mWheelTurning, -mParams.maxWheelTurning);
	}
	else {
		if (mWheelTurning != 0) {
			if (mWheelTurning > 0) {
				mWheelTurning -= mParams.steeringWheelReturnFactor;
				mWheelTurning = std::max(mWheelTurning, (double) 0);
			}
			else {
				mWheelTurning += mParams.steeringWheelReturnFactor;
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
		mSpeed += mParams.accelerationFactor;

		mSpeed = std::min(mParams.maxSpeed, mSpeed);
	}
	else if (goBackward) {
		// Was going forward => brake
		if (mSpeed > 0) {
			mSpeed -= mParams.brakeFactor;
			mSpeed = std::max((double) 0, mSpeed);
		}
		// Was stationary => go backward
		else {
			mSpeed -= mParams.backwardAccelerationFactor;
			mSpeed = std::max(mParams.minSpeed, mSpeed);
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

	Map::TileEvent nextTileEvent = mLevel->map()->tileEvent(position());

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

	bool notCrashingWithThatKart = true;

    for (std::list<KartCrashVector>::iterator i = mCrashForces.begin();
        i != mCrashForces.end() && notCrashingWithThatKart;
		i++) {
		notCrashingWithThatKart &= ((*i).k != k);
	}

	if (notCrashingWithThatKart)

		mCrashForces.push_back(
			KartCrashVector {
				k,
			 	Vector  {k->direction(), k->mSpeed * k->mParams.weight * CRASH_FACTOR / mParams.weight}

			}
		);

		mCrashForces.push_back(
			KartCrashVector {
				k,
			 	Vector  {kartsDirPoint.direction, k->mSpeed * k->mParams.weight  * CRASH_FACTOR / mParams.weight}
			}
		);
}

void Kart::bounce() {
	if (!mBouncing) {
		mSpeed = (mSpeed < 0 ? 1 : -1 /* opposite of sign */) * mParams.bounceSpeedInitialSpeed;
		mBouncing = true;
		mBounceTimer.reset();
		d("Will bounce");
	}

	mBounceTimer.update();

	if (mSpeed > 0) {
		mSpeed -= mParams.bounceDecellerationFactor;
		// Ensure the speed is not below 0
		mSpeed = std::max(mSpeed, (double) 0);
	}
	else {
		mSpeed += mParams.bounceDecellerationFactor;
		// Ensure the speed is not above 0
		mSpeed = std::min(mSpeed, (double) 0);
	}

	if (mSpeed == 0) {
		mBouncing = false;
		d("Finished bouncing due null speed");
		return;
	}

	d("Go forward for bounce");
	advanceInCurrentDirection();

}

void Kart::bounceTimeFinished() {
	d("Finished bouncing with speed: ", mSpeed);
	mBouncing = false;
}

bool Kart::isSkidding() {
	return fabs(mWheelTurning) > fabs(mParams.wheelTurningSkidPoint);
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

    for (std::list<KartCrashVector>::iterator crashIter = mCrashForces.begin();
        crashIter != mCrashForces.end();
		/* Do not increment by default */) {

		d2("Force: ", (*crashIter).v, " will contribuite to final force");
		finalForce += (*crashIter).v;

		double forceDecrement = mParams.weight * CRASH_REDUCTION_FACTOR;

		bool sign = (*crashIter).v.magnitude > 0;
		(*crashIter).v.magnitude +=
			((*crashIter).v.magnitude < 0 ? 1 : -1 ) * forceDecrement;
		bool newSign = (*crashIter).v.magnitude > 0;

		if (sign != newSign || (*crashIter).v.magnitude == 0) {
			d2("Erasing force: ", (*crashIter).v);
			crashIter = mCrashForces.erase(crashIter);
		}
		else {
			crashIter++;
		}
	}

	// if (mExtraForces.empty())
	// 	mCrashing = false;

	mDirectionalPoint.advance(finalForce);
}

const char * Kart::logTag() {
	return LOG_TAG;
}

bool Kart::canLog() {
	return CAN_LOG;
}