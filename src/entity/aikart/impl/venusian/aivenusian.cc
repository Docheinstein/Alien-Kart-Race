#include <SFML/Graphics.hpp>
#include "aivenusian.h"
#include "resourceutil.h"
#include "kartparamsfactory.h"

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

}

void AIVenusian::initParameters() {
    mParams = KartParamsFactory::params(KartParamsFactory::KartType::Venusian);
}
