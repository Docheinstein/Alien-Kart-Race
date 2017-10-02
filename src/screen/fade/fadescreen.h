#ifndef FADE_SCREEN_H
#define FADE_SCREEN_H

#define FADE_DEFAULT_DURATION 800

#include "screen.h"
#include "gametimer.h"

namespace sf {
    class Texture;
    class Sprite;
}

/**
 * Base class representing a screen of the game that has the
 * feauture of fade in and fade out with a vanish effect of a
 * certain duration.
 */
class FadeScreen : public Screen {
public:

    /**
     * Creates a new fade screen.
     * @param window      the window this screen is encapsulated in.
     * @param keysManager the optional keyboard manager that will notify this
     *                    screen about async key pressure.
     * @param fadeInDuration  the time in millis representing the duration
     *                        of the fade in effect.
     *                        The default value is 800 millis.
     * @param fadeOutDuration the time in millis representing the duration
     *                        of the fade out effect.
     *                        The default value is 800 millis.
     */
    FadeScreen( sf::RenderWindow *window,
                KeyboardManager *keysManager = NULL,
                int fadeInDuration = FADE_DEFAULT_DURATION,
                int fadeOutDuration = FADE_DEFAULT_DURATION);
    ~FadeScreen();

    void update();
    void render();

    /** Starts a fade in effect. */
    void fadeIn();

    /** Starts a fade out effect. */
    void fadeOut();

protected:
    /** Timer counting the time elapsed from the beginning of the fade in effect. */
    GameTimer<FadeScreen> mFadeInTimer;

    /** Timer counting the time elapsed from the beginning of the fade out effect. */
    GameTimer<FadeScreen> mFadeOutTimer;

    /** Duration of the fade in effect in millis. */
    int mFadeInDuration;

    /** Duration of the fade out effect in millis. */
    int mFadeOutDuration;

    /**
     * Returns whether the screen is fading in.
     * @return whether the screen is fading in.
     */
    bool isFadingIn();

    /**
     * Returns whether the screen is fading out.
     * @return whether the screen is fading out.
     */
    bool isFadingOut();

    /**
     * Returns whether the screen is fading, either in or out.
     * @return whether the screen is fading, either in or out.
     */
    bool isFading();

    /**
     * This method will be called when the screen finished the fade in effect.
     * Entities inheriting from this class can override this method
     * for perform tasks after the completation of the fade effect
     * (i.e. starting the logic of the screen).
     */
    virtual void fadeInFinished();

    /**
     * This method will be called when the screen finished the fade out effect.
     * Entities inheriting from this class can override this method
     * for perform tasks after the completation of the fade effect
     * (i.e. change the segue of this screen to another one).
     */
    virtual void fadeOutFinished();

private:
    /** Texture of the overlay that covers the entire screen during a fade effect. */
	sf::Texture    *mOverlayTexture;

    /** Sprite of the overlay that covers the entire screen during a fade effect. */
	sf::Sprite     *mOverlaySprite;
};

#endif // FADE_SCREEN_H