#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class Map;
class Minimap;
class PlayerKart;
class AIKart;

namespace sf {
	class Sprite;
}

class Level {
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

	std::vector<sf::Sprite*> mDepthSprites;

	bool static spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2);
	void renderSpritesForDepth();
};

#endif // LEVEL_H
