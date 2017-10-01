#ifndef PLAYER_VENUSIAN_H
#define PLAYER_VENUSIAN_H

#include "playerkart.h"

/** Implementation of a player kart of type Venusian. */
class PlayerVenusian : public PlayerKart {
public:
    PlayerVenusian(Level *level, const char *kartName, sf::Color *kartColor);
    void draw();

protected:
    /** Initializes the kart's parameters. */
    void initParameters();

	/** Initializes the kart's sprites. */
    void initSprites();
};

#endif // PLAYER_VENUSIAN_H