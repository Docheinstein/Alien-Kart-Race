#ifndef GAME_H
#define GAME_H

namespace sf {
	class RenderWindow;
	class Event;
}

class Level;

class Game {
public:
	static Game & instance();
	static const int WINDOW_WIDTH = 320;
	static const int WINDOW_HEIGHT = 240;

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
};

#endif // GAME_H
