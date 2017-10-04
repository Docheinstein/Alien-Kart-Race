#include <SFML/Graphics.hpp>
#include "keyboardmanager.h"
#include "levelpicker.h"
#include "config.h"
#include "fontfactory.h"
#include "resourceutil.h"
#include "level.h"
#include "sfmlutil.h"

#define LOG_TAG "{LevelPicker} "
#define CAN_LOG 1

#define BACKGROUND_COLOR_RGB                    26,     28,     28
#define PICKER_SECTION_HEADER_COLOR_RGB         178,    22,     22
#define PICKER_ELEMENT_HIGHLIGHTED_COLOR_RGB    15,     72,    189
#define PICKER_ELEMENT_SELECTED_COLOR_RGB       178,    22,     22
#define PICKER_ELEMENT_UNSELECTED_COLOR_RGB     30,     43,    71

#define PICKER_SELECTED_ELEMENT_ALPHA_HIGH 255
#define PICKER_SELECTED_ELEMENT_ALPHA_LOW  128
#define PICKER_SELECTED_ELEMENT_ALPHA_INCREMENT 3

const float PICKER_SECTION_HEADER_FONT_SIZE = Config::WINDOW_HEIGHT / 18;
const float PICKER_ELEMENT_FONT_SIZE = Config::WINDOW_HEIGHT / 20;

const float PICKER_FRAME_PADDING_TOP = Config::WINDOW_HEIGHT / 12;

const float PICKER_ELEMENT_FROM_SECION_HEADER_MARGIN_TOP = Config::WINDOW_HEIGHT / 6;
const float PICKER_NAME_FROM_ELEMENT_MARGIN_TOP = Config::WINDOW_HEIGHT / 28;

LevelPicker::LevelPicker(sf::RenderWindow *w, KeyboardManager *km) : FadeScreen(w, km) {
    // Background
    sf::Image backgroundImage;
    backgroundImage.create( Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT,
                            sf::Color(BACKGROUND_COLOR_RGB));

    mBackgroundTexture = new sf::Texture();
    mBackgroundTexture->loadFromImage(backgroundImage);

    mBackgroundSprite = new sf::Sprite();
    mBackgroundSprite->setTexture(*mBackgroundTexture);

    // Frame
	mFrameTexture = new sf::Texture();
	mFrameTexture->loadFromFile(ResourceUtil::image("level_picker_frame.png").c_str());

	mFrameSprite = new sf::Sprite();
	mFrameSprite->setTexture(*mFrameTexture);
    SFMLUtil::centerOrigin(mFrameSprite);
	mFrameSprite->setScale(Config::WINDOW_WIDTH / 640.0, Config::WINDOW_HEIGHT / 480.0);
	mFrameSprite->setPosition(
		Config::WINDOW_WIDTH / 2,
		Config::WINDOW_HEIGHT / 2
	);

    // Sections header
    mKartHeaderText = initializedPickerSectionText("Select Kart");
    mKartHeaderText->setPosition(
        Config::WINDOW_WIDTH / 2,
        mFrameSprite->getPosition().y - mFrameSprite->getGlobalBounds().height / 2
            + PICKER_FRAME_PADDING_TOP
    );
    mMapHeaderText  = initializedPickerSectionText("Select Map");
    mMapHeaderText->setPosition(
        Config::WINDOW_WIDTH / 2,
        mKartHeaderText->getPosition().y +  mFrameSprite->getGlobalBounds().height / 2
            - PICKER_FRAME_PADDING_TOP / 2
    );

    mCurrentPickerSection = Karts;

    mKarts.currentSelectedIndex = 0;
    mKarts.currentSelectedAlpha = PICKER_SELECTED_ELEMENT_ALPHA_HIGH;
    mKarts.currentSelectedAlphaIncrementSign = -1;

    mMaps.currentSelectedIndex = 0;
    mMaps.currentSelectedAlpha = PICKER_SELECTED_ELEMENT_ALPHA_HIGH;
    mMaps.currentSelectedAlphaIncrementSign = -1;

    addPickableKart(KartFactory::KartType::VenusianType);

    addPickableMap(MapFactory::MapType::EarthType);

    mInPicking = true;

    mKeysManager->addListener(this);
}

