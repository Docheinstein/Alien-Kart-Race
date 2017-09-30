#ifndef EARTH_H
#define EARTH_H

#include "map.h"

class Earth : public Map {
public:
    static const int STARTING_BLOCK_COUNT = 8;
    Earth(sf::RenderWindow *window, Level *level);
    void loadMap();
    Point startingPointForRacePosition(int racePosition);
    int lastRacePosition();
};

#endif // EARTH_H