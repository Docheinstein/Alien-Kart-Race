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
class PlayerKart;

class KartFactory {
public:

    enum KartType {
        VenusianType,
        _KartTypeCount
    };

    static Kart::KartParams params(KartType type);
    static int sprites(KartType type, sf::Sprite *&sprites);
    static int skidGasSprites(KartType type, sf::Sprite *&gasSprites);

    static float scaleFactor();
    static sf::Sprite *leadeboardFaceSprite(KartType type);
    static sf::Sprite *pickerSprite(KartType type);
    static const char *name(KartType type);
    static Kart *playerKart(Level *level, KartType type);

private:
    static sf::Texture **sKartsTextures;
    static sf::Texture **sKartsSkidGasTextures;
    static sf::Texture * sKartsLeadboardFacesTextures;
    static sf::Texture * sKartsPickerTextures;

    static sf::Texture ** textures();
    static sf::Texture ** skidGasTextures();
    static sf::Texture *  leadeboardFacesTextures();
    static sf::Texture *  pickerTextures();

    static int spriteCount(KartType type);
    static std::string spriteFilename(KartType type, int index);
    static Point spriteOrigin(KartType type, int index);

    static int spriteSkidGasCount(KartType type);
    static std::string spriteSkidGasFilename(KartType type, int index);
    static Point spriteSkidGasOrigin(KartType type, int index);

    static std::string spriteLeadeboardFaceFilename(KartType type);
    static std::string spritePickerFilename(KartType type);
};

#endif // KART_FACTORY_H