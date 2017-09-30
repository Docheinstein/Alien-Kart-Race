#include <SFML/Graphics.hpp>
#include <string>
#include "level.h"
#include "const.h"
#include "minimap.h"
#include "leaderboard.h"
#include "aivenusian.h"
#include "resourceutil.h"
#include "stdutil.h"
#include "fontfactory.h"
#include "keyboardmanager.h"

#define LOG_TAG "{Level} "
#define CAN_LOG 1

#define GO_TEXT "Go!"

#define FADE_STARTING_ALPHA 200
#define FADE_ENDING_ALPHA 1

#define FADE_TIMER_COUNTDOWN_MS 800

#define RACE_START_TIMER_COUNTDOWN_SEC 0
#define RACE_START_TIMER_GO_MS 300

#define RACE_LAP_COUNT 3

#define LAP_Q1_PERCENTAGE 0.25
#define LAP_Q2_PERCENTAGE 0.50
#define LAP_Q3_PERCENTAGE 0.75
#define LAP_Q4_PERCENTAGE 1.00

#define AI_KART_RGB_BLUE 	23, 	147, 	243
#define AI_KART_RGB_YELLOW 	237, 	230, 	44
#define AI_KART_RGB_VIOLET 	119, 	10, 	210
#define AI_KART_RGB_ORANGE 	235, 	133, 	0
#define AI_KART_RGB_RED 	234, 	13, 	13
#define AI_KART_RGB_BROWN	133, 	74, 	4
#define AI_KART_RGB_AZURE 	0, 		216, 	235

#define KART_CRASH_RADIUS 0.3

#define MENU_OPTION_UNSELECTED_RGB 		71,		71,		71
#define MENU_OPTION_SELECTED_RGB 		255,	255,	255

#define MENU_OPTION_SELECTED_ALPHA_LOW 			128
#define MENU_OPTION_SELECTED_ALPHA_HIGH 		255
#define MENU_OPTION_SELECTED_INCREMENT 			3

const float MENU_PADDING_TOP = Const::WINDOW_HEIGHT / 10;
const float MENU_OPTION_FONT_SIZE = Const::WINDOW_HEIGHT / 18;
const float MENU_MARGIN_BETWEEN_OPTIONS = Const::WINDOW_HEIGHT / 14;

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
	mRaceStartTimerText->setCharacterSize(Const::WINDOW_HEIGHT / 4);
	mRaceStartTimerText->setFillColor(sf::Color::Red);
	mRaceStartTimerText->setOutlineColor(sf::Color::Black);
	mRaceStartTimerText->setOutlineThickness(Const::WINDOW_HEIGHT / 120.0);

	mLapText = new sf::Text();
	mLapText->setFont((FontFactory::font(FontFactory::FontType::Fancy)));
	mLapText->setCharacterSize(Const::WINDOW_HEIGHT / 24);
	mLapText->setFillColor(sf::Color::Black);

	sf::Image overlayImage;
	overlayImage.create(Const::WINDOW_WIDTH, Const::WINDOW_HEIGHT, sf::Color::Black);
	mOverlayTexture = new sf::Texture();
	mOverlayTexture->loadFromImage(overlayImage);

	mOverlaySprite = new sf::Sprite();
	mOverlaySprite->setTexture(*mOverlayTexture);

	mLevelFinishedMenuFrameTexture = new sf::Texture();
	mLevelFinishedMenuFrameTexture->loadFromFile(
		ResourceUtil::image("level_finished_menu_frame.png").c_str());

	mLevelFinishedMenuFrameSprite = new sf::Sprite();
	mLevelFinishedMenuFrameSprite->setTexture(*mLevelFinishedMenuFrameTexture);
	mLevelFinishedMenuFrameSprite->setOrigin(
		mLevelFinishedMenuFrameSprite->getLocalBounds().width / 2,
		mLevelFinishedMenuFrameSprite->getLocalBounds().height / 2
	);
	mLevelFinishedMenuFrameSprite->setScale(Const::WINDOW_WIDTH / 640.0, Const::WINDOW_HEIGHT / 480.0);
	mLevelFinishedMenuFrameSprite->setPosition(
		Const::WINDOW_WIDTH * 3 / 4,
		Const::WINDOW_HEIGHT / 2
	);


	// Menus
	mOptionContinue.text = optionText("Continue");
	mOptionContinue.func = &Level::continueFunc;

	mOptionRetry.text = optionText("Retry");
	mOptionRetry.func = &Level::retryFunc;

	mOptionQuit.text = optionText("Quit");
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

	updateKartsLapProgress();
	mLeaderboard->update();

	mLevelFinished = false;

	mKeysManager->addListener(this);
}

