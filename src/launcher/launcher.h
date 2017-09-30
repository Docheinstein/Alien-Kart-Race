#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "fadescreen.h"

namespace sf {
    class Texture;
    class Sprite;
    class Text;
}

class Launcher : public FadeScreen {
public:
    Launcher(sf::RenderWindow *window, KeyboardManager *keysManager);

    void update();
    void render();

protected:
    void fadeInFinished();
    void fadeOutFinished();
private:
    GameTimer<Launcher> mLauncherShowTimer;

    sf::Text    *mAlienKartText;
    sf::Text    *mRaceText;
    sf::Texture *mBackgroundTexture;
    sf::Sprite  *mBackgroundSprite;

    sf::Text *initializedLauncherText(const char *);
    void startFadeOut();
};

#endif // LAUNCHER_H