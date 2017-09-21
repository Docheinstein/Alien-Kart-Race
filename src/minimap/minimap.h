#ifndef MINIMAP_H
#define MINIMAP_H

#include "game.h"

namespace sf {
    class Image;
    class Color;
}

class Minimap {
public:
    static const int MINIMAP_SIZE = Game::WINDOW_WIDTH * 0.4;
    Minimap();
    ~Minimap();

    void loadFromFile(const char *minimapFilename);
    void draw();

    void drawPoint(double row, double col, int size, sf::Color color);
private:
    void loadMatrixFromFile(const char*minimapFilename);

    sf::Texture *mMinimapTexture;
    sf::Sprite *mMinimapSprite;

    int mRowCount;
    int mColCount;
};

#endif // MINIMAP_H