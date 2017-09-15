#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include "game.h"
#include "level.h"
#include "map.h"
#include "kart.h"
#include "log.h"

const int MINI_MAP_KART_SIZE = 2;

Kart::Kart() {
	d("initKart()");
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

void Kart::draw() {
	if (mWheelTurning < 0) {
		Game::instance().window()->draw(mSprites[0]);
	}
	else if (mWheelTurning == 0) {
		Game::instance().window()->draw(mSprites[1]);
	}
	else {
		Game::instance().window()->draw(mSprites[2]);
	}

}

void Kart::drawOnMiniMap() {
	const int MINI_MAP_KART_SIZE = 2;

	sf::CircleShape miniKartCircle(MINI_MAP_KART_SIZE);
	miniKartCircle.setFillColor(sf::Color(100, 250, 50));
	Map *m = Game::instance().level()->map();
	miniKartCircle.setPosition(
		//Map::MINI_MAP_X + m->colCount() / (double) Map::MINI_MAP_WIDTH * mCol,
		//Map::MINI_MAP_Y + m->rowCount() / (double) Map::MINI_MAP_WIDTH * mRow
		Map::MINI_MAP_X + (double) Map::MINI_MAP_WIDTH / m->colCount() * mCol
			- MINI_MAP_KART_SIZE / 2,
		Map::MINI_MAP_Y + (double) Map::MINI_MAP_HEIGHT / m->rowCount() * mRow
			- MINI_MAP_KART_SIZE / 2
	);
	Game::instance().window()->draw(miniKartCircle);
}

void Kart::update() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		// Accellerating
		mSpeed += mAccelerationFactor;

		mSpeed = std::min(mMaxSpeed, mSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		// Was going forward => brake
		if (mSpeed > 0) {
			mSpeed -= mBrakeFactor;
			mSpeed = std::max((double) 0, mSpeed);
		}
		// Was stationary => go backward
		else {
			mSpeed -= mBackwardAccelerationFactor;
			mSpeed = std::max(mMinSpeed, mSpeed);
		}

	}
	else {
		// Decellerating
		// Was going forward => decrease speed
		if (mSpeed > 0) {
			mSpeed -= mDecelerationFactor;
			// Ensure the speed is not below 0
			mSpeed = std::max(mSpeed, (double) 0);
		}
		// Was going backward => increase speed
		else {
			mSpeed += mDecelerationFactor;
			// Ensure the speed is not above 0
			mSpeed = std::min(mSpeed, (double) 0);
		}
	}

	double turningRadius = 1 / sin(-mWheelTurning);
	double turningPerimeter = 2 * M_PI * turningRadius;
	double angularSpeed = mSpeed / turningPerimeter;
	double angularIncrement = angularSpeed * 2 * M_PI;

	// Left / Right
	// Ensure the wheer angle is under the maximum

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		mWheelTurning += mSteeringWheelTurningFactor;
		mWheelTurning = std::min(mWheelTurning, mMaxWheelTurning);

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		mWheelTurning -= mSteeringWheelTurningFactor;
		mWheelTurning = std::max(mWheelTurning, -mMaxWheelTurning);
	}
	else {
		if (mWheelTurning != 0) {
			if (mWheelTurning > 0) {
				mWheelTurning -= mSteeringWheelReturnFactor;
				mWheelTurning = std::max(mWheelTurning, (double) 0);
			}
			else {
				mWheelTurning += mSteeringWheelReturnFactor;
				mWheelTurning = std::min(mWheelTurning, (double) 0);
			}
		}
		// else {
		// 	d("Steering wheel on its natural asset");
		// }
	}

	mDirection += angularIncrement;


	mRow -= cos(-mDirection) * mSpeed;
	mCol += sin(-mDirection) * mSpeed;

	mDirection = fmod(mDirection + M_PI * 2, M_PI * 2);

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
