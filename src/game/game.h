#ifndef GAME_H
#define GAME_H

#include "loggerinterface.h"

namespace sf {
	class RenderWindow;
	class Sprite;
}

class KeyboardManager;
class ScreenManager;

class Game : public LoggerInterface {
public:
	Game();
	~Game();
	void start();

	sf::RenderWindow * window();
	KeyboardManager * keysManager();

private:
	sf::RenderWindow *mWindow;
	KeyboardManager *mAsyncKeyboardManager;
	ScreenManager *mScreenManager;

	void init();
	void pollEvents();
	void update();
	void render();

    const char *logTag();
    bool canLog();
};

#endif // GAME_H