Level::~Level() {
	delete mMap;
	delete mMinimap;
	delete mLeaderboard;

	delete mRaceStartTimerText;
	delete mLapText;
	delete mOverlayTexture;
	delete mOverlaySprite;
	delete mLevelFinishedMenuFrameTexture;
	delete mLevelFinishedMenuFrameSprite;

	deleteKarts();

	mDepthSprites.clear();

	mKeysManager->removeListener(this);
}

Kart * Level::playerKart() {
	return mPlayerKart.kart;
}

Map * Level::map() {
	return mMap;
}

void Level::onKeyPressed(int keyCode) {
	if (mMenuInGame.opened)
		updateMenu(mMenuInGame, keyCode);
	else if (mMenuLevelFinished.opened)
		updateMenu(mMenuLevelFinished, keyCode);
}

void Level::update() {

	FadeScreen::update();
	// Level should fade in
	if (!isFading()) {
		// Race is not started => update start timer
		if (mRaceStartTimer.isRunning()) {
			int currentTicks = mRaceStartTimer.update();
			int remainingSec = 	RACE_START_TIMER_COUNTDOWN_SEC -
			 	(currentTicks / Const::TARGET_UPDATES_PER_SECOND);

			sf::String timerStr = (
				remainingSec <= 0 ?
					GO_TEXT :
					std::to_string(remainingSec)
			);
			mRaceStartTimerText->setString(timerStr);
			mRaceStartTimerText->setPosition(
				(Const::WINDOW_WIDTH - mRaceStartTimerText->getGlobalBounds().width) / 2,
				(Const::WINDOW_HEIGHT - mRaceStartTimerText->getGlobalBounds().height) / 2
			);
		}
		// Race is already started => updates the level.
		else if (!mLevelFinished) {
			// mMap->update();

			mPlayerKart.kart->update();

		    for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
		        aiIter != mAIKarts.end();
		        aiIter++)
					(*aiIter).kart->update();

			handleKartsCrash();

			updateKartsLapProgress();

			mLeaderboard->update();

			mLevelFinished = levelFinishedByAKart();
			if (!mLevelFinished) {
				// Current lap
				int renderedLap = mPlayerKart.currentLap > 0 ? mPlayerKart.currentLap : 1;
				std::string lapStr = std::to_string(renderedLap) + "/" + std::to_string(RACE_LAP_COUNT);
				mLapText->setString(lapStr);
				mLapText->setPosition(
					(Const::WINDOW_WIDTH - mLapText->getGlobalBounds().width) -
						Const::WINDOW_WIDTH / 12,
					Const::WINDOW_WIDTH / 12
				);
			}
			else {
				d("Level has been finished by a kart");
				mMenuLevelFinished.opened = true;
			}
		}
		else {
			// updateMenu(mMenuLevelFinished);
			// d2("Updating for level finished");
		}
	}
}

void Level::render() {

	mDepthSprites.clear();

	mMap->draw();

	mMinimap->draw();

	if (!mLevelFinished)
		mLeaderboard->draw(false);

    for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
			(*aiIter).kart->draw();

	mPlayerKart.kart->draw();

	mMinimap->drawPoint(
		mPlayerKart.kart->position().y, mPlayerKart.kart->position().x,
		mPlayerKart.kart->minimapSize(), mPlayerKart.kart->color()
	);

    for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
			mMinimap->drawPoint(
				(*aiIter).kart->position().y, (*aiIter).kart->position().x,
				(*aiIter).kart->minimapSize(), (*aiIter).kart->color()
			);

	renderSpritesForDepth();

	if (mRaceStartTimer.isRunning())
		mWindow->draw(*mRaceStartTimerText);

	if (mLevelFinished) {
		// d2("Drawing for level finished");
		mLeaderboard->draw(true);
		drawLevelFinishedMenu();
	}

	mWindow->draw(*mLapText);

	FadeScreen::render();
}

