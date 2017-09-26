#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>

class Kart;

class Leaderboard {
public:
    Leaderboard();
    void addKart(Kart *k);

    void update();
    void draw();
private:
    std::vector<Kart *> mKarts;
};

#endif // LEADERBOARD_H