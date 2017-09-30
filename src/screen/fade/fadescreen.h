#ifndef FADE_SCREEN_H
#define FADE_SCREEN_H

#define FADE_DEFAULT_DURATION 800

#include "screen.h"
#include "gametimer.h"

namespace sf {
    class Texture;
    class Sprite;
}

class FadeScreen : public Screen {
public:
    FadeScreen( sf::RenderWindow *window,
                KeyboardManager *keysManager = NULL,
                int fadeInDuration = FADE_DEFAULT_DURATION,
                int fadeOutDuration = FADE_DEFAULT_DURATION);
    ~FadeScreen();

    void update();
    void render();

    void fadeOut();

protected:
    GameTimer<FadeScreen> mFadeInTimer;
    GameTimer<FadeScreen> mFadeOutTimer;

    int mFadeInDuration;
    int mFadeOutDuration;

    bool isFadingIn();
    bool isFadingOut();
    bool isFading();

    virtual void fadeInFinished();
    virtual void fadeOutFinished();

private:
	sf::Texture    *mOverlayTexture;
	sf::Sprite     *mOverlaySprite;
};

#endif // FADE_SCREEN_H