void Level::pushSprite(sf::Sprite *sprite) {
	mDepthSprites.push_back(sprite);
}

void Level::deleteKarts() {
	delete mPlayerKart.kart;

 	for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
		 aiIter != mAIKarts.end();
		 aiIter++)
		 	delete aiIter->kart;

	mAIKarts.clear();
}

void Level::initKarts() {
	mLeaderboard->clearKarts();

	mPlayerKart = getInitialRacingKart(KartFactory::playerKart(this, mPlayerKartType));
	mPlayerKart.kart->setPosition(mMap->startingPointForRacePosition(mMap->lastRacePosition()));
	mLeaderboard->addKart(&mPlayerKart);

	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Harlock",
											new sf::Color(AI_KART_RGB_BLUE))));
	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Lucky Star",
											new sf::Color(AI_KART_RGB_YELLOW))));
	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Spock",
											new sf::Color(AI_KART_RGB_VIOLET))));
	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Skywalker",
											new sf::Color(AI_KART_RGB_ORANGE))));
	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Daneel",
											new sf::Color(AI_KART_RGB_RED))));
	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Yoda",
											new sf::Color(AI_KART_RGB_AZURE))));
	mAIKarts.push_back(getInitialRacingKart(new AIVenusian(this, "Baley",
											new sf::Color(AI_KART_RGB_BROWN))));


	int racePosition = 1;
    for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++, racePosition ++) {
			mLeaderboard->addKart(&(*aiIter));
			(*aiIter).kart->setPosition(mMap->startingPointForRacePosition(racePosition));
		}

}

bool Level::spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2) {
	return s1->getPosition().y < s2->getPosition().y;
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

    for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
		updateKartLapProgress(*aiIter);
}

void Level::updateKartLapProgress(LevelNS::RacingKart &rk) {
	int sectorCount = mMap->sectorCount();
	rk.currentSector = mMap->sector(rk.kart->position());
	int currentSectorIndex = rk.currentSector.index;
	double lapPercentage = (double) currentSectorIndex / sectorCount;

	// d2("Sector count for kart: ", rk.kart->name(), ": ", sectorCount);
	// d2("Current sector for kart: ", rk.kart->name(), ": ", currentSectorIndex);
	// d2("At position: ", rk.kart->position());
	// d2("Current lap sector percentage for kart: ", rk.kart->name(), ": ", lapPercentage);

	if (MathUtil::isInRange(MathUtil::Range {LAP_Q1_PERCENTAGE, LAP_Q2_PERCENTAGE},
							lapPercentage) &&
		!rk.q1Finished) {
		d(rk.kart->name(), " has finished Q1");
		rk.q1Finished = true;
	}
	else if (MathUtil::isInRange(MathUtil::Range {LAP_Q2_PERCENTAGE, LAP_Q3_PERCENTAGE},
								 lapPercentage) &&
			 !rk.q2Finished &&
			 rk.q1Finished) {
		d(rk.kart->name(), " has finished Q2");
		rk.q2Finished = true;
	}
	else if (MathUtil::isInRange(MathUtil::Range {LAP_Q3_PERCENTAGE, LAP_Q4_PERCENTAGE},
								 lapPercentage) &&
			 !rk.q3Finished &&
			 rk.q1Finished &&
		 	 rk.q2Finished) {
		d(rk.kart->name(), " has finished Q3");
		rk.q3Finished = true;
	}

	if (rk.q1Finished && rk.q2Finished && rk.q3Finished &&
		mMap->tileEvent(rk.kart->position()) == Map::TileEvent::LapFinished) {
			d(rk.kart->name(), " has finished ", rk.currentLap, "° lap");
			rk.q1Finished = rk.q2Finished = rk.q3Finished = false;
			rk.currentLap++;
	}
}

