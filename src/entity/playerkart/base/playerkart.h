#ifndef PLAYER_KART_H
#define PLAYER_KART_H

#include "game.h"
#include "kart.h"

class PlayerKart : public Kart {
public:
	static const int MARGIN_FROM_BOTTOM = Game::WINDOW_HEIGHT / 12;

    virtual void update();
    virtual void draw();

    int minimapSize();

protected:
	enum PlayerKartSpriteType {
		Left3,
		Left2,
		Left1,
		Center,
		Right1,
		Right2,
		Right3,
		_PlayerKartSpriteTypeCount
	};
};

#endif // PLAYER_KART_H