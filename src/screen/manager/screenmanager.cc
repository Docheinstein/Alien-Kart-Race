#include "screenmanager.h"
#include "screen.h"

#define LOG_TAG "{ScreenManager} "
#define CAN_LOG 1

ScreenManager::ScreenManager() {}
ScreenManager::~ScreenManager() {}

void ScreenManager::setScreen(Screen *s) {
    mCurrentScreen = s;
}

void ScreenManager::update() {
    if (mCurrentScreen) {
        mCurrentScreen->update();
        Screen *previousScreen = mCurrentScreen;
        mCurrentScreen = mCurrentScreen->segue();
        if (previousScreen != mCurrentScreen) {
            d("Changing screen and deleting previous one");
            delete previousScreen;
        }
    }
}

void ScreenManager::render() {
    if (mCurrentScreen)
        mCurrentScreen->render();
    else
        w("No screen to render, window will be empty!");
}

const char* ScreenManager::logTag() {
    return LOG_TAG;
}

bool ScreenManager::canLog() {
    return CAN_LOG;
}