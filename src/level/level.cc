#include <SFML/Graphics.hpp>
#include <string>
#include "level.h"
#include "config.h"
#include "minimap.h"
#include "leaderboard.h"
#include "aivenusian.h"
#include "fontfactory.h"
#include "keyboardmanager.h"
#include "levelpicker.h"
#include "sfmlutil.h"
#include "mathutil.h"
#include "resourceutil.h"

#define LOG_TAG "{Level} "
#define CAN_LOG 1

#define GO_TEXT "Go!"

// Time of the countdown
#define RACE_START_TIMER_COUNTDOWN_SEC 3

// Millis between the countdown end and the race start
#define RACE_START_TIMER_GO_MS 300

// Laps to do for the race
#define RACE_LAP_COUNT 3

#define LAP_Q1_PERCENTAGE 0.25
#define LAP_Q2_PERCENTAGE 0.50
#define LAP_Q3_PERCENTAGE 0.75
#define LAP_Q4_PERCENTAGE 1.00

// AI karts color
#define AI_KART_RGB_BLUE 	23, 	147, 	243
#define AI_KART_RGB_YELLOW 	237, 	230, 	44
#define AI_KART_RGB_VIOLET 	119, 	10, 	210
#define AI_KART_RGB_ORANGE 	235, 	133, 	0
#define AI_KART_RGB_RED 	234, 	13, 	13
#define AI_KART_RGB_BROWN	133, 	74, 	4
#define AI_KART_RGB_AZURE 	0, 		216, 	235

// Radius in which two kart are considered in collision.
#define KART_CRASH_RADIUS 0.3

#define MENU_OPTION_UNSELECTED_RGB 		71,		71,		71
#define MENU_OPTION_SELECTED_RGB 		255,	255,	255

#define MENU_OPTION_SELECTED_ALPHA_LOW 			128
#define MENU_OPTION_SELECTED_ALPHA_HIGH 		255
#define MENU_OPTION_SELECTED_INCREMENT 			3

const float MENU_OPTION_FONT_SIZE = Config::WINDOW_HEIGHT / 18;

Level::Level(	sf::RenderWindow *window, KeyboardManager *keysManager,
				KartFactory::KartType kartType, MapFactory::MapType mapType)
			 : FadeScreen(window, keysManager) {
	mPlayerKartType = kartType;
	mMapType = mapType;

	mMap = MapFactory::map(mWindow, this, mMapType);
	mMap->loadMap();

	mMinimap = new Minimap(mWindow, mMapType);

	mLeaderboard = new Leaderboard(mWindow);

	initKarts();

	mRaceStartTimer.initialize(	RACE_START_TIMER_COUNTDOWN_SEC * 1000 +
								RACE_START_TIMER_GO_MS);
	mRaceStartTimerText = new sf::Text();
	mRaceStartTimerText->setFont((FontFactory::font(FontFactory::FontType::Fancy)));
	mRaceStartTimerText->setCharacterSize(Config::WINDOW_HEIGHT / 4);
	mRaceStartTimerText->setFillColor(sf::Color::Red);
	mRaceStartTimerText->setOutlineColor(sf::Color::Black);
	mRaceStartTimerText->setOutlineThickness(Config::WINDOW_HEIGHT / 120.0);

	mLapText = new sf::Text();
	updateLapText();
	mLapText->setFont((FontFactory::font(FontFactory::FontType::Fancy)));
	mLapText->setCharacterSize(Config::WINDOW_HEIGHT / 24);
	mLapText->setFillColor(sf::Color::Black);
	mLapText->setOutlineColor(sf::Color::White);
	mLapText->setOutlineThickness(Config::WINDOW_HEIGHT / 240.0);
	mLapText->setPosition(
		Config::WINDOW_WIDTH - mLapText->getGlobalBounds().width - Config::WINDOW_WIDTH / 18,
		Config::WINDOW_WIDTH / 18
	);

	// Menus frame
	mLevelRunningMenuFrameTexture = new sf::Texture();
	mLevelRunningMenuFrameTexture->loadFromFile(
		ResourceUtil::image("level_menu_frame_in_game.png").c_str());

	mLevelFinishedMenuFrameTexture = new sf::Texture();
	mLevelFinishedMenuFrameTexture->loadFromFile(
		ResourceUtil::image("level_menu_frame_out_game.png").c_str());

	mLevelFinishedMenuFrameSprite = new sf::Sprite();
	mLevelFinishedMenuFrameSprite->setTexture(*mLevelFinishedMenuFrameTexture);
	SFMLUtil::centerOrigin(mLevelFinishedMenuFrameSprite);
	mLevelFinishedMenuFrameSprite->setScale(Config::WINDOW_WIDTH / 640.0,
											Config::WINDOW_HEIGHT / 480.0);
	mLevelFinishedMenuFrameSprite->setPosition(
		Config::WINDOW_WIDTH * 3 / 4,
		Config::WINDOW_HEIGHT / 2
	);

	mLevelRunningMenuFrameSprite = new sf::Sprite();
	mLevelRunningMenuFrameSprite->setTexture(*mLevelRunningMenuFrameTexture);
	SFMLUtil::centerOrigin(mLevelRunningMenuFrameSprite);
	mLevelRunningMenuFrameSprite->setScale(	Config::WINDOW_WIDTH / 640.0,
											Config::WINDOW_HEIGHT / 480.0);
	mLevelRunningMenuFrameSprite->setPosition(
		Config::WINDOW_WIDTH / 2,
		Config::WINDOW_HEIGHT / 2
	);

	// Menus
	mOptionContinue.text = initializedOptionText("Continue");
	mOptionContinue.func = &Level::continueFunc;

	mOptionRetry.text = initializedOptionText("Retry");
	mOptionRetry.func = &Level::retryFunc;

	mOptionQuit.text = initializedOptionText("Quit");
	mOptionQuit.func = &Level::quitFunc;

	mMenuInGame.options.push_back(mOptionContinue);
	mMenuInGame.options.push_back(mOptionRetry);
	mMenuInGame.options.push_back(mOptionQuit);
	mMenuInGame.currentOptionIndex = 0;
	mMenuInGame.opened = false;

	mMenuLevelFinished.options.push_back(mOptionRetry);
	mMenuLevelFinished.options.push_back(mOptionQuit);
	mMenuLevelFinished.currentOptionIndex = 0;
	mMenuLevelFinished.opened = false;

	mMenuSelectedOptionCurrentAlpha = MENU_OPTION_SELECTED_ALPHA_HIGH;
	mMenuSelectedOptionCurrentAlphaIncrementSign = -1;

	// Updates the progress and the leadboard so that they are rendered
	// properly even if the race is not started yet
	updateKartsLapProgress();

	mLeaderboard->update();

	mLevelState = Running;

	mKeysManager->addListener(this);
}

