
namespace sf {
    class Image;
}

class Minimap {
public:
    Minimap();
    ~Minimap();
    void loadFromFile(const char *minimapFilename);
    void draw();

private:
    void loadMatrixFromFile(const char*minimapFilename);

    sf::Image *mMinimapSprite;

    int mRowCount;
    int mColCount;
};