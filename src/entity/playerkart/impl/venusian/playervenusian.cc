#include <SFML/Graphics.hpp>
#include "resourceutil.h"
#include "playervenusian.h"
#include "game.h"
#include "minimap.h"
#include "kartparamsfactory.h"

PlayerVenusian::PlayerVenusian() {
    initSprites();
    initParameters();
}

sf::Color PlayerVenusian::minimapColor() {
    return sf::Color::Green;
}

// PRIVATE
void PlayerVenusian::initSprites() {
    const int GAS_SPRITE_COUNT = _GasSpriteTypeCount;
    const int SPRITE_COUNT = _PlayerKartSpriteTypeCount;
    const float SCALE_FACTOR = 1.1 * Game::WINDOW_WIDTH / 320;

	mTextures = new sf::Texture[SPRITE_COUNT];
	mSprites = new sf::Sprite[SPRITE_COUNT];

	mTextures[Left3].loadFromFile(ResourceUtil::image("venusian_left_3.png"));
	mTextures[Left2].loadFromFile(ResourceUtil::image("venusian_left_2.png"));
	mTextures[Left1].loadFromFile(ResourceUtil::image("venusian_left_1.png"));
	mTextures[Center].loadFromFile(ResourceUtil::image("venusian_center.png"));
	mTextures[Right1].loadFromFile(ResourceUtil::image("venusian_right_1.png"));
	mTextures[Right2].loadFromFile(ResourceUtil::image("venusian_right_2.png"));
	mTextures[Right3].loadFromFile(ResourceUtil::image("venusian_right_3.png"));

    mGasTex[GasLeft].loadFromFile(ResourceUtil::image("venusian_skid_left_gas.png"));
    mGasTex[GasRight].loadFromFile(ResourceUtil::image("venusian_skid_right_gas.png"));

    for (int i = 0; i < SPRITE_COUNT; i ++) {

        mSprites[i].setTexture(mTextures[i]);
        mSprites[i].setScale(SCALE_FACTOR, SCALE_FACTOR);
        mSprites[i].setPosition(
            (Game::WINDOW_WIDTH - mSprites[i].getGlobalBounds().width) / 2,
            Game::WINDOW_HEIGHT - PlayerKart::MARGIN_FROM_BOTTOM - mSprites[i].getGlobalBounds().height);

    }

    for (int i = 0; i < GAS_SPRITE_COUNT; i ++) {

        mGasSprite[i].setTexture(mGasTex[i]);
        mGasSprite[i].setScale(SCALE_FACTOR, SCALE_FACTOR);
        mGasSprite[i].setPosition(
            (Game::WINDOW_WIDTH - mGasSprite[i].getGlobalBounds().width) / 2,
            Game::WINDOW_HEIGHT - MARGIN_FROM_BOTTOM - 6);

    }
}

void PlayerVenusian::draw() {
    PlayerKart::draw();
    if (isSkidding()) {
        GasSpriteType spriteType;
        if (mWheelTurning < 0)
            spriteType = GasLeft;
        else
            spriteType = GasRight;
    	Game::instance().window()->draw(mGasSprite[spriteType]);
    }
}

void PlayerVenusian::initParameters() {
    mParams = KartParamsFactory::params(KartParamsFactory::KartType::Venusian);
}
