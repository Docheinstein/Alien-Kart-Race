#ifndef GAME_H
#define GAME_H

#include "loggerinterface.h"

namespace sf {
	class RenderWindow;
	class Event;
	class Sprite;
}

class Level;

class Game : public LoggerInterface {
public:
	static Game & instance();

	static const int WINDOW_WIDTH = 640;//1280;
	static const int WINDOW_HEIGHT = 480;//960;

	void start();
	Level *level();
	sf::RenderWindow * window();



private:
	Game();
	~Game();
	Game(Game const &);
	void operator = (Game const &);

	sf::RenderWindow *mWindow;
	Level *mLevel;

	void init();
	void pollEvents();
	void update();
	void render();
	void handleEvent(const sf::Event &event);

    const char *logTag();
    bool canLog();
};

#endif // GAME_H
