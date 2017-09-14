#include "level.h"
#include "map.h"
#include "venusian.h"

Level::Level() {
	mKart = new Venusian();
	mKart->setCol((double)30);
	mKart->setRow((double)30);
	mKart->setDirection(5);

	mMap = new Map();
	mMap->loadMap(Map::MapType::FirstMap);
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
	mKart->draw();
	mKart->drawOnMiniMap();
}
