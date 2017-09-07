#ifndef LEVEL_H
#define LEVEL_H

class Map;
class Kart;

class Level {
public:
	Level();
	~Level();

	Kart *kart();
	Map *map();
	void update();
	void render();

private:
	Kart *mKart;
	Map *mMap;
};

#endif // LEVEL_H
