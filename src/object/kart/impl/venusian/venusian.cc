#include <SFML/Graphics.hpp>
#include "resourceutil.h"
#include "venusian.h"
#include "game.h"
#include "log.h"

Venusian::Venusian() {
    initSprite();
    initParameters();
}

// PRIVATE
void Venusian::initSprite() {
    const int SPRITE_COUNT = 3;
	mTextures = new sf::Texture[SPRITE_COUNT];
	mSprites = new sf::Sprite[SPRITE_COUNT];

	mTextures[0].loadFromFile(ResourceUtil::image("venusian_sx.png"));
    mTextures[1].loadFromFile(ResourceUtil::image("venusian_center.png"));
    mTextures[2].loadFromFile(ResourceUtil::image("venusian_dx.png"));

    for (int i = 0; i < SPRITE_COUNT; i ++) {
        const float SCALE_FACTOR = 1.2;
        mSprites[i].setTexture(mTextures[i]);
        mSprites[i].setScale(SCALE_FACTOR, SCALE_FACTOR);
        mSprites[i].setPosition(
            (Game::WINDOW_WIDTH - mSprites[i].getGlobalBounds().width) / 2,
            Game::WINDOW_HEIGHT - MARGIN_FROM_BOTTOM -mSprites[i].getGlobalBounds().height);

    }
}

void Venusian::initParameters() {
    mMaxSpeed = 0.12;
    mMinSpeed = -0.05;

    mMaxWheelTurning = 0.5;

	mAccelerationFactor = (double) 1 / (double) (1 << 11);

    mBackwardAccelerationFactor = (double) 1 / (double) (1 << 9);

    mDecelerationFactor = mAccelerationFactor * 3;

    mBrakeFactor = mAccelerationFactor * 5;

    mSteeringWheelTurningFactor = 0.05;
	mSteeringWheelReturnFactor = 0.04;

    mWheelTurning = 0.05;

	mSpeed = 0;
}
