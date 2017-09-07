#ifndef KEY_H
#define KEY_H

enum Key {
	Left,
	Up,
	Right,
	Down
} typedef Key;

const char *keyToStr(const Key key);

#endif
