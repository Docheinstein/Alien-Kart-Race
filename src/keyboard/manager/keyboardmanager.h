#ifndef KEYBOARD_MANAGER_H
#define KEYBOARD_MANAGER_H

#include <list>

class KeyboardListener;

class KeyboardManager {
public:
    KeyboardManager();

    void addListener(KeyboardListener *listener);
    void removeListener(KeyboardListener *listener);
    void notifyKeyPressed(int keyCode);
private:
    std::list<KeyboardListener *> mListeners;
};

#endif // KEYBOARD_MANAGER_H