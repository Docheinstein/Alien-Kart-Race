#ifndef MINIMAP_H
#define MINIMAP_H

#include "mapfactory.h"

namespace sf {
    class RenderWindow;
    class Image;
    class Color;
}

class Minimap {
public:
    static const int MINIMAP_SIZE;
    Minimap(sf::RenderWindow *window, MapFactory::MapType mapType);
    ~Minimap();

    void draw();

    void drawPoint(double row, double col, int size, sf::Color color);

private:
    void loadFromFile(const char *minimapFilename);

    sf::RenderWindow *mWindow;
    sf::Texture *mMinimapTexture;
    sf::Sprite *mMinimapSprite;

    int mRowCount;
    int mColCount;
};

#endif // MINIMAP_H