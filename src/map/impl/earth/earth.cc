#include <SFML/Graphics.hpp>
#include "resourceutil.h"
#include "earth.h"
#include "logger.h"

static const Point POLE_POSITION {55, 96.5};

Earth::Earth() {
    // initParameters();
}

void Earth::loadMap() {
    Map::loadMap("earth.txt", "earth_tileset.png", "earth_sectors.txt");
}

Point Earth::startingPointForRacePosition(int racePosition) {
    if (racePosition < 1 || racePosition > STARTING_BLOCK_COUNT) {
        w("Earth doesn't provide starting point for position, ", racePosition);
        return Point { 0, 0 };
    }

    bool even = racePosition % 2 == 0;

    d("POLE_POSITION.y\t",POLE_POSITION.y);
    d("POLE_POSITION.y + racePosition * 2:\t", POLE_POSITION.y + racePosition * 2);
    return Point {
        POLE_POSITION.x - (even ? 5 : 0),
        POLE_POSITION.y + (racePosition - 1) * 2};
}

int Earth::lastRacePosition() {
    return STARTING_BLOCK_COUNT;
}