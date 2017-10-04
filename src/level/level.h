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
	class Text;
}

/**
 * Wrapper for a kart that contains information about the lap,
 * the current sector and finished quarters.
 * Keeping information about the quarters is needed for ensure that
 * the karts are finishing laps in the right direction; for finish
 * a lap a kart should have finished q1, q2 and q3 in order.
 */
struct RacingKart {
	/** Wrapped kart. */
	Kart *kart;

	/**
	 * Current sector of the kart.
	 * This is stored for cache reason in order to avoid to recalculate it every time
	 * is needed (i.e. the leadeboard will use sector information for sort the karts).
	 */
	Map::Sector currentSector;

	/** Current lap of the kart within this level. */
	int currentLap;

	/** Whether 1/4 of the map has been covered by the wrapped kart. */
	bool q1Finished;

	/** Whether 2/4 of the map has been covered by the wrapped kart. */
	bool q2Finished;

	/** Whether 3/4 of the map has been covered by the wrapped kart. */
	bool q3Finished;
} typedef RacingKart;

/** Handles the karts, the map and eventually every other object in the level. */
class Level : public FadeScreen, public KeyboardListener, public LoggerInterface {
public:

	/**
	* Creates a new level for the given player kart and map.
	* @param window      		the window this level is encapsulated in.
	* @param keysManager 		the keyboard manager that will notify this level about
	*                      		async key pressure (needed for menu).
	* @param playerKartType    the type of the kart for the player.
	* @param mapType     		the type of the map used for this level.
	*/
	Level(	sf::RenderWindow *window, KeyboardManager *keysManager,
			KartFactory::KartType playerKartType,
			MapFactory::MapType mapType);
	~Level();

	void update();
	void render();
	void onKeyPressed(int keyCode);

	/**
	 * Returns the kart of the active player.
	 * @return the player kart.
	 */
	Kart *playerKart();

	/**
	 * Returns the map used for this level.
	 * @return the map of this level.
	 */
	Map *map();

	/**
	 * Push a sprite to the collection of sprite to render in the next #render call.
	 * This method should be used by every entity that need to render on the screen
	 * during the level.
	 * This is needed since the sprite will be rendered at once after a sort
	 * by "z-axis", in order to appropriately place object in the screen and avoid
	 * to wrap near objects with distant objects.
	 * @param sprite the sprite to render the next #render call
	 */
	void pushSprite(sf::Sprite *sprite);

protected:
	void fadeInFinished();
	void fadeOutFinished();

private:
	/** Option of a menu. */
	struct MenuOption {
		/** Text used for render this option. */
		sf::Text *text;

		/** Function that will be called when the option is selected by the player. */
 		void (Level::*func)();
	} typedef MenuOption;

	/** Represents a level menu. */
	struct Menu {
		/** The options contained in this menu. */
		std::vector<MenuOption> options;

		/** The index of the currently selected option. */
		int currentOptionIndex;

		/** Whether this menu is opened and shown. */
		bool opened;
	} typedef Menu;

	/** State of this level. */
	enum LevelState {
		/**
		 * The race is running.
		 * The level will have the running state even if the screen is
		 * fading in, or it is in pause (since the race is still running even
		 * if it is not actually going).
		 */
		Running,

		/** The race is finished (a kart completed it). */
		Finished,

		/** The player chose to retry the level. */
		Retrying,

		/** The player chose to quit the level. */
		Quitting
	};

	/** Continue option. */
	MenuOption mOptionContinue;

	/** Retry option. */
  	MenuOption mOptionRetry;

	/** Quit option. */
  	MenuOption mOptionQuit;

	/** Menu shown during the game (contains containue option). */
	Menu mMenuInGame;

	/** menu shown out of the game (doesn't contain continue option). */
	Menu mMenuLevelFinished;

	/** Current state of the level. */
 	LevelState mLevelState;

	/** Type of the player kart. */
	KartFactory::KartType mPlayerKartType;

	/** Type of map used for this level. */
	MapFactory::MapType   mMapType;

	/** Kart of the active player. */
	RacingKart mPlayerKart;

	/** Karts of the CPU. */
	std::vector<RacingKart> mAIKarts;

	/** Map used for this level. */
	Map *mMap;

