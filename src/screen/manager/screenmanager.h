#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

class Game;
class Screen;

class ScreenManager {
public:
    ScreenManager(Game *g);

    void setScreen(Screen *screen);

    void update();
    void render();

private:
    Game *mGame;
    Screen *mCurrentScreen;
};

#endif // SCREEN_MANAGER_H