LevelPicker::~LevelPicker() {
    delete mBackgroundSprite;
    delete mBackgroundTexture;

    delete mFrameTexture;
    delete mFrameSprite;

    delete mKartHeaderText;
    delete mMapHeaderText;

    for (std::vector<Pickable<KartFactory::KartType>>::iterator i = mKarts.elements.begin();
        i != mKarts.elements.end();
        i++) {
        delete (*i).sprite;
        delete (*i).name;
    }

    for (std::vector<Pickable<MapFactory::MapType>>::iterator i = mMaps.elements.begin();
        i != mMaps.elements.end();
        i++) {
        delete (*i).sprite;
        delete (*i).name;
    }

    mKeysManager->removeListener(this);
}

// ------------------------
// PUBLIC -----------------
// ------------------------

void LevelPicker::update() {
    FadeScreen::update();
    if (mCurrentPickerSection == Karts) {
        mKarts.currentSelectedAlpha +=
            mKarts.currentSelectedAlphaIncrementSign * PICKER_SELECTED_ELEMENT_ALPHA_INCREMENT;
        if (mKarts.currentSelectedAlpha > PICKER_SELECTED_ELEMENT_ALPHA_HIGH ||
    		mKarts.currentSelectedAlpha < PICKER_SELECTED_ELEMENT_ALPHA_LOW) {
    		mKarts.currentSelectedAlphaIncrementSign *= -1;
    		mKarts.currentSelectedAlpha +=
    			mKarts.currentSelectedAlphaIncrementSign * PICKER_SELECTED_ELEMENT_ALPHA_INCREMENT;
    	}
    }
    if (mCurrentPickerSection == Maps) {
        mMaps.currentSelectedAlpha +=
            mMaps.currentSelectedAlphaIncrementSign * PICKER_SELECTED_ELEMENT_ALPHA_INCREMENT;
        if (mMaps.currentSelectedAlpha > PICKER_SELECTED_ELEMENT_ALPHA_HIGH ||
    		mMaps.currentSelectedAlpha < PICKER_SELECTED_ELEMENT_ALPHA_LOW) {
    		mMaps.currentSelectedAlphaIncrementSign *= -1;
    		mMaps.currentSelectedAlpha +=
    			mMaps.currentSelectedAlphaIncrementSign * PICKER_SELECTED_ELEMENT_ALPHA_INCREMENT;
    	}
    }
}

