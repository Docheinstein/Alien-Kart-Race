#include <SFML/Graphics.hpp>
#include "fontfactory.h"
#include "resourceutil.h"

#define FONT_NAME_PLAIN "comfortaa.ttf"
#define FONT_NAME_SEMIBOLD "cocogoose.ttf"
#define FONT_NAME_FANCY "grobold.ttf"

sf::Font * FontFactory::sFonts = fonts();

sf::Font &FontFactory::font(FontType type) {
    return sFonts[type];
}

sf::Font *FontFactory::fonts() {
    sf::Font *fonts = new sf::Font[_FontTypeCount];
    fonts[Plain].loadFromFile(ResourceUtil::font(FONT_NAME_PLAIN).c_str());
    fonts[Semibold].loadFromFile(ResourceUtil::font(FONT_NAME_SEMIBOLD).c_str());
    fonts[Fancy].loadFromFile(ResourceUtil::font(FONT_NAME_FANCY).c_str());
    return fonts;
}