#include "level.h"
#include "earth.h"
#include "playervenusian.h"
#include "aivenusian.h"
#include "minimap.h"
#include "resourceutil.h"

Level::Level() {
	mPlayerKart = new PlayerVenusian();
	mPlayerKart->setCol((double)55);
	mPlayerKart->setRow((double)96.5);

	mAIKarts = {new AIVenusian()};
	mAIKarts[0].setRow(/*98.5*/75);
	mAIKarts[0].setCol(/*50*/99);


	mMap = new Earth();
	mMap->loadMap();

	mMinimap = new Minimap();

	mMinimap->loadFromFile(ResourceUtil::raw("earth_minimap.txt"));
}

Level::~Level() {
	delete mPlayerKart;
	delete mMap;
}

PlayerKart * Level::playerKart() {
	return mPlayerKart;
}

Map * Level::map() {
	return mMap;
}

void Level::update() {
	mPlayerKart->update();
	mMap->update();

	for (int i = 0; i < AI_KART_COUNT; i++) {
		mAIKarts[i].update();
	}
}

void Level::render() {
	mMap->draw();

	mMinimap->draw();

	mPlayerKart->draw();

	mMinimap->drawPoint(
		mPlayerKart->row(), mPlayerKart->col(),
		mPlayerKart->minimapSize(), mPlayerKart->minimapColor()
	);

	for (int i = 0; i < AI_KART_COUNT; i++) {
		mMinimap->drawPoint(
			mAIKarts[i].row(), mAIKarts[i].col(),
			mAIKarts[i].minimapSize(), mAIKarts[i].minimapColor()
		);
	}
}
