#include <SFML/Graphics.hpp>
#include "leaderboard.h"
#include "fontfactory.h"
#include "resourceutil.h"
#include "kart.h"
#include "level.h"
#include "const.h"

#include <cstring>

#define LOG_TAG "{Leaderboard} ";
#define CAN_LOG 0

#define RANKING_HEADER "Ranking"

const float LEADERBOARD_MARGIN_LEFT = Const::WINDOW_WIDTH / 18;
const float LEADERBOARD_MARGIN_TOP = Const::WINDOW_HEIGHT / 12;
const float LEADERBOARD_FONT_SIZE = Const::WINDOW_HEIGHT / 24;
const float LEADERBOARD_MARGIN_BETWEEN_KARTS = Const::WINDOW_HEIGHT / 16;
const float LEADERBOARD_MARGIN_FROM_FACE_TO_TEXT = Const::WINDOW_WIDTH / 18;

const float LEADERBOARD_RANKING_PADDING_TOP = Const::WINDOW_HEIGHT / 14;
const float LEADERBOARD_RANKING_PADDING_LEFT = Const::WINDOW_HEIGHT / 24;

const float LEADERBOARD_RANKING_HEADER_MARGIN_TOP = Const::WINDOW_HEIGHT / 28;
const float LEADERBOARD_RANKING_FONT_SIZE = Const::WINDOW_HEIGHT / 18;
const float LEADERBOARD_RANKING_FRAME_SIZE = 12 * Const::WINDOW_WIDTH / 640.0;

Leaderboard::Leaderboard(sf::RenderWindow *window) {
	mWindow = window;
	mRankingFrameTexture = new sf::Texture();
	mRankingFrameTexture->loadFromFile(ResourceUtil::image("ranking_frame.png").c_str());


	mRankingFrameSprite = new sf::Sprite();
	mRankingFrameSprite->setTexture(*mRankingFrameTexture);

	mRankingFrameSprite->setScale(Const::WINDOW_WIDTH / 640.0, Const::WINDOW_HEIGHT / 480.0);
	mRankingFrameSprite->setPosition(
		Const::WINDOW_WIDTH / 4 - mRankingFrameSprite->getLocalBounds().width / 2,
		Const::WINDOW_HEIGHT / 2 - mRankingFrameSprite->getLocalBounds().height / 2
	);


	mRankingHeaderText = new sf::Text();

	mRankingHeaderText->setFont(FontFactory::font(FontFactory::FontType::Plain));
	mRankingHeaderText->setString(RANKING_HEADER);
	mRankingHeaderText->setCharacterSize(LEADERBOARD_RANKING_FONT_SIZE);
	mRankingHeaderText->setFillColor(sf::Color(145, 106, 30, 255));
	mRankingHeaderText->setOutlineColor(sf::Color::Black);
	mRankingHeaderText->setOutlineThickness(1);

	mRankingHeaderText->setOrigin(
		mRankingHeaderText->getLocalBounds().width / 2,
		0
	);

	mRankingHeaderText->setPosition(
		Const::WINDOW_WIDTH / 4,
		mRankingFrameSprite->getPosition().y + LEADERBOARD_RANKING_HEADER_MARGIN_TOP
	);
}

Leaderboard::~Leaderboard() {
	delete mRankingFrameSprite;
	delete mRankingFrameTexture;
	delete mRankingHeaderText;
}

void Leaderboard::addKart(LevelNS::RacingKart *rk) {
	mKarts.push_back(LeaderboardKart {rk, kartText(rk)});
}

void Leaderboard::clearKarts() {
	mKarts.clear();
}

void Leaderboard::update() {
	int kartPosition = 1;
    for (std::vector<LeaderboardKart>::iterator lkIter = mKarts.begin();
        lkIter != mKarts.end();
        lkIter++, kartPosition++) {
		d2(kartPosition, "° ", (*lkIter).rk->kart->name(),
			"; Lap: ", (*lkIter).rk->currentLap,
			"; Sector: ", (*lkIter).rk->currentSector.index);
	}
	d2("---------");
	std::sort(mKarts.begin(), mKarts.end(), racingKartCompareFunction);
}

