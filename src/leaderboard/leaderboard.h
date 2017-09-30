#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include "loggerinterface.h"

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
}

namespace LevelNS {
    class RacingKart;
}

class Leaderboard : public LoggerInterface {
public:
    Leaderboard(sf::RenderWindow *window);
    ~Leaderboard();

    void addKart(LevelNS::RacingKart *rk);
    void clearKarts();

    void update();
    void draw(bool levelFinished);
private:
    struct LeaderboardKart {
        LevelNS::RacingKart *rk;
        sf::Text *leaderboardNameText;
    } typedef LeaderboardKart;

    static sf::Text *kartText(LevelNS::RacingKart *rk);
    static bool racingKartCompareFunction(LeaderboardKart &lk1, LeaderboardKart &lk2);

    sf::RenderWindow *mWindow;

    std::vector<LeaderboardKart> mKarts;

    sf::Texture *mRankingFrameTexture;
    sf::Sprite  *mRankingFrameSprite;
    sf::Text *mRankingHeaderText;

    bool canLog();
    const char *logTag();
};

#endif // LEADERBOARD_H