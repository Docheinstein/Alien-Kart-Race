#include "level.h"
#include "earth.h"
#include "playervenusian.h"
#include "aivenusian.h"
#include "minimap.h"
#include "resourceutil.h"

Level::Level() {
	mMap = new Earth();
	mMap->loadMap();

	mPlayerKart = new PlayerVenusian();
	mPlayerKart->setPosition(mMap->startingPointForRacePosition(mMap->lastRacePosition()));

	mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());

	int racePosition = 2;
    for (std::vector<AIKart *>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++, racePosition ++)
			(*aiIter)->setPosition(mMap->startingPointForRacePosition(racePosition));

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

    for (std::vector<AIKart *>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
			(*aiIter)->update();
}

void Level::render() {
	mDepthSprites.clear();

	mMap->draw();

	mMinimap->draw();

    for (std::vector<AIKart *>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
			(*aiIter)->draw();

	mPlayerKart->draw();

	mMinimap->drawPoint(
		mPlayerKart->position().y, mPlayerKart->position().x,
		mPlayerKart->minimapSize(), mPlayerKart->minimapColor()
	);

    for (std::vector<AIKart *>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
			mMinimap->drawPoint(
				(*aiIter)->position().y, (*aiIter)->position().x,
				(*aiIter)->minimapSize(), (*aiIter)->minimapColor()
			);

	renderSpritesForDepth();

}

void Level::pushSprite(sf::Sprite *sprite) {
	mDepthSprites.push_back(sprite);
}

bool Level::spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2) {
	return s1->getPosition().y < s2->getPosition().y;
}

void Level::renderSpritesForDepth() {
	std::sort(mDepthSprites.begin(), mDepthSprites.end(), spriteDepthCompareFunction);

    for (std::vector<sf::Sprite *>::iterator spriteIter = mDepthSprites.begin();
        spriteIter != mDepthSprites.end();
        spriteIter++)
        Game::instance().window()->draw(*(*spriteIter));
}
