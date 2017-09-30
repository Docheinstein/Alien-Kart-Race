#ifndef MAP_FACTORY_H
#define MAP_FACTORY_H

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

class Level;
class Map;

class MapFactory {
public:
    enum MapType {
        EarthType,
        _MapTypeCount
    };

    static sf::Sprite *pickerSprite(MapType type);
    static const char *name(MapType type);
    static Map * map(sf::RenderWindow *window, Level *level, MapType type);
    static const char * minimapFilename(MapType type);

private:
    static sf::Texture *sMapsPickerTextures;

    static sf::Texture *pickerTextures();

    static std::string spritePickerFilename(MapType type);
};

#endif // MAP_FACTORY_H