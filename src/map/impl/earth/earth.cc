#include <SFML/Graphics.hpp>
#include "resourceutil.h"
#include "earth.h"
#include "logger.h"

Earth::Earth() {
    // initParameters();
}

void Earth::loadMap() {
    Map::loadMap("earth.txt", "earth_tileset.png");
}