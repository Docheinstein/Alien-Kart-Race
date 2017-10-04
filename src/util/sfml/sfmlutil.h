#ifndef SFML_UTIL_H
#define SFML_UTIL_H

namespace sf {
    class Sprite;
    class Text;
}

/** Provides utility function for SFML entities. */
class SFMLUtil {
public:

    /**
     * Centers the origin of the sprite within its x and y center.
     * @param  s the sprite.
     */
    static void centerOrigin(sf::Sprite *s);

    /**
     * Centers the origin of the text within its x and y center.
     * @param  t the text.
     */
    static void centerOrigin(sf::Text *t);
};

#endif