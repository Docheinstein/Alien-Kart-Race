#include <SFML/Graphics.hpp>
#include "resourceutil.h"
#include "playervenusian.h"
#include "level.h"
#include "game.h"
#include "minimap.h"
#include "kartfactory.h"

PlayerVenusian::PlayerVenusian() {
    initSprites();
    initParameters();
}

sf::Color PlayerVenusian::minimapColor() {
    return sf::Color::Green;
}

// PRIVATE
void PlayerVenusian::initSprites() {
    mSpriteCount = KartFactory::sprites(KartFactory::KartType::Venusian, mSprites);
    mSkidGasSpriteCount = KartFactory::skidGasSprites(KartFactory::KartType::Venusian, mSkidGasSprites);

    float kartScaleFactor = KartFactory::scaleFactor(KartFactory::KartType::Venusian);
    float scaleFactor  = PLAYER_KART_PERSPECTIVE_SCALE * kartScaleFactor;
    d("Venusian scale factor:", scaleFactor);
    for (int i = 0; i < mSpriteCount; i ++)
        mSprites[i].setScale(scaleFactor, scaleFactor);

    for (int i = 0; i < mSkidGasSpriteCount; i ++)
        mSkidGasSprites[i].setScale(scaleFactor, scaleFactor);

    PlayerKart::initSprites();
}

void PlayerVenusian::draw() {
    PlayerKart::draw();
    if (isSkidding()) {
        int spriteType =  (mWheelTurning < 0 ? 0 : 1);
    	//Game::instance().window()->draw(mSkidGasSprites[spriteType]);
        Game::instance().level()->pushSprite(&mSkidGasSprites[spriteType]);
    }
}

void PlayerVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::Venusian);
    initRenderTurningRanges();
}
