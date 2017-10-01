#ifndef KART_FACTORY_H
#define KART_FACTORY_H

#include <string>
#include "kart.h"
#include "geometryutil.h"

namespace sf {
    class Texture;
    class Sprite;
}

class Level;

/**
 * Factory class usable for create and retrieve information about
 * concrete types of karts.
 */
class KartFactory {
public:
    /** Type of kart. */
    enum KartType {
        /** Alien from the venus planet :) */
        VenusianType,

        /** Number of kart. */
        _KartTypeCount
    };

    /**
     * Returns the parameters for the given kart type.
     * @param  type             the type of the kart.
     * @param  randomnessFactor a factor of randomness that will be applied to the parameters.
     *                          each parameter will be added or subtracted by a random number between
     *                          -randomnessFactor and +randomnessFactor.
     * @return                  the parameters for the given kart type.
     */
    static Kart::KartParams params(KartType type, double randomnessFactor = 0);

    /**
     * Returns the sprites of the given kart type to use for render on the map.
     * @param  type    the type of the kart.
     * @param  sprites the sprites pointer that will be filled with the sprites.
     * @return         the number of created sprites.
     */
    static int sprites(KartType type, sf::Sprite *&sprites);

    /**
     * Returns the sprites of the given kart type to used for render the skid effect on the map.
     * @param  type       the type of the kart.
     * @param  gasSprites the sprites pointer that will be filled with the sprites.
     * @return            the number of created sprites.
     */
    static int skidGasSprites(KartType type, sf::Sprite *&gasSprites);

    /**
     * Returns the scale factor for the kart.
     * This actually doesn't take any type as parameter since
     * the scale should be the same for every kart.
     * @return the scale factor for the kart.
     */
    static float scaleFactor();

    /**
     * Returns the sprite to use for the leaderboard for the given kart type.
     * @param  type the type of the kart.
     * @return      the sprite to use for the leaderboard.
     */
    static sf::Sprite *leadeboardFaceSprite(KartType type);

    /**
     * Returns the sprite to used for the level picker for the given kart type.
     * @param  type the type of the kart.
     * @return      the sprite to use for the level picker.
     */
    static sf::Sprite *pickerSprite(KartType type);

    /**
     * Returns the name associated with the kart type.
     * This actually matches the string representation of the type.
     * @param  type the name associated with the kart type.
     * @return      the name of type.
     */
    static const char *name(KartType type);

    /**
     * Creates and returns a player kart of the given kart type.
     * @param  level the level the newly player kart will be part of.
     * @param  type  the type of the kart to create.
     * @return       the player kart.
     */
    static Kart *playerKart(Level *level, KartType type);

private:
    // Textures are static since are common between sprites.

    /** Textures of the karts. */
    static sf::Texture **sKartsTextures;

    /** Textures of the karts' skid. */
    static sf::Texture **sKartsSkidGasTextures;

    /** Textures of the karts for the leaderboard. */
    static sf::Texture * sKartsLeadboardFacesTextures;

    /** Textures of the karts forf the level picker. */
    static sf::Texture * sKartsPickerTextures;

    /**
     * Returns the textures for the karts.
     * @return the textures for the karts.
     */
    static sf::Texture ** textures();

    /**
     * Returns the textures for the kart's skid.
     * @return the textures for the kart's skid.
     */
    static sf::Texture ** skidGasTextures();

    /**
     * Returns the textures of the karts for the leaderboard.
     * @return the textures of the karts for the leaderboard.
     */
    static sf::Texture *  leadeboardFacesTextures();

    /**
     * Returns the textures of the karts for the level picker.
     * @return the textures of the karts for the level picker.
     */
    static sf::Texture *  pickerTextures();

    /**
     * Returns the number of sprites used by the given kart type.
     * @param  type the kart type.
     * @return      the count of sprite for the given kart type.
     */
    static int spriteCount(KartType type);

    /**
     * Returns the filename for the given kart type at the specified index.
     * @param  type  the kart type.
     * @param  index the index of the sprite.
     * @return       the filename of the sprite.
     */
    static std::string spriteFilename(KartType type, int index);

    /**
     * Returns the origin (center of the image) for the given kart type.
     * @param  type  the kart type.
     * @param  index the index of the sprite.
     * @return       the origin of the sprite.
     */
    static Point spriteOrigin(KartType type, int index);

    /**
     * Returns the number of sprites used for the skid effect by the given kart type.
     * @param  type the kart type.
     * @return      the count of skid sprite for the given kart type.
     */
    static int spriteSkidGasCount(KartType type);

    /**
     * Returns the filename for the skid for the given kart type at the specified index.
     * @param  type  the kart type.
     * @param  index the index of the sprite.
     * @return       the filename of the skid sprite.
     */
    static std::string spriteSkidGasFilename(KartType type, int index);

    /**
     * Returns the origin (center of the image) for the skid sprite for given kart type.
     * @param  type  the kart type.
     * @param  index the index of the sprite.
     * @return       the origin of the skid sprite.
     */
    static Point spriteSkidGasOrigin(KartType type, int index);

    /**
     * Returns the filename for the leaderboard for the given kart type.
     * @param  type  the kart type.
     * @return       the filename of the leaderboard sprite.
     */
    static std::string spriteLeadeboardFaceFilename(KartType type);

    /**
     * Returns the filename for the level picker for the given kart type.
     * @param  type  the kart type.
     * @return       the filename of the level picker sprite.
     */
    static std::string spritePickerFilename(KartType type);
};

#endif // KART_FACTORY_H