#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "fadescreen.h"

namespace sf {
    class Text;
}

/**
 * Launcher screen of the game, shown at the game's startup.
 */
class Launcher : public FadeScreen {
public:
    /**
     * Creates a new launcher screen.
     * @param window      the window this screen is encapsulated in.
     * @param keysManager the optional keyboard manager that will notify this
     *                    screen about async key pressure.
     */
    Launcher(sf::RenderWindow *window, KeyboardManager *keysManager);

    void update();
    void render();

protected:
    void fadeInFinished();
    void fadeOutFinished();

private:
    /** Timer that count the timer elapsed from the shown of the launcher. */
    GameTimer<Launcher> mLauncherShowTimer;

    /** Text containing "Alien Kart". */
    sf::Text    *mAlienKartText;

    /** Text containing "Race". */
    sf::Text    *mRaceText;

    /** Texture for the background of the screen. */
    sf::Texture *mBackgroundTexture;

    /** Sprite for the background of the screen. */
    sf::Sprite  *mBackgroundSprite;

    /**
     * Initialize a new text with the graphics appropriate for this launcher, for the given string.
     * @param  str the string used for the text.
     * @return     a new sf::Text configured appropriately for this launcher screen.
     */
    sf::Text *initializedLauncherText(const char *str);

    /** Starts the fade out effect. */
    void startFadeOut();
};

#endif // LAUNCHER_H