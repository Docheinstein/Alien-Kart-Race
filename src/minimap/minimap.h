#ifndef MINIMAP_H
#define MINIMAP_H

#include "mapfactory.h"
#include "geometryentities.h"

namespace sf {
    class RenderWindow;
    class Image;
    class Color;
}

/** Represents a simplified map shown on a corner of the screen. */
class Minimap {
public:
    /** Size of the minimap. */
    static const int MINIMAP_SIZE;

    /**
     * Creates a new minimap.
     * @param window  the window this minimap will be drawn to.
     * @param mapType the map type of the minimap.
     */
    Minimap(sf::RenderWindow *window, MapFactory::MapType mapType);
    ~Minimap();

    /** Draws the minimap. */
    void draw();

    /**
     * Draws a point on the minimap at the given coordinate with the given size and color.
     * @param p     the point to draw.
     * @param size  the size of the point.
     * @param color the color of the point.
     */
    void drawPoint(const Point &p, int size, sf::Color color);

private:
    /**
     * Loads the minimap from the file with the given filename.
     * @param minimapFilename the name of the minimap file.
     */
    void loadFromFile(const char *minimapFilename);

    /** Window the minimap will be drawn to. */
    sf::RenderWindow *mWindow;

    /** Texture of the minimap. */
    sf::Texture *mMinimapTexture;

    /** Sprite of the minimap. */
    sf::Sprite *mMinimapSprite;

    /** Number of rows of the underlying map. */
    int mRowCount;

    /** Number of columns of the underlying map. */
    int mColCount;
};

#endif // MINIMAP_H