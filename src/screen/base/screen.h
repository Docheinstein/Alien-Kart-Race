#ifndef SCREEN_H
#define SCREEN_H

#include <cstddef>

namespace sf {
    class RenderWindow;
}

class KeyboardManager;

/**
 * Base class representing a screen of the game.
 * A screen is associated with a single function of the game; so it
 * can be a level, a menu, a setting screen...
 */
class Screen {
public:

    /**
     * Creates a new screen.
     * @param window      the window this screen is encapsulated in.
     * @param keysManager the optional keyboard manager that will notify this
     *                    screen about async key pressure.
     */
    Screen(sf::RenderWindow *window, KeyboardManager *keysManager = NULL);
	virtual ~Screen();

    /**
     * Updates the logic of the screen.
     * This method should not be used for draw on the window, instead use #render
     */
    virtual void update() = 0;

    /**
     * Draws the screen on the window.
     */
    virtual void render() = 0;

    /**
     * Returns the screen that should be shown on the window.
     * for the next updates.
     * The default return value is the screen itself.
     * Entities inheriting from this base classe should change the returned
     * segue for starts a new screen.
     * @return the screen that should segue this screen.
     */
    Screen *segue();

protected:
    /** Window this screen is encapsulated in. */
    sf::RenderWindow *mWindow;

    /** Keyboard manager that will notify this screen about async key pressure. */
    KeyboardManager *mKeysManager;

    /** Screen that will segue this screen for next update. */
    Screen *mSegue;
};

#endif // SCREEN_H