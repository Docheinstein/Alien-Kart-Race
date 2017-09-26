#include <SFML/Graphics.hpp>
#include "fontfactory.h"
#include "resourceutil.h"

sf::Font * FontFactory::sFonts = fonts();

sf::Font &FontFactory::font(FontType type) {
    return sFonts[type];
}

sf::Font *FontFactory::fonts() {
    sf::Font *fonts = new sf::Font[_FontTypeCount];
    fonts[Main].loadFromFile(ResourceUtil::font("grobold.ttf"));
    return fonts;
}