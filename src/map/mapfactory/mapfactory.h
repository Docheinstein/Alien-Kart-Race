#ifndef MAP_FACTORY_H
#define MAP_FACTORY_H

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

class Level;
class Map;

/**
 * Factory class usable for create and retrieve information
 * about concrete types of maps.
 */
class MapFactory {
public:
    /** Type of map. */
    enum MapType {
        /** Earth. */
        EarthType,

        /** Number of maps. */
        _MapTypeCount
    };

    /**
     * Returns the sprite to used for the level picker for the given map type.
     * @param  type the type of the map.
     * @return      the sprite to use for the level picker.
     */
    static sf::Sprite *pickerSprite(MapType type);

    /**
     * Returns the name associated with the given map type.
     * This actually matches the string representation of the type.
     * @param  type the name associated with the map type.
     * @return      the name of type.
     */
    static const char *name(MapType type);

    /**
     * Creates and returns a map of the given map type.
     * @param  window the window the new map will draw to.
     * @param  level  the level the new map is involved in.
     * @param  type   the type of the map to create.
     * @return        the created map.
     */
    static Map * map(sf::RenderWindow *window, Level *level, MapType type);

    /**
     * Returns the filename of the minimap associated with this map.
     * @param  type the type of the map.
     * @return      the filename of the minimap associated with this map.
     */
    static const char * minimapFilename(MapType type);

private:
    /** Textures of the maps for the level picker. */
    static sf::Texture *sMapsPickerTextures;

    /**
     * Returns the textures of the maps for the level picker.
     * @return the textures of the maps for the level picker.
     */
    static sf::Texture *pickerTextures();

    /**
     * Returns the filename for the level picker for the given map type.
     * @param  type  the map type.
     * @return       the filename of the level picker sprite.
     */
    static std::string spritePickerFilename(MapType type);
};

#endif // MAP_FACTORY_H