Level::~Level() {
	delete mMap;
	delete mMinimap;
	delete mLeaderboard;

	delete mRaceStartTimerText;
	delete mLapText;

	delete mLevelFinishedMenuFrameSprite;
	delete mLevelFinishedMenuFrameTexture;

	delete mLevelRunningMenuFrameSprite;
	delete mLevelRunningMenuFrameTexture;


	delete mOptionContinue.text;
	delete mOptionRetry.text;
	delete mOptionQuit.text;

	deleteKarts();

	mDepthSprites.clear();

	mKeysManager->removeListener(this);
}

// ------------------------
// PUBLIC -----------------
// ------------------------

void Level::update() {

	FadeScreen::update();
	// Level should fade in, do not update the logic
	if (isFading())
		return;

	// Level is in pause, do not update the logic
	if (mMenuInGame.opened)
		return;

	if (mLevelState == Running) {
		// Race is not started => update start timer
		if (mRaceStartTimer.isRunning()) {
			updateStartTimeText();
		}
		// Race is already started => updates the level.
		else  {
			mMap->update();

			mPlayerKart.kart->update();

		    for (std::vector<RacingKart>::iterator aiIter = mAIKarts.begin();
		        aiIter != mAIKarts.end();
		        aiIter++)
					(*aiIter).kart->update();

			handleKartsCrash();

			updateKartsLapProgress();

			mLeaderboard->update();

			mLevelState = levelFinishedByAKart() ? Finished : Running;
			if (mLevelState == Running) {
				updateLapText();
			}
			else {
				d("Level has been finished by a kart");
				mLevelState = Finished;
				mMenuLevelFinished.opened = true;
			}
		}
	}
}

void Level::render() {

	mDepthSprites.clear();

	mMap->draw();

	mWindow->draw(*mLapText);

	mMinimap->draw();

	mPlayerKart.kart->draw();
	mMinimap->drawPoint(
		mPlayerKart.kart->position(),
		mPlayerKart.kart->minimapSize(),
		mPlayerKart.kart->color()
	);

    for (std::vector<RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++) {
			(*aiIter).kart->draw();
			mMinimap->drawPoint(
				(*aiIter).kart->position(),
				(*aiIter).kart->minimapSize(),
				(*aiIter).kart->color()
			);
	}

	renderSpritesForDepth();

	if (mRaceStartTimer.isRunning())
		mWindow->draw(*mRaceStartTimerText);

	mLeaderboard->draw(mLevelState != Finished);

	if (mMenuLevelFinished.opened)
		drawLevelFinishedMenu();

	if (mMenuInGame.opened)
		drawInGameMenu();

	FadeScreen::render();
}