void LevelPicker::render() {
    mWindow->draw(*mBackgroundSprite);
    mWindow->draw(*mFrameSprite);
    mWindow->draw(*mKartHeaderText);
    mWindow->draw(*mMapHeaderText);

    for (std::vector<Pickable<KartFactory::KartType>>::iterator i = mKarts.elements.begin();
        i != mKarts.elements.end();
        i++) {
        int iterIndex = i - mKarts.elements.begin();

        float marginLeft =
            mFrameSprite->getPosition().x - mFrameSprite->getGlobalBounds().width / 2 +
            mFrameSprite->getGlobalBounds().width *
                (iterIndex + 1) / (mKarts.elements.size() + 1);

        (*i).sprite->setPosition(
            marginLeft,
            mKartHeaderText->getPosition().y + PICKER_ELEMENT_FROM_SECION_HEADER_MARGIN_TOP
        );

        (*i).name->setPosition(
            marginLeft,
            (*i).sprite->getPosition().y - (*i).sprite->getOrigin().y * (*i).sprite->getScale().y +
                (*i).sprite->getGlobalBounds().height + PICKER_NAME_FROM_ELEMENT_MARGIN_TOP
        );

        if (mKarts.currentSelectedIndex == iterIndex) {
            // Cursor over this option and this section => ighlighted and blinking text
            if (mCurrentPickerSection == Karts)
                (*i).name->setFillColor(sf::Color(  PICKER_ELEMENT_HIGHLIGHTED_COLOR_RGB,
                                                    mKarts.currentSelectedAlpha));
            // This is the selected option, but the cursor is not over this section
            else
                (*i).name->setFillColor(sf::Color(PICKER_ELEMENT_SELECTED_COLOR_RGB));
        }
        // Cursor in a different section and this option is unselected
        else
            (*i).name->setFillColor(sf::Color(PICKER_ELEMENT_UNSELECTED_COLOR_RGB));


        mWindow->draw(*(*i).sprite);
        mWindow->draw(*(*i).name);
    }


    for (std::vector<Pickable<MapFactory::MapType>>::iterator i = mMaps.elements.begin();
        i != mMaps.elements.end();
        i++) {
        int iterIndex = i - mMaps.elements.begin();

        float marginLeft =
            mFrameSprite->getPosition().x - mFrameSprite->getGlobalBounds().width / 2 +
            mFrameSprite->getGlobalBounds().width *
                (iterIndex + 1) / (mMaps.elements.size() + 1);

        (*i).sprite->setPosition(
            marginLeft,
            mMapHeaderText->getPosition().y + PICKER_ELEMENT_FROM_SECION_HEADER_MARGIN_TOP
        );

        (*i).name->setPosition(
            marginLeft,
            (*i).sprite->getPosition().y - (*i).sprite->getOrigin().y * (*i).sprite->getScale().y +
                (*i).sprite->getGlobalBounds().height + PICKER_NAME_FROM_ELEMENT_MARGIN_TOP
        );

        if (mMaps.currentSelectedIndex == iterIndex) {
            if (mCurrentPickerSection == Maps)
                // Cursor over this option and this section => ighlighted and blinking text
                (*i).name->setFillColor(sf::Color(  PICKER_ELEMENT_HIGHLIGHTED_COLOR_RGB,
                                                    mMaps.currentSelectedAlpha));
            // This is the selected option, but the cursor is not over this section
            else
                (*i).name->setFillColor(sf::Color(PICKER_ELEMENT_SELECTED_COLOR_RGB));
        }
        // Cursor in a different section and this option is unselected
        else
            (*i).name->setFillColor(sf::Color(PICKER_ELEMENT_UNSELECTED_COLOR_RGB));

        mWindow->draw(*(*i).sprite);
        mWindow->draw(*(*i).name);
    }

    FadeScreen::render();
}

void LevelPicker::onKeyPressed(int keyCode) {
    if (!mInPicking)
        return;

    // Change the section
    if (keyCode == sf::Keyboard::Key::Up ||
        keyCode == sf::Keyboard::Key::Down) {

        int indexIncrement = 0;
    	if (keyCode == sf::Keyboard::Key::Up)
    		indexIncrement = 1;
    	else
    		indexIncrement = -1;

        mCurrentPickerSection =
            static_cast<PickerSectionType>(
                (mCurrentPickerSection + indexIncrement + _PickerSectionCount)
                    % static_cast<int>(_PickerSectionCount)
            );
    }

    // Change the option within the sector
    if (keyCode == sf::Keyboard::Key::Left ||
        keyCode == sf::Keyboard::Key::Right) {

        int indexIncrement = 0;
    	if (keyCode == sf::Keyboard::Key::Right)
    		indexIncrement = 1;
    	else
    		indexIncrement = -1;

        int currentIndex;
        int elementCount;

        if (mCurrentPickerSection == Karts) {
            currentIndex = mKarts.currentSelectedIndex;
            elementCount = mKarts.elements.size();
        }
        else if (mCurrentPickerSection == Maps) {
            currentIndex = mMaps.currentSelectedIndex;
            elementCount = mMaps.elements.size();
        }

        int newIndex = (currentIndex + indexIncrement + elementCount) % elementCount;

        if (mCurrentPickerSection == Karts)
            mKarts.currentSelectedIndex = newIndex;
        else if (mCurrentPickerSection == Maps)
            mMaps.currentSelectedIndex = newIndex;
    }

    // The level will be started with the selected options
	else if (keyCode == sf::Keyboard::Key::Return) {
        mInPicking = false;
        fadeOut();
	}
}

