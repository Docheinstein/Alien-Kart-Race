#include <SFML/Graphics.hpp>
#include "mapfactory.h"
#include "resourceutil.h"
#include "earth.h"

sf::Texture *  MapFactory::sMapsPickerTextures = pickerTextures();

sf::Sprite * MapFactory::pickerSprite(MapType type) {
    sf::Sprite *pickerSprite = new sf::Sprite();
    pickerSprite->setTexture(MapFactory::sMapsPickerTextures[type]);
    return pickerSprite;
}

const char * MapFactory::name(MapType type) {
    switch (type) {
    case EarthType:
        return "Earth";
    default:
        return "";
    }
}

sf::Texture * MapFactory::pickerTextures() {
    sf::Texture *textures = new sf::Texture[MapType::_MapTypeCount];
    for (int i = 0; i < MapType::_MapTypeCount; i++) {
        textures[i].loadFromFile(
            ResourceUtil::image(spritePickerFilename(
                static_cast<MapType>(i)).c_str()).c_str());
    }
    return textures;
}

std::string MapFactory::spritePickerFilename(MapType type) {
    switch (type) {
    case EarthType: {
        return "earth_level_picker.png";
    }
    default:
        return "";
    }
}

Map * MapFactory::map(sf::RenderWindow *window, Level *level, MapType type) {
    switch (type) {
    case EarthType:
        return new Earth(window, level);
    default:
        return NULL;
    }
}

const char * MapFactory::minimapFilename(MapType type) {
    switch (type) {
    case EarthType: {
        return "earth_minimap.txt";
    }
    default:
        return "";
    }
}