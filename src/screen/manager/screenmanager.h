#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

class Screen;

/**
 * Responsible for handle the current shown #Screen.
 * Only a screen will be handled by this manager, which will
 * call Screen#update and Screen#render for the current screen appropriately.
 * A screen should return a different segue screen for change the
 * screen handled by this manager.
 * @param g [description]
 */
class ScreenManager {
public:
    /** Creates a new screen manager. */
    ScreenManager();

    /** Sets the screen handled by this screen manager. */
    void setScreen(Screen *screen);

    /** Updates the handled screen (actually calls Screen#update). */
    void update();

    /** Renders the handled screen (actually calls Screen#render). */
    void render();

private:
    /** Screen currently handled by this manager. */
    Screen *mCurrentScreen;
};

#endif // SCREEN_MANAGER_H