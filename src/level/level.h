#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "map.h"
#include "fadescreen.h"
#include "keyboardlistener.h"
#include "kartfactory.h"
#include "mapfactory.h"

class Minimap;
class Leaderboard;
class Kart;
class PlayerKart;

namespace sf {
	class Sprite;
	class Text;
	class Texture;
}

namespace LevelNS {
	struct RacingKart {
		Kart *kart;
		Map::Sector currentSector; // Not needed, just for cache reason
		int currentLap;
		bool q1Finished, q2Finished, q3Finished;
	} typedef RacingKart;
}

class Level : public FadeScreen, public KeyboardListener, public LoggerInterface {
public:
	Level(	sf::RenderWindow *window, KeyboardManager *keysManager,
			KartFactory::KartType kartType,
			MapFactory::MapType mapType);
	~Level();

	void update();
	void render();

	Kart *playerKart();
	Map *map();

	void pushSprite(sf::Sprite *sprite);

	void onKeyPressed(int keyCode);

private:
	struct MenuOption {
		sf::Text *text;
 		void (Level::*func)();
	} typedef MenuOption;

	struct Menu {
		std::vector<MenuOption> options;
		int currentOptionIndex;
		bool opened;
	} typedef Menu;

	MenuOption mOptionContinue;
  	MenuOption mOptionRetry;
  	MenuOption mOptionQuit;

	Menu mMenuInGame;
	Menu mMenuLevelFinished;

	KartFactory::KartType mPlayerKartType;
	MapFactory::MapType   mMapType;

	LevelNS::RacingKart mPlayerKart;
	std::vector<LevelNS::RacingKart> mAIKarts;

	Map *mMap;
	Minimap *mMinimap;
	Leaderboard *mLeaderboard;

	GameTimer<Level> mRaceStartTimer;
	sf::Text *mRaceStartTimerText;

	sf::Text *mLapText;

	sf::Texture *mOverlayTexture;
	sf::Sprite *mOverlaySprite;

	sf::Texture *mLevelFinishedMenuFrameTexture;
	sf::Sprite *mLevelFinishedMenuFrameSprite;

	std::vector<sf::Sprite*> mDepthSprites;

	bool mLevelFinished;

	int mMenuSelectedOptionCurrentAlpha;
	int mMenuSelectedOptionCurrentAlphaIncrementSign;

	static Menu inGameMenu();
	static Menu levelFinishedMenu();

	static LevelNS::RacingKart getInitialRacingKart(Kart *k);
	static sf::Text *optionText(const char *title);

	static bool spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2);

	void deleteKarts();
	void initKarts();

	void renderSpritesForDepth();

	void updateKartsLapProgress();
	void updateKartLapProgress(LevelNS::RacingKart &racingKart);

	void handleKartsCrash();

	void drawMenuOptions(Menu &m, int frameCenterX, int frameTopY);
	void drawLevelFinishedMenu();

	bool levelFinishedByAKart();

	void updateMenu(Menu &m, int keyCode);
	
	void retryFunc();
	void continueFunc();
	void quitFunc();

    const char *logTag();
    bool canLog();
};

#endif // LEVEL_H
