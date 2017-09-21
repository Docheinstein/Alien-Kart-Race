#ifndef LEVEL_H
#define LEVEL_H

class Map;
class Minimap;
class PlayerKart;
class AIKart;

class Level {
public:
	Level();
	~Level();

	PlayerKart *playerKart();
	Map *map();
	void update();
	void render();

private:
	const int AI_KART_COUNT = 1;

	PlayerKart *mPlayerKart;
	AIKart *mAIKarts;
	Map *mMap;
	Minimap *mMinimap;
};

#endif // LEVEL_H
