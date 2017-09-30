#ifndef PLAYER_VENUSIAN_H
#define PLAYER_VENUSIAN_H

#include "playerkart.h"

class PlayerVenusian : public PlayerKart {
public:
    PlayerVenusian(Level *level, const char *kartName, sf::Color *kartColor);
    void draw();
protected:

    void initParameters();
    void initSprites();
};

#endif // PLAYER_VENUSIAN_H