void Level::onKeyPressed(int keyCode) {
	if (keyCode == sf::Keyboard::Key::Return &&
		mLevelState == Running &&
		!mMenuInGame.opened) {
		d("Opening in-game menu");
		mMenuInGame.opened = true;
	}
	else if (mMenuInGame.opened)
		updateMenu(mMenuInGame, keyCode);
	else if (mMenuLevelFinished.opened)
		updateMenu(mMenuLevelFinished, keyCode);

}

Kart * Level::playerKart() {
	return mPlayerKart.kart;
}

Map * Level::map() {
	return mMap;
}

void Level::pushSprite(sf::Sprite *sprite) {
	mDepthSprites.push_back(sprite);
}

// ------------------------
// PROTECTED --------------
// ------------------------

void Level::fadeInFinished() {
	mLevelState = Running;
	mRaceStartTimer.start();
}

void Level::fadeOutFinished() {
	if (mLevelState == Retrying) {
		mLevelState = Running;
		mRaceStartTimer.restart();
		deleteKarts();
		initKarts();
		updateStartTimeText();
		updateLapText();
		updateKartsLapProgress();
		mLeaderboard->update();
		fadeIn();
	}
	else if (mLevelState == Quitting) {
		mSegue = new LevelPicker(mWindow, mKeysManager);
	}
}

// ------------------------
// PRIVATE ----------------
// ------------------------


RacingKart Level::initialRacingKart(Kart *k) {
	RacingKart rk;
	rk.kart = k;
	rk.currentLap = 0;
	rk.q1Finished = rk.q2Finished = rk.q3Finished = true;
	return rk;
}

sf::Text * Level::initializedOptionText(const char *title) {
	sf::Text *optionText = new sf::Text();
	optionText->setFont(FontFactory::font(FontFactory::FontType::Plain));
	optionText->setCharacterSize(MENU_OPTION_FONT_SIZE);
	optionText->setOutlineThickness(1);
	optionText->setString(title);
	optionText->setOrigin(	optionText->getLocalBounds().width / 2,
							optionText->getLocalBounds().height * 3 / 5);
	return optionText;
}

bool Level::spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2) {
	return s1->getPosition().y < s2->getPosition().y;
}

void Level::deleteKarts() {
	delete mPlayerKart.kart;

 	for (std::vector<RacingKart>::iterator aiIter = mAIKarts.begin();
		 aiIter != mAIKarts.end();
		 aiIter++)
		 	delete aiIter->kart;

	mAIKarts.clear();
}

void Level::initKarts() {
	mLeaderboard->clearKarts();

	mPlayerKart = initialRacingKart(KartFactory::playerKart(this, mPlayerKartType));
	mPlayerKart.kart->setPosition(mMap->startingPointForRacePosition(mMap->lastRacePosition()));
	mLeaderboard->addKart(&mPlayerKart);

	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Harlock", new sf::Color(AI_KART_RGB_BLUE), mMap->randomAIPath())));
	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Lucky Star", new sf::Color(AI_KART_RGB_YELLOW), mMap->randomAIPath())));
	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Spock", new sf::Color(AI_KART_RGB_VIOLET), mMap->randomAIPath())));
	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Skywalker", new sf::Color(AI_KART_RGB_ORANGE), mMap->randomAIPath())));
	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Daneel", new sf::Color(AI_KART_RGB_RED), mMap->randomAIPath())));
	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Yoda", new sf::Color(AI_KART_RGB_AZURE), mMap->randomAIPath())));
	mAIKarts.push_back(initialRacingKart(
		new AIVenusian(this, "Baley", new sf::Color(AI_KART_RGB_BROWN), mMap->randomAIPath())));

	int racePosition = 1;
    for (std::vector<RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++, racePosition ++) {
			(*aiIter).kart->setPosition(mMap->startingPointForRacePosition(racePosition));
			mLeaderboard->addKart(&(*aiIter));
	}
}

void Level::renderSpritesForDepth() {
	std::sort(mDepthSprites.begin(), mDepthSprites.end(), spriteDepthCompareFunction);

    for (std::vector<sf::Sprite *>::iterator spriteIter = mDepthSprites.begin();
        spriteIter != mDepthSprites.end();
        spriteIter++)
        mWindow->draw(*(*spriteIter));
}

