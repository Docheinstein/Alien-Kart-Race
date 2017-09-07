#ifndef KEYBOARD_UTIL_H
#define KEYBOARD_UTIL_H

namespace sf {
	sf::Keyboard::Key
}

class KeyboardUtil {
public:
	enum Key {
		Left,
		Up,
		Right,
		Down
	};
	static bool isKeyDown(Key key);
}

#endif
