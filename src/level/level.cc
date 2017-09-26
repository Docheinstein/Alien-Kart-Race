#include <SFML/Graphics.hpp>
#include <string>
#include "level.h"
#include "earth.h"
#include "playervenusian.h"
#include "aivenusian.h"
#include "minimap.h"
#include "resourceutil.h"
#include "stdutil.h"
#include "fontfactory.h"
#include "gametimer.h"
#include "leaderboard.h"

#define LOG_TAG "{Level} "
#define CAN_LOG 1

#define GO_TEXT "Go!"

#define FADE_STARTING_ALPHA 200
#define FADE_ENDING_ALPHA 1

#define FADE_TIMER_COUNTDOWN_MS 800

#define RACE_START_TIMER_COUNTDOWN_SEC 3
#define RACE_START_TIMER_GO_MS 300

Level::Level() {
	mMap = new Earth();
	mMap->loadMap();

	mMinimap = new Minimap();
	mMinimap->loadFromFile(ResourceUtil::raw("earth_minimap.txt"));

	mLeaderboard = new Leaderboard();

	mPlayerKart = new PlayerVenusian("Me");
	mPlayerKart->setPosition(mMap->startingPointForRacePosition(mMap->lastRacePosition()));
	mLeaderboard->addKart(mPlayerKart);

	mAIKarts.push_back(new AIVenusian("Starky"));

	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());
	// mAIKarts.push_back(new AIVenusian());

	int racePosition = 2;
    for (std::vector<AIKart *>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++, racePosition ++) {
			mLeaderboard->addKart(*aiIter);
			(*aiIter)->setPosition(mMap->startingPointForRacePosition(racePosition));
		}

	mFadeInTimer.initialize(FADE_TIMER_COUNTDOWN_MS);
	mRaceStartTimer.initialize(	RACE_START_TIMER_COUNTDOWN_SEC * 1000 +
								RACE_START_TIMER_GO_MS);
	mRaceStartTimerText = new sf::Text();
	mRaceStartTimerText->setFont((FontFactory::font(FontFactory::FontType::Main)));
	mRaceStartTimerText->setCharacterSize(Game::WINDOW_HEIGHT / 4);
	mRaceStartTimerText->setFillColor(sf::Color::Red);
	mRaceStartTimerText->setOutlineColor(sf::Color::Black);
	mRaceStartTimerText->setOutlineThickness((float)Game::WINDOW_HEIGHT / 120);

	sf::Image overlayImage;
	overlayImage.create(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, sf::Color::Black);
	mOverlayTexture = new sf::Texture();
	mOverlayTexture->loadFromImage(overlayImage);

	mOverlaySprite = new sf::Sprite();
	mOverlaySprite->setTexture(*mOverlayTexture);
}

Level::~Level() {
	delete mMap;
	delete mRaceStartTimerText;
	delete mMinimap;
	delete mLeaderboard;
	delete mOverlayTexture;
	delete mOverlaySprite;

	delete mPlayerKart;
	STDUtil::deleteContainerPtr(mAIKarts.begin(), mAIKarts.end());
	mDepthSprites.clear();
}

PlayerKart * Level::playerKart() {
	return mPlayerKart;
}

Map * Level::map() {
	return mMap;
}

void Level::update() {
	// Level should fade in
	if (mFadeInTimer.isRunning()) {
		mFadeInTimer.update();
	}
	else {
		// Race is not started => update start timer
		if (mRaceStartTimer.isRunning()) {
				int currentTicks = mRaceStartTimer.update();
				int remainingSec = 	RACE_START_TIMER_COUNTDOWN_SEC - (currentTicks / Game::TARGET_UPDATES_PER_SECOND);

				sf::String timerStr = (
					remainingSec <= 0 ?
						GO_TEXT :
						std::to_string(remainingSec)
				);
				mRaceStartTimerText->setString(timerStr);
				mRaceStartTimerText->setPosition(
					(Game::WINDOW_WIDTH - mRaceStartTimerText->getGlobalBounds().width) / 2,
					(Game::WINDOW_HEIGHT - mRaceStartTimerText->getGlobalBounds().height) / 2
				);
		}
		// Race is already started => updates the level.
		else {
			mPlayerKart->update();
			mMap->update();
			mLeaderboard->update();

		    for (std::vector<AIKart *>::iterator aiIter = mAIKarts.begin();
		        aiIter != mAIKarts.end();
		        aiIter++)
					(*aiIter)->update();

		}
	}
}

void Level::render() {

	mDepthSprites.clear();

	mMap->draw();

	mMinimap->draw();

	mLeaderboard->draw();

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

	if (mRaceStartTimer.isRunning())
		Game::instance().window()->draw(*mRaceStartTimerText);

	if (mFadeInTimer.isRunning()) {
		int currentFadeTicks = mFadeInTimer.currentTicks();
		int alphaValue = MathUtil::changeRange(
			MathUtil::Range {0, FADE_TIMER_COUNTDOWN_MS},
			MathUtil::Range {FADE_STARTING_ALPHA, FADE_ENDING_ALPHA},
			TimeUtil::updatesToMillis(currentFadeTicks)
		);
		d2("Alpha val:", alphaValue);
		d2("currentFadeTicks", currentFadeTicks);
		mOverlaySprite->setColor(sf::Color(255, 255, 255, alphaValue));
		Game::instance().window()->draw(*mOverlaySprite);
	}
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

const char * Level::logTag() {
	return LOG_TAG;
}

bool Level::canLog() {
	return CAN_LOG;
}
