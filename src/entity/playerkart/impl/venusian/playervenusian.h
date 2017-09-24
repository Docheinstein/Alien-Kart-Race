#ifndef PLAYER_VENUSIAN_H
#define PLAYER_VENUSIAN_H

#include "playerkart.h"

namespace sf {
    class Sprite;
}


class PlayerVenusian : public PlayerKart {
public:
    PlayerVenusian();
    sf::Color minimapColor();
    void draw();
protected:

    void initParameters();
    void initSprites();
};

#endif // PLAYER_VENUSIAN_H