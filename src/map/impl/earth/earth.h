#ifndef EARTH_H
#define EARTH_H

#include "map.h"

/** Concrete implementation of the map of earth planet. */
class Earth : public Map {
public:
    Earth(sf::RenderWindow *window, Level *level);
    void loadMap();
    Point startingPointForRacePosition(int racePosition);
    int lastRacePosition();
};

#endif // EARTH_H