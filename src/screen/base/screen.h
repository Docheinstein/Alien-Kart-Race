#ifndef SCREEN_H
#define SCREEN_H

namespace sf {
    class RenderWindow;
}

class KeyboardManager;

class Screen {
public:
    Screen(sf::RenderWindow *window, KeyboardManager *keysManager = NULL);
	virtual ~Screen();

    virtual void update() = 0;
    virtual void render() = 0;
    Screen *segue();

protected:
    sf::RenderWindow *mWindow;
    KeyboardManager *mKeysManager;
    Screen *mSegue;
};

#endif // SCREEN_H