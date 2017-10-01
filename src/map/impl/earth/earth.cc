#include <SFML/Graphics.hpp>
#include <string>
#include "level.h"
#include "resourceutil.h"
#include "earth.h"
#include "logger.h"

static const Point POLE_POSITION {55, 96.5};

#define EARTH_MAP_FILENAME "earth.txt"
#define EARTH_EVENTS_FILENAME "earth_events.txt"
#define EARTH_SECTORS_FILENAME "earth_sectors.txt"
#define EARTH_TILESET_FILENAME "earth_tileset.png"
#define EARTH_AI_PATH_FILENAME(X) "earth_ai_path_"#X".txt"

Earth::Earth(sf::RenderWindow *window, Level *level) : Map(window, level) {}

void Earth::loadMap() {
    Map::loadMap(EARTH_MAP_FILENAME);
    Map::loadEvents(EARTH_EVENTS_FILENAME);
    Map::loadTileset(EARTH_TILESET_FILENAME);
    Map::loadSectors(EARTH_SECTORS_FILENAME);

    Map::addAIPath(EARTH_AI_PATH_FILENAME(0));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(1));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(2));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(3));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(4));
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