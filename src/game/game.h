#ifndef GAME_H
#define GAME_H

#include "loggerinterface.h"

namespace sf {
	class RenderWindow;
	class Sprite;
}

class KeyboardManager;
class ScreenManager;

/** Principal class responsible for handle the window and the game loop. */
class Game : public LoggerInterface {
public:
	/** Creates and initialize a new game. */
	Game();
	~Game();

	/** Starts the game. */
	void start();

	/**
	 * Returns the window used by the game.
	 * @return the window used by the game.
	 */
	sf::RenderWindow * window();

	/**
	 * Returns the keyboard manager used by the game.
	 * The manager is aware of the keys pressed asynchronously (outside of the gameloop)
	 * and can be used for being notified about those.
	 * @return the keyboard manager used by the game.
	 */
	KeyboardManager * keysManager();

private:
	/** Window in which the game is shown. */
	sf::RenderWindow *mWindow;

	/** Keyboard manager used by the game. */
	KeyboardManager *mAsyncKeyboardManager;

	/** Screen manager used by the game. */
	ScreenManager *mScreenManager;

	/** Initializes the game */
	void init();

	/** Updates the game logic. */
	void update();

	/** Render the game */
	void render();

	/** Handles the incoming events (window closing or key press). */
	void pollEvents();

    const char *logTag();
    bool canLog();
};

#endif // GAME_H
