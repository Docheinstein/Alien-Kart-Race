#include "keyboardhandler.h"
#include <iostream>

KeyboardHandler::KeyboardHandler() {

}

void KeyboardHandler::notifyKeyPressed(const Key key) {
	for (std::list<KeyboardListener*>::const_iterator iter = mListeners.begin();
	 	iter != mListeners.end();
		iter++
	) {
		(*iter)->onKeyPressed(key);
	}
}

void KeyboardHandler::addListener(KeyboardListener * listener) {
	std::cout << "Added listener to keyboard listeners" << std::endl;
	mListeners.push_front(listener);
}
