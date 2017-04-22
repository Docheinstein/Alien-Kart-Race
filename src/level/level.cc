#include "level.h"
#include "map.h"
#include "kart.h"

Level::Level() {
	mMap = new Map();
	mMap->loadMap(Map::MapType::FirstMap);

	mKart = new Kart();
	mKart->setCol((double)50);
	mKart->setRow((double)50);
}

Level::~Level() {
	delete mKart;
}

Kart * Level::kart() {
	return mKart;
}

Map * Level::map() {
	return mMap;
}

void Level::update() {
	mKart->update();
	mMap->update();
}

void Level::render() {
	mMap->drawMap();
	mMap->drawMiniMap();
	mKart->drawOnMiniMap();
}