	/** Minimap used for this level. */
	Minimap *mMinimap;

	/** Leaderboard used for this level. */
	Leaderboard *mLeaderboard;

	/** Timer that count the start-up of the race (3, 2, 1, Go!) */
	GameTimer<Level> mRaceStartTimer;

	/** Text of the start-up timer*/
	sf::Text *mRaceStartTimerText;

	/** Text of the current lap. */
	sf::Text *mLapText;

	/** Texture of the menu's frame shown when the level is finished. */
	sf::Texture *mLevelFinishedMenuFrameTexture;

	/** Sprite of the menu's frame shown when the level is finished. */
	sf::Sprite *mLevelFinishedMenuFrameSprite;

	/** Texture of the menu's frame shown when the level is going. */
	sf::Texture *mLevelRunningMenuFrameTexture;

	/** Sprite of the menu's frame shown when the level is going. */
	sf::Sprite *mLevelRunningMenuFrameSprite;

	/** Sprites that will be sorted by z-axis and rendered during the next #render call. */
	std::vector<sf::Sprite*> mDepthSprites;

	/** Alpha value of the current selected menu option. */
	int mMenuSelectedOptionCurrentAlpha;

	/**
	 * Sign of the alpha increment for the selected menu option.
	 * In order to have a blink effect, this will alternate between 1 and -1.
	 */
	int mMenuSelectedOptionCurrentAlphaIncrementSign;

	/**
	 * Returns an initialized racing kart for the initial position
	 * that wraps the given kart.
	 * @param  k the Underlying kart.
	 * @return   the initial racing kart of the level start-up.
	 */
	static RacingKart initialRacingKart(Kart *k);

    /**
     * Initialize a new text with the graphics appropriate for a menu option.
     * @param  str the string used for the menu option.
     * @return     a new sf::Text configured appropriately for a menu option.
     */
	static sf::Text *initializedOptionText(const char *title);

	/**
	 * Compares the sprites and returns true if the first is further away from the
	 * camera, false if the second one is further away from the camera.
	 * @param  s1 a sprite.
	 * @param  s2 a sprite.
	 * @return    whether the first is further away from the camera.
	 */
	static bool spriteDepthCompareFunction(sf::Sprite *s1, sf::Sprite *s2);

	/** Deallocates and clears the kart collection. */
	void deleteKarts();

	/** Initialize the kart with the inital positions. */
	void initKarts();

	/** Render the sprites after sort them by "z-axis" (actually y axis). */
	void renderSpritesForDepth();

	/** Updates the progress of the karts within this level. */
	void updateKartsLapProgress();

	/**
	 * Updates the progress of the given kart within this level
	 * @param racingKart the racing kart wich progress will be updated.
	 */
	void updateKartLapProgress(RacingKart &racingKart);

	/** Checks if any kart is crashing with any other kart and handle the event appropriately. */
	void handleKartsCrash();

	/**
	 * Updates the given menu for the given key pressed.
	 * @param m       the menu to update.
	 * @param keyCode the key pressed by the user.
	 */
	void updateMenu(Menu &m, int keyCode);

	/** Update the player kart lap. */
	void updateLapText();

	/** Updates the start-up timer. */
	void updateStartTimeText();


	/**
	 * Returns whether the level has been finished by a kart.
	 * @return whether a kart finished the level.
	 */
	bool levelFinishedByAKart();

	/**
	 * Draws the given menu at the given position.
	 * @param m            the menu that will be drawn.
	 * @param frameCenterX the position of the center of the frame in the x-axis.
	 * @param frameTopY    the top of the frame in the y-axis.
	 * @param frameBottomY the bottom of the frame in the y-axis.
	 */
	void drawMenuOptions(Menu &m, int frameCenterX, int frameTopY, int frameBottomY);

	/** Draws the menu for the running level. */
	void drawInGameMenu();

	/** Draws the menu for the finished level. */
	void drawLevelFinishedMenu();

	/** Function called when the continue option is selected. */
	void continueFunc();

	/** Function called when the retry option is selected. */
	void retryFunc();

	/** Function called when the quit option is selected. */
	void quitFunc();

    const char *logTag();
    bool canLog();
};

#endif // LEVEL_H