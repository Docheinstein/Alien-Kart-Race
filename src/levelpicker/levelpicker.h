#ifndef LEVEL_PICKER_H
#define LEVEL_PICKER_H

#include <vector>
#include "fadescreen.h"
#include "loggerinterface.h"
#include "keyboardlistener.h"
#include "mapfactory.h"
#include "kartfactory.h"

namespace sf {
    class Text;
}

/**
 * Screen responsible for choose the player kart and the map to use for the level.
 */
class LevelPicker : public FadeScreen, public KeyboardListener, public LoggerInterface {
public:

    /**
     * Creates a new level picker.
     * @param window      the window this screen is encapsulated in.
     * @param keysManager the optional keyboard manager that will notify this
     *                    screen about async key pressure.
     */
    LevelPicker(sf::RenderWindow *w, KeyboardManager *km);
    ~LevelPicker();

    void update();
    void render();

    void onKeyPressed(int keyCode);

protected:
    void fadeInFinished();
    void fadeOutFinished();

private:
    /** Type of a picker section. */
    enum PickerSectionType {
        /** Section for choose the player kart. */
        Karts,

        /** Section for choose the level map. */
        Maps,

        /** Count of section. */
        _PickerSectionCount
    };

    /** Genereric container representing an object that can be picked. */
    template <typename Type>
    struct Pickable {
        /** Generic type associated with this pickable entity. */
        Type type;

        /** Sprite rendered for this pickable entity. */
        sf::Sprite  *sprite;

        /** Name rendered for this pickable entity. */
        sf::Text    *name;
    };

    /** Container of generic pickable entities. */
    template <typename Type>
    struct Pickables {
        /** Pickable entities in this section. */
        std::vector<Pickable<Type>> elements;

        /* Index of the currently selected entity. */
        int currentSelectedIndex;

        /** Alpha value of the currently selected entity (needed for blink) */
        int currentSelectedAlpha;

        /**
         * Current sign of the currently selected entity
         * (needed for blink, varies between -1 and 1)
         * @return     [description]
         */
        int currentSelectedAlphaIncrementSign;
    };

    /** Pickable karts. */
    Pickables<KartFactory::KartType> mKarts;

    /** Pickable maps. */
    Pickables<MapFactory::MapType> mMaps;

    /** Current section in which the selector is choosing the pickable entity. */
    PickerSectionType mCurrentPickerSection;

    /** Texture for the background of the screen. */
    sf::Texture *mBackgroundTexture;

    /** Sprite for the background of the screen. */
    sf::Sprite  *mBackgroundSprite;

    /** Texture for the frame of the picker. */
    sf::Texture *mFrameTexture;

    /** Sprite for the frame of the picker. */
    sf::Sprite  *mFrameSprite;

    /** Text for the header of the karts section. */
    sf::Text    *mKartHeaderText;

    /** Text for the header of the maps section. */
    sf::Text    *mMapHeaderText;

    /** Whether the level picker is active (neither fading in nor out) */
    bool mInPicking;

    /**
     * Initialize a new text with the graphics appropriate for a picker section header.
     * @param  str the string used for the text.
     * @return     a new sf::Text configured appropriately for a picker section header.
     */
    sf::Text *initializedPickerSectionText(const char *str);

    /**
     * Adds a pickable kart to the pickable karts of the given type.
     * @param type the type of the kart to add to the karts section.
     */
    void addPickableKart(KartFactory::KartType type);

    /**
     * Adda a pickable map to the pickable map of the given type.
     * @param type the type of map to add to the maps section.
     */
    void addPickableMap(MapFactory::MapType type);

    bool canLog();
    const char * logTag();
};

#endif // LEVEL_PICKER_H