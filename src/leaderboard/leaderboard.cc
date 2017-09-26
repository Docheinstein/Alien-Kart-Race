#include <SFML/Graphics.hpp>
#include "leaderboard.h"
#include "fontfactory.h"
#include "kart.h"

const int LEADERBOARD_MARGIN_LEFT = Game::WINDOW_WIDTH / 16;
const int LEADERBOARD_MARGIN_TOP = Game::WINDOW_HEIGHT / 12;
const int LEADERBOARD_FONT_SIZE = Game::WINDOW_HEIGHT / 36;
const int LEADERBOARD_MARGIN_BETWEEN_KARTS = LEADERBOARD_MARGIN_TOP;

Leaderboard::Leaderboard() {
}

void Leaderboard::addKart(Kart *k) {
	mKarts.push_back(k);
}

void Leaderboard::update() {

}

void Leaderboard::draw() {
    int kartPosition = 1;
    for (std::vector<Kart *>::iterator kIter = mKarts.begin();
        kIter != mKarts.end();
        kIter++, kartPosition++) {
        	sf::Text kartName;
        	kartName.setFont((FontFactory::font(FontFactory::FontType::Main)));
        	kartName.setCharacterSize(LEADERBOARD_FONT_SIZE);
        	kartName.setFillColor(sf::Color::Black);
        	// mRaceStartTimerText->setOutlineColor(sf::Color::Black);
        	// mRaceStartTimerText->setOutlineThickness((float)Game::WINDOW_HEIGHT / 120);
            kartName.setString((*kIter)->name());
            kartName.setPosition(
                LEADERBOARD_MARGIN_LEFT,
                LEADERBOARD_MARGIN_TOP + (kartPosition - 1) * LEADERBOARD_MARGIN_BETWEEN_KARTS);
            Game::instance().window()->draw(kartName);
        }
}