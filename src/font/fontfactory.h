#ifndef FONT_FACTORY_H
#define FONT_FACTORY_H

namespace sf {
    class Font;
}

class FontFactory {
public:

    enum FontType {
        Main,
        _FontTypeCount
    };

    static sf::Font& font(FontType type);

private:
    static sf::Font *sFonts;

    static sf::Font *fonts();
};

#endif // FONT_FACTORY_H