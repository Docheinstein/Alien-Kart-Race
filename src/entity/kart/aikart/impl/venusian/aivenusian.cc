#include <SFML/Graphics.hpp>
#include "aivenusian.h"
#include "resourceutil.h"
#include "kartfactory.h"
#include "level.h"

AIVenusian::AIVenusian( Level * level, const char *kartName,  sf::Color *kartColor)
                        : AIKart(level, kartName, kartColor) {
    mPathFilename = ResourceUtil::raw("earth_ai_path_1.txt");
    initSprites();
    initParameters();
    initAIPath();
}

sf::Color AIVenusian::minimapColor() {
    return sf::Color::Red;
}

// PRIVATE
void AIVenusian::initSprites() {
    mSpriteCount = KartFactory::sprites(KartFactory::KartType::VenusianType, mSprites);
    mLeaderboardSprite = KartFactory::leadeboardFaceSprite(KartFactory::KartType::VenusianType);
}

void AIVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::VenusianType);
}
