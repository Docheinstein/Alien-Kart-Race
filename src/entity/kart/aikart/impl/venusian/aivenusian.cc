#include <SFML/Graphics.hpp>
#include "aivenusian.h"
#include "kartfactory.h"

// The higher this value, the higher the possibility of get kart with random parameters
// (which may lead to have more fun, or undefined behaviour).
#define PARAM_RANDOMNESS_FACTOR (1.0 / 20)

AIVenusian::AIVenusian( Level * level, const char *kartName,
                        sf::Color *kartColor, const char *pathName)
                        : AIKart(level, kartName, kartColor, pathName) {
    initSprites();
    initParameters();
    initAIPath();
}

// ------------------------
// PROTECTED --------------
// ------------------------

void AIVenusian::initSprites() {
    mSpriteCount = KartFactory::sprites(KartFactory::KartType::VenusianType, mSprites);
    mLeaderboardSprite = KartFactory::leadeboardFaceSprite(KartFactory::KartType::VenusianType);
}

void AIVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::VenusianType, PARAM_RANDOMNESS_FACTOR);
}