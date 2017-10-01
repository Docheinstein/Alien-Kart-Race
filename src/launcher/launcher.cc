#include <SFML/Graphics.hpp>
#include "keyboardmanager.h"
#include "launcher.h"
#include "const.h"
#include "fontfactory.h"
#include "levelpicker.h"

#define LAUNCHER_FADE_IN_DURATION 1200
#define LAUNCHER_FADE_OUT_DURATION 1200

#define LAUNCHER_SHOW_DURATION 2000 /*2000*/

#define BACKGROUND_COLOR_RGB     26,    28,     28
#define GAME_TITLE_COLOR_RGB     178,   22,     22

const float GAME_TITLE_FONT_SIZE = Const::WINDOW_HEIGHT / 6;

Launcher::Launcher(sf::RenderWindow *w, KeyboardManager *keysManager)
    : FadeScreen(w, keysManager, LAUNCHER_FADE_IN_DURATION, LAUNCHER_FADE_OUT_DURATION) {
    // Background
    sf::Image backgroundImage;
    backgroundImage.create( Const::WINDOW_WIDTH, Const::WINDOW_HEIGHT,
                            sf::Color(BACKGROUND_COLOR_RGB));

    mBackgroundTexture = new sf::Texture();
    mBackgroundTexture->loadFromImage(backgroundImage);

    mBackgroundSprite = new sf::Sprite();
    mBackgroundSprite->setTexture(*mBackgroundTexture);

    // Game title
    mAlienKartText = initializedLauncherText("Alien Kart");
    mRaceText = initializedLauncherText("Race");

    mAlienKartText->setPosition(
        Const::WINDOW_WIDTH / 2,
        Const::WINDOW_HEIGHT / 2 - mAlienKartText->getGlobalBounds().height * 3 / 4
    );

    mRaceText->setPosition(
        Const::WINDOW_WIDTH / 2,
        Const::WINDOW_HEIGHT / 2 + mRaceText->getGlobalBounds().height * 3 / 4
    );

	mLauncherShowTimer.initialize(LAUNCHER_SHOW_DURATION, this, &Launcher::startFadeOut);
}

void Launcher::update() {
    FadeScreen::update();

    mLauncherShowTimer.update();
}

void Launcher::render() {
    mWindow->draw(*mBackgroundSprite);
    mWindow->draw(*mAlienKartText);
    mWindow->draw(*mRaceText);

    FadeScreen::render();
}

sf::Text * Launcher::initializedLauncherText(const char *str) {
    sf::Text *text = new sf::Text();
	text->setFont((FontFactory::font(FontFactory::FontType::Fancy)));
	text->setCharacterSize(GAME_TITLE_FONT_SIZE);
	text->setFillColor(sf::Color(GAME_TITLE_COLOR_RGB));
	text->setOutlineColor(sf::Color::White);
	text->setOutlineThickness((float)Const::WINDOW_HEIGHT / 120) ;

    text->setString(str);

	text->setOrigin(
        text->getLocalBounds().width / 2,
        text->getLocalBounds().height / 2
    );

    return text;
}

void Launcher::startFadeOut() {
    fadeOut();
}

void Launcher::fadeInFinished() {
    mLauncherShowTimer.start();
}

void Launcher::fadeOutFinished() {
    mSegue = new LevelPicker(mWindow, mKeysManager);
}
