#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <list>
#include "keyboardlistener.h"
#include "key.h"

class KeyboardHandler {
public:
	KeyboardHandler();
	void notifyKeyPressed(Key key);
	void addListener(KeyboardListener * listener);

private:
	std::list<KeyboardListener *> mListeners;
};

#endif // KEYBOARD_HANDLER_H
