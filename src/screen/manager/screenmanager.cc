#include "screenmanager.h"
#include "game.h"
#include "screen.h"
#include <iostream>

ScreenManager::ScreenManager(Game *g) {
    mGame = g;
}

void ScreenManager::setScreen(Screen *s) {
    mCurrentScreen = s;
}

void ScreenManager::update() {
    if (mCurrentScreen) {
        mCurrentScreen->update();
        Screen *previousScreen = mCurrentScreen;
        mCurrentScreen = mCurrentScreen->segue();
        if (previousScreen != mCurrentScreen)
            delete previousScreen;
    }
}

void ScreenManager::render() {
    if (mCurrentScreen)
        mCurrentScreen->render();
}