#ifndef LEVEL_PICKER_H
#define LEVEL_PICKER_H

#include <vector>
#include "fadescreen.h"
#include "mapfactory.h"
#include "kartfactory.h"
#include "loggerinterface.h"
#include "keyboardlistener.h"

namespace sf {
    class Texture;
    class Sprite;
    class Text;
}

class LevelPicker : public FadeScreen, public KeyboardListener, public LoggerInterface {
public:
    LevelPicker(sf::RenderWindow *w, KeyboardManager *km);
    ~LevelPicker();

    void update();
    void render();

    void onKeyPressed(int keyCode);

protected:
    void fadeInFinished();
    void fadeOutFinished();

private:
    enum PickerSectionType {
        Karts,
        Maps,
        _PickerSectionCount
    };

    template <typename Type>
    struct Pickable {
        Type type;
        sf::Sprite  *sprite;
        sf::Text    *name;
    };

    template <typename Type>
    struct Pickables {
        std::vector<Pickable<Type>> elements;
        int currentSelectedIndex;
        int currentSelectedAlpha;
        int currentSelectedAlphaIncrementSign;
    };

    Pickables<KartFactory::KartType> mKarts;
    Pickables<MapFactory::MapType> mMaps;

    PickerSectionType mCurrentPickerSection;

    sf::Texture *mBackgroundTexture;
    sf::Sprite  *mBackgroundSprite;

    sf::Texture *mFrameTexture;
    sf::Sprite  *mFrameSprite;

    sf::Text    *mKartHeaderText;
    sf::Text    *mMapHeaderText;

    sf::Text *initializedPickerSectionText(const char *str);

    void addPickableKart(KartFactory::KartType type);
    void addPickableMap(MapFactory::MapType type);

    bool canLog();
    const char * logTag();
};

#endif // LEVEL_PICKER_H