void Level::updateKartsLapProgress() {
	// Update the lap count for the player
	updateKartLapProgress(mPlayerKart);

    for (std::vector<RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
		updateKartLapProgress(*aiIter);
}

void Level::updateKartLapProgress(RacingKart &rk) {
	int sectorCount = mMap->sectorCount();
	rk.currentSector = mMap->sector(rk.kart->position());
	int currentSectorIndex = rk.currentSector.index;
	double lapPercentage = (double) currentSectorIndex / sectorCount;

	d2("Kart [", rk.kart->name(), "] is at sector ", currentSectorIndex,
		". Global lap percentage: ", lapPercentage * 100, "%");

	if (MathUtil::isInRange(Range {LAP_Q1_PERCENTAGE, LAP_Q2_PERCENTAGE},
							lapPercentage) &&
		!rk.q1Finished) {
		d2("Kart [", rk.kart->name(), "] has finished Q1");
		rk.q1Finished = true;
	}
	else if (MathUtil::isInRange(Range {LAP_Q2_PERCENTAGE, LAP_Q3_PERCENTAGE},
								 lapPercentage) &&
			 !rk.q2Finished &&
			 rk.q1Finished) {
		d2("Kart [", rk.kart->name(), "] has finished Q2");
		rk.q2Finished = true;
	}
	else if (MathUtil::isInRange(Range {LAP_Q3_PERCENTAGE, LAP_Q4_PERCENTAGE},
								 lapPercentage) &&
			 !rk.q3Finished &&
			 rk.q1Finished &&
		 	 rk.q2Finished) {
		d2("Kart [", rk.kart->name(), "] has finished Q3");
		rk.q3Finished = true;
	}

	if (rk.q1Finished && rk.q2Finished && rk.q3Finished &&
		mMap->tileEvent(rk.kart->position()) == Map::TileEvent::LapFinished) {
			d2("Kart [", rk.kart->name(), "] has finished ", rk.currentLap, "° lap");
			rk.q1Finished = rk.q2Finished = rk.q3Finished = false;
			rk.currentLap++;
	}
}

void Level::handleKartsCrash() {
	// Create temp array;
	int kartCount = mAIKarts.size() + 1; // player
	struct CrashableRacingKart {
		RacingKart *rk;
		Quad q;
	} crk[kartCount];

	// Fill the array
	crk[0].rk = &mPlayerKart;

	for (int i = 1; i < kartCount; i++) {
		crk[i].rk = &mAIKarts[i - 1];
	}

	// Calculate the quad for the karts
	for (int i = 0; i < kartCount; i++) {
		crk[i].q.ul.x = crk[i].rk->kart->position().x - KART_CRASH_RADIUS;
		crk[i].q.ul.y = crk[i].rk->kart->position().y - KART_CRASH_RADIUS;

		crk[i].q.ur.x = crk[i].rk->kart->position().x + KART_CRASH_RADIUS;
		crk[i].q.ur.y = crk[i].rk->kart->position().y - KART_CRASH_RADIUS;

		crk[i].q.dr.x = crk[i].rk->kart->position().x + KART_CRASH_RADIUS;
		crk[i].q.dr.y = crk[i].rk->kart->position().y + KART_CRASH_RADIUS;

		crk[i].q.dl.x = crk[i].rk->kart->position().x - KART_CRASH_RADIUS;
		crk[i].q.dl.y = crk[i].rk->kart->position().y + KART_CRASH_RADIUS;
	}

	// Check if the karts' quad intersects each other
	for (int i = 0; i < kartCount - 1; i++) {
		for (int j = i + 1; j < kartCount; j++) {
			if (crk[i].q.intersects(crk[j].q)) {
				d("Crash between [", crk[i].rk->kart->name(),
					"] and [[", crk[i].rk->kart->name(), "]");
				crk[i].rk->kart->crash(crk[j].rk->kart);
				crk[j].rk->kart->crash(crk[i].rk->kart);
			}
		}
	}
}

void Level::updateMenu(Menu &m, int keyCode) {
	int indexIncrement = 0;
	if (keyCode == sf::Keyboard::Key::Up)
		indexIncrement = -1;
	else if (keyCode == sf::Keyboard::Key::Down)
		indexIncrement = 1;
	else if (keyCode == sf::Keyboard::Key::Return) {
		d("Closing menu and doing the selected action");
		(this->*(m.options[m.currentOptionIndex].func))();
		m.opened = false;
	}
	m.currentOptionIndex = (m.currentOptionIndex + indexIncrement + m.options.size())
							% m.options.size();
}

void Level::updateLapText() {
	int renderedLap = mPlayerKart.currentLap > 0 ? mPlayerKart.currentLap : 1;
	std::string lapStr = std::to_string(renderedLap) + "/" + std::to_string(RACE_LAP_COUNT);
	mLapText->setString(lapStr);
}

void Level::updateStartTimeText() {
	int currentTicks = mRaceStartTimer.update();

	int remainingSec = 	RACE_START_TIMER_COUNTDOWN_SEC -
	 	(currentTicks / Config::TARGET_UPDATES_PER_SECOND);

	sf::String timerStr = (
		remainingSec <= 0 ?
			GO_TEXT :
			std::to_string(remainingSec)
	);

	mRaceStartTimerText->setString(timerStr);
	mRaceStartTimerText->setPosition(
		(Config::WINDOW_WIDTH - mRaceStartTimerText->getGlobalBounds().width) / 2,
		(Config::WINDOW_HEIGHT - mRaceStartTimerText->getGlobalBounds().height) / 2
	);
}

bool Level::levelFinishedByAKart() {
	if (mPlayerKart.currentLap > RACE_LAP_COUNT)
		return true;

    for (std::vector<RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
		if ((*aiIter).currentLap > RACE_LAP_COUNT)
			return true;

	return false;
}

void Level::drawMenuOptions(Menu &m, int frameCenterX, int frameTopY, int frameBottomY) {
	int optionIndex = 0;
	float marginBetweenOptions = (frameBottomY - frameTopY) / (m.options.size() + 1);

    for (std::vector<MenuOption>::iterator optionIter = m.options.begin();
        optionIter != m.options.end();
        optionIter++, optionIndex++) {
		int rowMarginTop =  frameTopY + (optionIndex + 1) * marginBetweenOptions;
		sf::Text *t = (*optionIter).text;
		t->setPosition(frameCenterX, rowMarginTop);

		if (optionIndex != m.currentOptionIndex)
			t->setFillColor(sf::Color(MENU_OPTION_UNSELECTED_RGB));
		else {
			t->setFillColor(sf::Color(MENU_OPTION_SELECTED_RGB, mMenuSelectedOptionCurrentAlpha));
		}

		mWindow->draw(*t);
	}
	// Updates the alpha value so that it goes from the lowest to the highest
	// value and changes increment sign when reach one of those (blink effect).
	mMenuSelectedOptionCurrentAlpha +=
		mMenuSelectedOptionCurrentAlphaIncrementSign * MENU_OPTION_SELECTED_INCREMENT;
	if (mMenuSelectedOptionCurrentAlpha > MENU_OPTION_SELECTED_ALPHA_HIGH ||
		mMenuSelectedOptionCurrentAlpha < MENU_OPTION_SELECTED_ALPHA_LOW) {
		mMenuSelectedOptionCurrentAlphaIncrementSign *= -1;
		mMenuSelectedOptionCurrentAlpha +=
			mMenuSelectedOptionCurrentAlphaIncrementSign * MENU_OPTION_SELECTED_INCREMENT;
	}
}

void Level::drawInGameMenu() {
	mWindow->draw(*mLevelRunningMenuFrameSprite);
	drawMenuOptions(mMenuInGame,
					mLevelRunningMenuFrameSprite->getPosition().x,
					mLevelRunningMenuFrameSprite->getGlobalBounds().top,
					mLevelRunningMenuFrameSprite->getGlobalBounds().top +
						mLevelRunningMenuFrameSprite->getGlobalBounds().height);
}

void Level::drawLevelFinishedMenu() {
	mWindow->draw(*mLevelFinishedMenuFrameSprite);
	drawMenuOptions(mMenuLevelFinished,
					mLevelFinishedMenuFrameSprite->getPosition().x,
					mLevelFinishedMenuFrameSprite->getGlobalBounds().top,
					mLevelFinishedMenuFrameSprite->getGlobalBounds().top +
						mLevelFinishedMenuFrameSprite->getGlobalBounds().height);
}

void Level::retryFunc() {
	mLevelState = Retrying;
	fadeOut();
}

void Level::continueFunc() {
	mLevelState = Running;
}

void Level::quitFunc() {
	mLevelState = Quitting;
	fadeOut();
}

const char * Level::logTag() {
	return LOG_TAG;
}

bool Level::canLog() {
	return CAN_LOG;
}
