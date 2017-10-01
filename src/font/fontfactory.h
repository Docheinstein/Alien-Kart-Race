#ifndef FONT_FACTORY_H
#define FONT_FACTORY_H

namespace sf {
    class Font;
}

/**
 * Factory class responsible for share the font of the
 * game between the game's sf::Text.
 */
class FontFactory {
public:
    /** Type of font. */
    enum FontType {
        /** Plain font. */
        Plain,

        /** Semibold font. */
        Semibold,

        /** Bold and fancy font. */
        Fancy,

        /** Number of fonts. */
        _FontTypeCount
    };

    /**
     * Returns a reference to the shared font of the given type.
     * @param  type the type of the font.
     * @return      the shared font.
     */
    static sf::Font& font(FontType type);

private:
    /** Shared fonts. */
    static sf::Font *sFonts;

    /**
     * Returns the fonts shared between game's sf::Text.
     * @return the shared fonts.
     */
    static sf::Font *fonts();
};

#endif // FONT_FACTORY_H