void Level::handleKartsCrash() {
	// Create temp array;
	int kartCount = mAIKarts.size() + 1; // player
	struct CrashableRacingKart {
		LevelNS::RacingKart *rk;
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

	for (int i = 0; i < kartCount - 1; i++) {
		for (int j = i + 1; j < kartCount; j++) {
			if (crk[i].q.intersects(crk[j].q)) {
				d2("Crashing between: ", crk[i].rk->kart->name() , " and ", crk[j].rk->kart->name());
				crk[i].rk->kart->crash(crk[j].rk->kart);
				crk[j].rk->kart->crash(crk[i].rk->kart);
			}
		}
	}
}

void Level::updateMenu(Menu &m, int keyCode) {
	int indexIncrement = 0;
	if (keyCode == sf::Keyboard::Key::Up)
		indexIncrement = 1;
	else if (keyCode == sf::Keyboard::Key::Down)
		indexIncrement = -1;
	else if (keyCode == sf::Keyboard::Key::Return) {
		(this->*(m.options[m.currentOptionIndex].func))();
		m.opened = false;
	}
	m.currentOptionIndex = (m.currentOptionIndex + indexIncrement + m.options.size()) % m.options.size();
}

void Level::drawLevelFinishedMenu() {
	mWindow->draw(*mLevelFinishedMenuFrameSprite);
	drawMenuOptions(mMenuLevelFinished,
					mLevelFinishedMenuFrameSprite->getPosition().x,
					mLevelFinishedMenuFrameSprite->getPosition().y
						- mLevelFinishedMenuFrameSprite->getOrigin().y);
}

void Level::drawMenuOptions(Menu &m, int frameCenterX, int frameTopY) {
	int optionIndex = 0;

    for (std::vector<MenuOption>::iterator optionIter = m.options.begin();
        optionIter != m.options.end();
        optionIter++, optionIndex++) {
		int rowMarginTop =  frameTopY + MENU_PADDING_TOP +
							optionIndex * MENU_MARGIN_BETWEEN_OPTIONS;
		sf::Text *t = (*optionIter).text;
		t->setPosition(frameCenterX, rowMarginTop);

		if (optionIndex != m.currentOptionIndex)
			t->setFillColor(sf::Color(MENU_OPTION_UNSELECTED_RGB));
		else {
			t->setFillColor(sf::Color(MENU_OPTION_SELECTED_RGB, mMenuSelectedOptionCurrentAlpha));
		}

		mWindow->draw(*t);
	}
	mMenuSelectedOptionCurrentAlpha +=
		mMenuSelectedOptionCurrentAlphaIncrementSign * MENU_OPTION_SELECTED_INCREMENT;
	if (mMenuSelectedOptionCurrentAlpha > MENU_OPTION_SELECTED_ALPHA_HIGH ||
		mMenuSelectedOptionCurrentAlpha < MENU_OPTION_SELECTED_ALPHA_LOW) {
		mMenuSelectedOptionCurrentAlphaIncrementSign *= -1;
		mMenuSelectedOptionCurrentAlpha +=
			mMenuSelectedOptionCurrentAlphaIncrementSign * MENU_OPTION_SELECTED_INCREMENT;
	}
}


bool Level::levelFinishedByAKart() {
	if (mPlayerKart.currentLap > RACE_LAP_COUNT)
		return true;

    for (std::vector<LevelNS::RacingKart>::iterator aiIter = mAIKarts.begin();
        aiIter != mAIKarts.end();
        aiIter++)
		if ((*aiIter).currentLap > RACE_LAP_COUNT)
			return true;

	return false;
}

LevelNS::RacingKart Level::getInitialRacingKart(Kart *k) {
	LevelNS::RacingKart rk;
	rk.kart = k;
	rk.currentLap = 0;
	rk.q1Finished = rk.q2Finished = rk.q3Finished = true;
	return rk;
}

void Level::retryFunc() {
	mRaceStartTimer.reset();
	mLevelFinished = false;
	deleteKarts();
	initKarts();
}

void Level::continueFunc() {

}

void Level::quitFunc() {

}

sf::Text * Level::optionText(const char *title) {
	sf::Text *optionText = new sf::Text();
	optionText->setFont(FontFactory::font(FontFactory::FontType::Plain));
	optionText->setCharacterSize(MENU_OPTION_FONT_SIZE);
	optionText->setOutlineThickness(1);
	optionText->setString(title);
	optionText->setOrigin(	optionText->getLocalBounds().width / 2,
							optionText->getLocalBounds().height / 2);
	return optionText;
}

const char * Level::logTag() {
	return LOG_TAG;
}

bool Level::canLog() {
	return CAN_LOG;
}