// ------------------------
// PROTECTED --------------
// ------------------------

void LevelPicker::fadeInFinished() {

}

void LevelPicker::fadeOutFinished() {
    d("Starting level",
        "\n\tSelected kart type : ", mKarts.elements[mKarts.currentSelectedIndex].type,
        "\n\tSelected map type  : ",  mMaps.elements[mKarts.currentSelectedIndex].type
    );
    mSegue = new Level( mWindow, mKeysManager,
                        mKarts.elements[mKarts.currentSelectedIndex].type,
                        mMaps.elements[mMaps.currentSelectedIndex].type);
}


// ------------------------
// PRIVATE ----------------
// ------------------------

sf::Text *LevelPicker::initializedPickerSectionText(const char *str) {
    sf::Text *text = new sf::Text();
	text->setFont((FontFactory::font(FontFactory::FontType::Fancy)));
	text->setCharacterSize(PICKER_SECTION_HEADER_FONT_SIZE);
	text->setFillColor(sf::Color(PICKER_SECTION_HEADER_COLOR_RGB));
	text->setOutlineColor(sf::Color::White);
	text->setOutlineThickness((float)Config::WINDOW_HEIGHT / 240) ;

    text->setString(str);

    SFMLUtil::centerOrigin(text);

    return text;
}

void LevelPicker::addPickableKart(KartFactory::KartType type) {
    Pickable<KartFactory::KartType> pickerKart;
    pickerKart.type = type;

    pickerKart.sprite = KartFactory::pickerSprite(type);
	pickerKart.sprite->setScale(Config::WINDOW_WIDTH / 640.0, Config::WINDOW_HEIGHT / 480.0);
    SFMLUtil::centerOrigin(pickerKart.sprite);

    pickerKart.name = new sf::Text();
    pickerKart.name->setFont((FontFactory::font(FontFactory::FontType::Semibold)));
	pickerKart.name->setCharacterSize(PICKER_ELEMENT_FONT_SIZE);
	// pickerKart.name->setFillColor(sf::Color(PICKER_ELEMENT_UNSELECTED_COLOR_RGB));
	pickerKart.name->setOutlineColor(sf::Color::White);
	pickerKart.name->setOutlineThickness(Config::WINDOW_WIDTH / 320.0) ;
    pickerKart.name->setString(KartFactory::name(type));
    SFMLUtil::centerOrigin(pickerKart.name);

    mKarts.elements.push_back(pickerKart);
}


void LevelPicker::addPickableMap(MapFactory::MapType type) {
    Pickable<MapFactory::MapType> pickerMap;
    pickerMap.type = type;

    pickerMap.sprite = MapFactory::pickerSprite(type);
	pickerMap.sprite->setScale(Config::WINDOW_WIDTH / 640.0, Config::WINDOW_HEIGHT / 480.0);
    SFMLUtil::centerOrigin(pickerMap.sprite);

    pickerMap.name = new sf::Text();
    pickerMap.name->setFont((FontFactory::font(FontFactory::FontType::Semibold)));
	pickerMap.name->setCharacterSize(PICKER_ELEMENT_FONT_SIZE);
	// pickerMap.name->setFillColor(sf::Color(PICKER_ELEMENT_UNSELECTED_COLOR_RGB));
	pickerMap.name->setOutlineColor(sf::Color::White);
	pickerMap.name->setOutlineThickness(Config::WINDOW_WIDTH / 320.0) ;
    pickerMap.name->setString(MapFactory::name(type));
    SFMLUtil::centerOrigin(pickerMap.name);
    mMaps.elements.push_back(pickerMap);
}

const char * LevelPicker::logTag() {
	return LOG_TAG;
}

bool LevelPicker::canLog() {
	return CAN_LOG;
}