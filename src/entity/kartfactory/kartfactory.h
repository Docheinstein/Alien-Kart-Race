#ifndef KART_FACTORY_H
#define KART_FACTORY_H

#include <string>
#include "kart.h"
#include "geometryutil.h"

namespace sf {
    class Texture;
}

class KartFactory {
public:

    enum KartType {
        Venusian,
        _KartTypeCount
    };

    static Kart::KartParams params(KartType type);
    static int sprites(KartType type, sf::Sprite *&sprites);
    static int skidGasSprites(KartType type, sf::Sprite *&gasSprites);

    static float scaleFactor(KartType type);

private:
    static sf::Texture **sKartsTextures;
    static sf::Texture **sKartsSkidGasTextures;

    static sf::Texture ** textures();
    static sf::Texture ** skidGasTextures();

    static int spriteCount(KartType type);
    static std::string spriteFilename(KartType type, int index);
    static Point spriteOrigin(KartType type, int index);

    static int spriteSkidGasCount(KartType type);
    static std::string spriteSkidGasFilename(KartType type, int index);
    static Point spriteSkidGasOrigin(KartType type, int index);
};

#endif // KART_FACTORY_H