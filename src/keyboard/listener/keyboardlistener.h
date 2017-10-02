#ifndef KEYBOARD_LISTENER_H
#define KEYBOARD_LISTENER_H

/**
 * Intarface usable for listen to keys pressed on the keyboard.
 */
class KeyboardListener {
public:
    /**
     * Called when a key is pressed on the keyboard.
     * This method will be called asynchronously when the user
     * press the keyboard, so outside the game loop.
     * @param keyCode the int ASCII code of the pressed key.
     */
    virtual void onKeyPressed(int keyCode) = 0;
};

#endif // KEYBOARD_LISTENER_H