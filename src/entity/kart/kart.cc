#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include "game.h"
#include "level.h"
#include "map.h"
#include "kart.h"
#include "logger.h"

#define CAN_LOG 0
#define LOG_TAG "{Kart} "

const char * Kart::logTag() {
	return LOG_TAG;
}

bool Kart::canLog() {
	return CAN_LOG;
}

const int MINI_MAP_KART_SIZE = 2;
const int BOUNCE_TICKS = 16;

Kart::Kart() {
	mCol = 0;
	mRow = 0;
	mDirection = (double)1 / (1 << 8);
	mSpeed = 0;
	mWheelTurning = 0;
}

Kart::~Kart() {
	delete [] mSprites;
	delete [] mTextures;
}

double Kart::col() {
	return mCol;
}

double Kart::row() {
	return mRow;
}

double Kart::direction() {
	return mDirection;
}

void Kart::setCol(double col) {
	mCol = col;
}

void Kart::setRow(double row) {
	mRow = row;
}

void Kart::setDirection(double dir) {
	mDirection = dir;
}

void Kart::update(bool goForward, bool goBackward, bool goLeft, bool goRight) {

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
		double previousRow = mRow;
		double previousCol = mCol;

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

		mDirection += angularIncrement;

		advanceInCurrentDirection();

		Map::TileEvent nextTileEvent = Game::instance().level()->map()->getTileEvent(mRow, mCol);

		// if (nextTileEvent == Map::TileEvent::Unpassable) {
		// 	mSpeed = previousSpeed;
		// 	mRow = previousRow;
		// 	mCol = previousCol;
		// 	bounce();
		// }

		mDirection = fmod(mDirection + M_PI * 2, M_PI * 2);

		// Ensure direction is not 0, just in case (not a good number for division and
		// trigonometric function)
		if (mDirection == 0)
			mDirection = 1 / (1 << 8); // Little value > 0

	/*
		std::cout << "COL\t: " << mCol << std::endl;
		std::cout << "ROW\t: " << mRow << std::endl;
		std::cout << "Dir\t: " << mDirection << std::endl << std::endl;
	*/
		// d("Kart [ROW: ", mRow, ", COL: ", mCol, "]");
		// Map *m = Game::instance().level()->map();
		// if (m->getTile(mRow, mCol) == 4) {
		// 	d("Over an event tile");
		// }


		// d("\n");
		// d("Direction: \t", mDirection);
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

void Kart::bounce() {
	if (!mBouncing) {
		mSpeed = (mSpeed < 0 ? 1 : -1 /* opposite of sign */) * mParams.bounceSpeedInitialSpeed;
		mBouncing = true;
		mBounceCurrentTicks = 0;
		d("Will bounce");
	}

	if (mBounceCurrentTicks >= BOUNCE_TICKS) {
		mBouncing = false;
		d("Finished bouncing with speed: ", mSpeed);
		return;
	}

	mBounceCurrentTicks++;

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

bool Kart::isSkidding() {
	return fabs(mWheelTurning) > fabs(mParams.wheelTurningSkidPoint);
}

void Kart::advanceInCurrentDirection() {
	double realSpeed = mSpeed;
	double realDirection = mDirection;

	if (isSkidding()) {
		d2("Is Skidding!");
		double maxDeSkid = mParams.maxWheelTurning - mParams.wheelTurningSkidPoint;
		double deSkid = fabs(mWheelTurning) - mParams.wheelTurningSkidPoint;
		double skidPercentage = deSkid / maxDeSkid;

		double lowestSpeedBoundDueSkid = 0.8;
		double highestSpeedBoundDueSkid = 1.0;

		double maxDeSpeedBoundForSKid = highestSpeedBoundDueSkid - lowestSpeedBoundDueSkid;
		double speedBoundForSkid = lowestSpeedBoundDueSkid + maxDeSpeedBoundForSKid * (1 - skidPercentage);

		realSpeed *= speedBoundForSkid;
		d2("mSpeed", mSpeed);
		d2("maxDeSkid", maxDeSkid);
		d2("deSkid", deSkid);
		d2("skidPercentage", skidPercentage);
		d2("speedBoundForSkid", speedBoundForSkid);
		d2("realSpeed", realSpeed);
	}

	mRow -= cos(realDirection) * realSpeed;
	mCol += sin(realDirection) * realSpeed;
}