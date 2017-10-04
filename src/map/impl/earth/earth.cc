#include <SFML/Graphics.hpp>
#include "earth.h"

static const Point POLE_POSITION {55, 96.5};
static const int STARTING_BLOCK_COUNT = 8;
static const int EARTH_EVEN_STARTING_BLOCK_X_DIFF = 5;
static const int EARTH_STARTING_BLOCK_Y_DIFF = 2;

#define EARTH_MAP_FILENAME "earth.txt"
#define EARTH_EVENTS_FILENAME "earth_events.txt"
#define EARTH_SECTORS_FILENAME "earth_sectors.txt"
#define EARTH_TILESET_FILENAME "earth_tileset.png"
#define EARTH_AI_PATH_FILENAME(X) "earth_ai_path_"#X".txt"

#define EARTH_SKY_RGB 146, 241, 255

Earth::Earth(sf::RenderWindow *window, Level *level) : Map(window, level) {}

void Earth::loadMap() {
    Map::loadMap(EARTH_MAP_FILENAME);
    Map::loadEvents(EARTH_EVENTS_FILENAME);
    Map::loadTileset(EARTH_TILESET_FILENAME);
    Map::loadSectors(EARTH_SECTORS_FILENAME);
    Map::loadBackground(new sf::Color(EARTH_SKY_RGB));

    Map::addAIPath(EARTH_AI_PATH_FILENAME(0));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(1));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(2));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(3));
    Map::addAIPath(EARTH_AI_PATH_FILENAME(4));
}

Point Earth::startingPointForRacePosition(int racePosition) {
    if (racePosition < 1 || racePosition > lastRacePosition()) {
        w("Earth doesn't provide starting point for position, ", racePosition, ".",
            "Returning last position instead");
        return startingPointForRacePosition(lastRacePosition());
    }

    bool even = racePosition % 2 == 0;

    return Point {
        POLE_POSITION.x - (even ? EARTH_EVEN_STARTING_BLOCK_X_DIFF : 0),
        POLE_POSITION.y + (racePosition - 1) * EARTH_STARTING_BLOCK_Y_DIFF};
}

int Earth::lastRacePosition() {
    return STARTING_BLOCK_COUNT;
}