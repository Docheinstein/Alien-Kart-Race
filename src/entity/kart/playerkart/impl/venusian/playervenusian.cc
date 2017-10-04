#include <SFML/Graphics.hpp>
#include "playervenusian.h"
#include "kartfactory.h"
#include "level.h"

PlayerVenusian::PlayerVenusian(Level *level, const char *kartName, sf::Color *kartColor)
    : PlayerKart(level,kartName, kartColor) {
    initSprites();
    initParameters();
    initRenderTurningRanges();
}

// ------------------------
// PUBLIC -----------------
// ------------------------

void PlayerVenusian::draw() {
    PlayerKart::draw();
    if (isSkidding()) {
        // Renders the skid effect below the kart
        int spriteType =  (mWheelTurning < 0 ? 0 : 1);
        mLevel->pushSprite(&mSkidGasSprites[spriteType]);
    }
}

// ------------------------
// PROTECTED --------------
// ------------------------

void PlayerVenusian::initSprites() {
    mSpriteCount = KartFactory::sprites(
        KartFactory::KartType::VenusianType, mSprites);
    mLeaderboardSprite = KartFactory::leadeboardFaceSprite(
        KartFactory::KartType::VenusianType);
    mSkidGasSpriteCount = KartFactory::skidGasSprites(
        KartFactory::KartType::VenusianType, mSkidGasSprites);

    float kartScaleFactor = KartFactory::scaleFactor();
    float scaleFactor  = PERSPECTIVE_SCALE * kartScaleFactor;
    for (int i = 0; i < mSpriteCount; i ++)
        mSprites[i].setScale(scaleFactor, scaleFactor);

    for (int i = 0; i < mSkidGasSpriteCount; i ++)
        mSkidGasSprites[i].setScale(scaleFactor, scaleFactor);

    PlayerKart::initSprites();
}

void PlayerVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::VenusianType);
}
