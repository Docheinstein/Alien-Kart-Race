#include <SFML/Graphics.hpp>
#include "resourceutil.h"
#include "playervenusian.h"
#include "level.h"
#include "game.h"
#include "minimap.h"
#include "kartfactory.h"

PlayerVenusian::PlayerVenusian(Level *level, const char *kartName, sf::Color *kartColor)
    : PlayerKart(level,kartName, kartColor) {
    initSprites();
    initParameters();
}

// PRIVATE
void PlayerVenusian::initSprites() {
    mSpriteCount = KartFactory::sprites(KartFactory::KartType::VenusianType, mSprites);
    mLeaderboardSprite = KartFactory::leadeboardFaceSprite(KartFactory::KartType::VenusianType);
    mSkidGasSpriteCount = KartFactory::skidGasSprites(KartFactory::KartType::VenusianType, mSkidGasSprites);

    float kartScaleFactor = KartFactory::scaleFactor();
    float scaleFactor  = PERSPECTIVE_SCALE * kartScaleFactor;
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
        mLevel->pushSprite(&mSkidGasSprites[spriteType]);
    }
}

void PlayerVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::VenusianType);
    initRenderTurningRanges();
}
