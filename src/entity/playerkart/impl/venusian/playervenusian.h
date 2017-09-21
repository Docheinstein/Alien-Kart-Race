#ifndef PLAYER_VENUSIAN_H
#define PLAYER_VENUSIAN_H

#include "playerkart.h"

/*
namespace sf {
    class Color;
    class Texture;
    class Sprite;
}
*/

class PlayerVenusian : public PlayerKart {
public:
    PlayerVenusian();
    sf::Color minimapColor();
    void draw();
protected:
    enum GasSpriteType {
        GasLeft,
        GasRight,
        _GasSpriteTypeCount
    };
    sf::Texture mGasTex[2];
    sf::Sprite mGasSprite[2];

    void initParameters();
    void initSprites();
};

#endif // PLAYER_VENUSIAN_H