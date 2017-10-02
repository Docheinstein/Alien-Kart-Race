#ifndef KEYBOARD_MANAGER_H
#define KEYBOARD_MANAGER_H

#include <list>

class KeyboardListener;

/**
 * Responsible for handle the pressure of the keys by notifying
 * listeners about new pressed key.
 */
class KeyboardManager {
public:
    /** Creates a new keyboard manager. */
    KeyboardManager();

    /**
     * Adds a listener that will be notified about keys pressure.
     * @param listener the listener that will be notified when a key is pressed.
     */
    void addListener(KeyboardListener *listener);

    /**
     * Removes a listener from the listeners set.
     * @param listener the listener that won't be notified anymore about pressed keys.
     */
    void removeListener(KeyboardListener *listener);

    /**
     * Notifies this manager that a new key has been pressed.
     * This manager is not active since doesn't poll the key events from the
     * system, so the game is reponsible for call this method when a new
     * key event has been polled.
     * @param keyCode the int code of the pressed key.
     */
    void notifyKeyPressed(int keyCode);

private:
    /** Contains the listeners that will be notified about new pressed keys. */
    std::list<KeyboardListener *> mListeners;
};

#endif // KEYBOARD_MANAGER_H