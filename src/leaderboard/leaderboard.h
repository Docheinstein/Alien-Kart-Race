#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include "loggerinterface.h"

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

class RacingKart;

/**
 * Responsible for show the current ranking of the karts in a map.
 * This is reponsible for show the leadeboard either while the
 * level is running or is finished.
 */
class Leaderboard : public LoggerInterface {
public:
    /**
     * Creates a leaderboard.
     * @param window the window used for render the leaderboard.
     */
    Leaderboard(sf::RenderWindow *window);
    ~Leaderboard();

    /**
     * Adds a kart to the collection of karts considered in the leaderboard.
     * @param rk the racing kart to add to the leaderboard.
     */
    void addKart(RacingKart *rk);

    /** Removes every karts from the collection of karts in the leaderboard. */
    void clearKarts();

    /**
     * Updates the leaderboard.
     * This method just sort the collection of karts, but the
     * current lap and the current sector of the karts should have
     * already been defined before call this.
     */
    void update();

    /**
     * Draws the leaderboard on the window.
     * This can draw the leaderboard either for level running or level finished.
     * If the level is running, the leaderboard will be drawn as floating on the left;
     * if the level is finished, it will be drawn within the ranking frame.
     * @param levelRunning whether the lever is still running.
     */
    void draw(bool levelRunning);

private:
    /**
     * Wrapper for the racing kart that contains the
     * sf::Text representing the name of the kart.
     */
    struct LeaderboardKart {
        /** Underlying racing kart. */
        RacingKart *rk;
        /** Name of the kart. */
        sf::Text *leaderboardNameText;
    } typedef LeaderboardKart;

    /** Window the leaderboard will draw to. */
    sf::RenderWindow *mWindow;

    /** Contains the karts handled by this leaderboard (should be every kart of the level). */
    std::vector<LeaderboardKart> mKarts;

    /** Texture of the ranking frame. */
    sf::Texture *mRankingFrameTexture;

    /** Sprite of the ranking frame. */
    sf::Sprite  *mRankingFrameSprite;

    /** Text of the header of the ranking frame. */
    sf::Text *mRankingHeaderText;

    /**
     * Initialize a new text with the graphics appropriate for this leaderboard, for the given kart.
     * @param  rk the racing kart the name will be taken from.
     * @return    a new sf::Text configured appropriately for this leaderboard.
     */
    static sf::Text *initializedKartText(RacingKart *rk);

    /**
     * Compares the karts and returns true if the first is ahead the second.
     * @param  lk1 a leaderboard racing kart.
     * @param  lk2 a leaderboard racing kart.
     * @return     whether the first kart is ahead the second one.
     */
    static bool racingKartCompareFunction(LeaderboardKart &lk1, LeaderboardKart &lk2);

    bool canLog();
    const char *logTag();
};

#endif // LEADERBOARD_H