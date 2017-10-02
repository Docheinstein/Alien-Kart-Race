#include "screenmanager.h"
#include "screen.h"

ScreenManager::ScreenManager() {
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