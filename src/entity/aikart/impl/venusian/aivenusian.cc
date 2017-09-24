#include <SFML/Graphics.hpp>
#include "aivenusian.h"
#include "resourceutil.h"
#include "kartfactory.h"

AIVenusian::AIVenusian() {
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
    mSpriteCount = KartFactory::sprites(KartFactory::KartType::Venusian, mSprites);
}

void AIVenusian::initParameters() {
    mParams = KartFactory::params(KartFactory::KartType::Venusian);
}
