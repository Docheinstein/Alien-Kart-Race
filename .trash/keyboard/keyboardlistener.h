#ifndef KEYBOARD_LISTENER_H
#define KEYBOARD_LISTENER_H

#include "key.h"

class KeyboardListener {
public:
	virtual ~KeyboardListener() {}
	virtual void onKeyPressed(const Key key) = 0;

private:
};

#endif // KEYBOARD_LISTENER_H
