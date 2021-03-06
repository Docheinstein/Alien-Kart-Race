#include "keyboardmanager.h"
#include "keyboardlistener.h"

KeyboardManager::KeyboardManager() {}

// ------------------------
// PUBLIC -----------------
// ------------------------

void KeyboardManager::addListener(KeyboardListener *l) {
    mListeners.push_back(l);
}

void KeyboardManager::removeListener(KeyboardListener *l) {
    mListeners.remove(l);
}

void KeyboardManager::notifyKeyPressed(int keyCode) {
    for (std::list<KeyboardListener *>::iterator l = mListeners.begin();
        l != mListeners.end();
        l++)
        (*l)->onKeyPressed(keyCode);
}