void Leaderboard::draw(bool levelRunning) {
	int xOffset = 0;
	int yOffset = 0;
	if (!levelRunning) {
		xOffset = mRankingFrameSprite->getPosition().x + LEADERBOARD_RANKING_PADDING_LEFT;
		yOffset = mRankingFrameSprite->getPosition().y + LEADERBOARD_RANKING_PADDING_TOP;
        mWindow->draw(*mRankingFrameSprite);
		mWindow->draw(*mRankingHeaderText);
	}

    int kartPosition = 1;
    for (std::vector<LeaderboardKart>::iterator lkIter = mKarts.begin();
        lkIter != mKarts.end();
        lkIter++, kartPosition++) {
		int rowMarginTop =  LEADERBOARD_MARGIN_TOP + yOffset +
							(kartPosition - 1) * LEADERBOARD_MARGIN_BETWEEN_KARTS;
		int rowMarginLeft = LEADERBOARD_MARGIN_LEFT + xOffset;

		// Ensure that the row is within the frame
		if (!levelRunning && rowMarginTop + LEADERBOARD_FONT_SIZE >=
				mRankingFrameSprite->getPosition().y +
					mRankingFrameSprite->getGlobalBounds().height -
					LEADERBOARD_RANKING_FRAME_SIZE)
			continue;

		// Face on the Left
		sf::Sprite *kartFace = (*lkIter).rk->kart->leadeboardSprite();
		kartFace->setPosition(rowMarginLeft, rowMarginTop - 2);
		kartFace->setScale(Const::WINDOW_WIDTH / 640.0, Const::WINDOW_HEIGHT / 480.0);

		std::string leaderboardNameStr = std::to_string(kartPosition) + ". ";
		leaderboardNameStr += (*lkIter).rk->kart->name();

		// Kart name on the right
		sf::Text *kartText = (*lkIter).leaderboardNameText;
		kartText->setString(leaderboardNameStr);
		kartText->setPosition(
			rowMarginLeft + LEADERBOARD_MARGIN_FROM_FACE_TO_TEXT,
			rowMarginTop
		);


        mWindow->draw(*kartFace);
        mWindow->draw(*kartText);
    }
}

sf::Text *Leaderboard::kartText(LevelNS::RacingKart *rk) {
	sf::Text *kartText = new sf::Text();
	kartText->setFont((FontFactory::font(FontFactory::FontType::Plain)));
	kartText->setCharacterSize(LEADERBOARD_FONT_SIZE);
	kartText->setFillColor(rk->kart->color());
	kartText->setOutlineColor(sf::Color::Black);
	kartText->setOutlineThickness(1);

	return kartText;
}

bool Leaderboard::racingKartCompareFunction(LeaderboardKart &lk1, LeaderboardKart &lk2) {
	LevelNS::RacingKart *k1 = lk1.rk;
	LevelNS::RacingKart *k2 = lk2.rk;

	// bool verbose = (strcmp(k1->name(), "Me") == 0 || strcmp(k1->name(), "Me") == 0)

	// Check if a kart is ahead of a lap
	if (k1->currentLap != k2->currentLap) {
		return k1->currentLap > k2->currentLap;
	}

	// Check if a kart is ahead of a sector
	if (k1->currentSector.index != k2->currentSector.index)
		return k1->currentSector.index > k2->currentSector.index;

	// Check within the same sector

	switch(k1->currentSector.type /* same as k2->currentSector.type */) {
		case Map::SectorDirectionType::UpLeft:
			if (k1->kart->position().y != k2->kart->position().y)
				return k1->kart->position().y < k2->kart->position().y;
			return k1->kart->position().x < k2->kart->position().x;
		case Map::SectorDirectionType::UpRight:
			if (k1->kart->position().y != k2->kart->position().y)
				return k1->kart->position().y < k2->kart->position().y;
			return k1->kart->position().x > k2->kart->position().x;

		case Map::SectorDirectionType::RightUp:
			if (k1->kart->position().x != k2->kart->position().x)
				return k1->kart->position().x > k2->kart->position().x;
			return k1->kart->position().y < k2->kart->position().y;
		case Map::SectorDirectionType::RightDown:
			if (k1->kart->position().x != k2->kart->position().x)
				return k1->kart->position().x > k2->kart->position().x;
			return k1->kart->position().y > k2->kart->position().y;

		case Map::SectorDirectionType::DownLeft:
			if (k1->kart->position().y != k2->kart->position().y)
				return k1->kart->position().y > k2->kart->position().y;
			return k1->kart->position().x < k2->kart->position().x;
		case Map::SectorDirectionType::DownRight:
			if (k1->kart->position().y != k2->kart->position().y)
				return k1->kart->position().y > k2->kart->position().y;
			return k1->kart->position().x > k2->kart->position().x;

		case Map::SectorDirectionType::LeftUp:
			if (k1->kart->position().x != k2->kart->position().x)
				return k1->kart->position().x < k2->kart->position().x;
			return k1->kart->position().y < k2->kart->position().y;
		case Map::SectorDirectionType::LeftDown:
			if (k1->kart->position().x != k2->kart->position().x)
				return k1->kart->position().x < k2->kart->position().x;
			return k1->kart->position().y > k2->kart->position().y;
	}
	return false;
}

bool Leaderboard::canLog() {
	return CAN_LOG;
}

const char * Leaderboard::logTag() {
	return LOG_TAG;
}
