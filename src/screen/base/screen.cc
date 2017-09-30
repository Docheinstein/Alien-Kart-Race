#include <SFML/Graphics.hpp>
#include "screen.h"
#include "keyboardmanager.h"

Screen::Screen(sf::RenderWindow *window, KeyboardManager *keysManager) {
    mWindow = window;
    mKeysManager = keysManager;
    mSegue = this;
}

Screen::~Screen() {}

Screen * Screen::segue() {
    return mSegue;
}