#ifndef KEYBOARD_LISTENER_H
#define KEYBOARD_LISTENER_H

class KeyboardListener {
public:
    virtual void onKeyPressed(int keyCode) = 0;
};

#endif // KEYBOARD_LISTENER_H