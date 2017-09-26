#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "gametimer.h"

class Map;
class Minimap;
class Leaderboard;
class PlayerKart;
class AIKart;

namespace sf {
	class Sprite;
	class Text;
	class Texture;
}

class Level : public LoggerInterface {
public:
	Level();
	~Level();

	PlayerKart *playerKart();
	Map *map();
	void update();
	void render();

	void pushSprite(sf::Sprite *sprite);

private:
	PlayerKart *mPlayerKart;
	std::vector<AIKart*> mAIKarts;
	Map *mMap;
	Minimap *mMinimap;
	Leaderboard *mLeaderboard;

	std::vector<sf::Sprite*> mDepthSprites;

	GameTimer<Level> mFadeInTimer;
	GameTimer<Level> mRaceStartTimer;
	sf::Text *mRaceStartTimerText;
	sf::Texture *mOverlayTexture;
	sf::Sprite *mOverlaySprite;

	bool static spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2);
	void renderSpritesForDepth();

    const char *logTag();
    bool canLog();
};

#endif // LEVEL_H
