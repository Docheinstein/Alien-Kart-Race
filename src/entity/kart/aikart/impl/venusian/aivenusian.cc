#include <SFML/Graphics.hpp>
#include "aivenusian.h"
#include "resourceutil.h"
#include "mathutil.h"
#include "kartfactory.h"
#include "level.h"

#define PARAM_RANDOMNESS_FACTOR (1.0 / 20)

AIVenusian::AIVenusian( Level * level, const char *kartName,
                        sf::Color *kartColor, const char *pathName)
                        : AIKart(level, kartName, kartColor, pathName) {
    initSprites();
    initParameters();
    initAIPath();
}

// PRIVATE
void AIVenusian::initSprites() {
    mSpriteCount = KartFactory::sprites(KartFactory::KartType::VenusianType, mSprites);
    mLeaderboardSprite = KartFactory::leadeboardFaceSprite(KartFactory::KartType::VenusianType);
}

void AIVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::VenusianType, PARAM_RANDOMNESS_FACTOR);
    // mParams.maxSpeed /=2;
}